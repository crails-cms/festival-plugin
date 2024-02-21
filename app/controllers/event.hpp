#pragma once
#include <crails/cms/controllers/resource.hpp>
#include "app/models/event_traits.hpp"
#include "app/controllers/application.hpp"
#include "lib/event-odb.hxx"
#include "lib/festival-odb.hxx"

class EventController : public Crails::Cms::ResourceController<EventTraits, ApplicationController>
{
  typedef Crails::Cms::ResourceController<EventTraits, ApplicationController> Super;
  std::shared_ptr<Festival> festival;
public:
  static constexpr const char* scope = "event";

  EventController(Crails::Context& context) : Super(context)
  {
  }

  odb::query<Festival> festival_query()
  {
    if (params["festival_id"].exists())
      return odb::query<Festival>::id == params["festival_id"].as<Crails::Odb::id_type>();
    return odb::query<Festival>::slug == params["festival_slug"].as<std::string>();
  }

  void initialize()
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

  void render_model(const Event& model) override
  {
    render("festival/event/show", {
      {"model", &model}
    });
  }
};
