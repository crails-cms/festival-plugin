#include "event.hpp"
#include "festival.hpp"
#include "lib/festival-odb.hxx"
#include "lib/event-odb.hxx"
#include <crails/i18n.hpp>

using namespace std;

AdminEventController::AdminEventController(Crails::Context& context)
  : Super(context)
{
  vars["page_title"] = i18n::t("admin.menu.festival-events");
  vars["page_subtitle"] = i18n::t("admin.festival.event-subtitle");
  vars["local_route"] =
    std::string(Super::scope) + '/' +
    std::string(AdminFestivalController::scope) + '/' +
    params["festival_id"].as<std::string>() + '/' +
    scope;
}

void AdminEventController::index()
{
  std::shared_ptr<Festival> festival;
  std::vector<std::shared_ptr<Event>> list;

  database.find_one(festival, odb::query<Festival>::id == params["festival_id"].as<Crails::Odb::id_type>());
  if (festival)
    list = festival->get_events();
  render("admin/" + get_view_scope() + "/index", {
    {"models", &list}
  });
}

void AdminEventController::show()
{
  initialize_parent_resource();
  Super::show();
}

void AdminEventController::destroy()
{
  std::shared_ptr<Event> model = require_resource();

  if (initialize_parent_resource())
  {
    parent_resource->remove_event(*model);
    database.save(*parent_resource);
  }
  Super::destroy();
}

bool AdminEventController::edit_resource(Event& model, Data data)
{
  if (initialize_parent_resource())
  {
    model.edit(data);
    return true;
  }
  return false;
}

bool AdminEventController::initialize_resource(Event& model, Data data)
{
  if (initialize_parent_resource())
  {
    vars["page_subtitle"] = "Festival event edition";
    database.save(model);
    parent_resource->add_event(std::make_shared<Event>(model));
    database.save(*parent_resource);
    return true;
  }
  return false;
}

bool AdminEventController::initialize_parent_resource()
{
  if (!parent_resource)
  {
    Crails::Odb::id_type festival_id = params["festival_id"];

    database.find_one(parent_resource, odb::query<Festival>::id == festival_id);
    if (!parent_resource)
    {
      respond_with(Crails::HttpStatus::not_found);
      return false;
    }
    vars["page_title"] = parent_resource->get_title();
  }
  return true;
}
