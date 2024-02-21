#pragma once
#include <crails/cms/controllers/admin/resource.hpp>
#include "app/controllers/admin/application.hpp"
#include "app/models/festival_traits.hpp"
#include "lib/festival-odb.hxx"

class AdminFestivalController : public Crails::Cms::AdminResourceController<FestivalTraits, Festival, AdminApplicationController>
{
  typedef Crails::Cms::AdminResourceController<FestivalTraits, Festival, AdminApplicationController> Super;
public:
  static constexpr const char* scope = Festival::plural_scope;

  AdminFestivalController(Crails::Context& context) : Super(context)
  {
    vars["page_title"] = i18n::t("admin.menu.festivals");
    vars["page_subtitle"] = i18n::t("admin.festival.index");
    vars["local_route"] = std::string(Super::scope) + '/' + scope;
  }

  std::string get_view_scope() const { return Festival::scope; }
};
