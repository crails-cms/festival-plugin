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

string FestivalController::injectable_index(Crails::SharedVars vars)
{
  DataTree params;
  params.from_map(map<string,string>{
    {"page", "1"},
    {"count", Crails::defaults_to<string>(vars, "count", "1")}
  });
  const auto now = chrono::system_clock::now();
  const Crails::Renderer* renderer;
  vector<Festival> models;
  Crails::RenderString output;
  Crails::Paginator paginator(params);
  Crails::Odb::Connection database;
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
  vars.erase("layout");
  vars["models"] = const_cast<const vector<Festival>*>(&models);
  renderer = Crails::Renderer::pick_renderer("festival/injectable", "text/html");
  renderer->render_template("festival/injectable", output, vars);
  return string("<!-- Injected festivals --!>")
    + string(output.value());
}
