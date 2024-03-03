#include "festival.hpp"
#include "lib/plugin-odb.hxx"
#include "../models/festival_traits.hpp"
#include <crails/any_cast.hpp>
#include <crails/renderer.hpp>

using namespace std;

void FestivalController::homepage()
{
}

void FestivalController::index()
{
  odb::result<FestivalIndexQuery> list;
  odb::query<Festival> query(true);
  vector<Festival> models;

  paginator.decorate_view(vars, [this]()
  {
    return database.count<Festival>();
  });
  paginator.decorate_query(query);
  database.find<FestivalIndexQuery>(list);
  for (const auto& entry : list)
    models.push_back(entry);
  render("festival/index", {
    {"models", const_cast<const vector<Festival>*>(&models)}
  });
}

void FestivalController::render_model(const Festival& model)
{
  render("festival/show", {
    {"model", &model}
  });
}

void FestivalController::InjectableIndex::run()
{
  DataTree params;

  params.from_map(map<string,string>{
    {"page", "1"},
    {"count", Crails::defaults_to<string>(vars, "count", "1")}
  });
  run(params.as_data());
}

FestivalController::InjectableIndex::InjectableIndex(const Crails::SharedVars& vars, Crails::RenderTarget& sink)
  : Crails::Cms::Injectable(vars, sink)
{
}

void FestivalController::InjectableIndex::run(Data params)
{
  const auto now = chrono::system_clock::now();
  vector<Festival> models;
  Crails::Paginator paginator(params);
  odb::result<FestivalIndexQuery> list;
  odb::query<Festival> query =
    odb::query<Festival>::end_date >= chrono::system_clock::to_time_t(now);
  query = query + "ORDER BY" + odb::query<Festival>::start_date + "DESC";

  paginator.decorate_view(vars, [&]()
  {
    return database.count<Festival>();
  });
  paginator.decorate_query(query);
  database.find<FestivalIndexQuery>(list, query);
  for (const auto& entry : list)
    models.push_back(entry);
  render("festival/injectable", {
    {"models", const_cast<const vector<Festival>*>(&models)}
  });
}
