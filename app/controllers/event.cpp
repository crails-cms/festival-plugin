#include "event.hpp"

using namespace std;

EventController::EventController(Crails::Context& context) : Super(context)
{
}

odb::query<Festival> EventController::festival_query()
{
  if (params["festival_id"].exists())
    return odb::query<Festival>::id == params["festival_id"].as<Crails::Odb::id_type>();
  return odb::query<Festival>::slug == params["festival_slug"].as<string>();
}

void EventController::initialize()
{
  database.find_one(festival, festival_query());
  if (festival)
  {
    vars["festival"] = const_cast<const Festival*>(festival.get());
    Super::initialize();
  }
  else
    Super::respond_with(Crails::HttpStatus::not_found);
}

void EventController::render_model(const Event& model)
{
  render("festival/event/show", {
    {"model", &model}
  });
}
