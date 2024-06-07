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

  EventController(Crails::Context& context);

  odb::query<Festival> festival_query();
  void initialize() override;
  virtual void render_model(const Event& model) override;
};
