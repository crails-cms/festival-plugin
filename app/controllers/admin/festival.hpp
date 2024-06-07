#pragma once
#include <crails/cms/controllers/admin/resource.hpp>
#include "app/controllers/admin/application.hpp"
#include "app/models/festival_traits.hpp"
#include "lib/festival-odb.hxx"
#include <crails/logger.hpp>

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

  void update()
  {
    using namespace Crails;
    logger << Logger::Info << "DEBUG EFIN UPDATE BEGIN" << Logger::endl;
    Super::update();
    logger << "DEBUG EFIN UPDATE END" << Logger::endl;
  }

  bool edit_resource(Festival& model, Data data)
  {
    using namespace Crails;
    logger << Logger::Info << "DEBUG EFIN EDIT_RESOURCE" << Logger::endl;
    return Super::edit_resource(model, data);
  }

  std::string get_view_scope() const { return Festival::scope; }

  odb::query<Festival> make_index_query() const override
  {
    return Festival::make_index_query<odb::query<Festival>>(params.as_data());
  }
};
