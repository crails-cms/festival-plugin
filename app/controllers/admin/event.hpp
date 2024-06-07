#pragma once
#include <crails/cms/controllers/admin/resource.hpp>
#include "app/controllers/admin/application.hpp"
#include "app/models/event_traits.hpp"
#include "lib/event-odb.hxx"

class Festival;

class AdminEventController : public Crails::Cms::AdminResourceController<EventTraits, Event, AdminApplicationController>
{
  typedef Crails::Cms::AdminResourceController<EventTraits, Event, AdminApplicationController> Super;
public:
  static constexpr const char* scope = "events";

  AdminEventController(Crails::Context& context);

  void index();
  void show();
  void destroy();
  void update_order();
  std::string get_view_scope() const override { return "festival-event"; }
  bool edit_resource(Event&, Data) override;
  bool initialize_resource(Event&, Data) override;
  bool initialize_parent_resource();
  bool must_protect_from_forgery() const override;

private:
  std::shared_ptr<Festival> parent_resource;
};
