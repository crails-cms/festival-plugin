#pragma once
#include <crails/cms/controllers/admin/resource.hpp>
#include "app/controllers/admin/application.hpp"
#include "app/models/artist_traits.hpp"
#include "lib/artist-odb.hxx"
#include <iostream>

class AdminArtistController : public Crails::Cms::AdminResourceController<ArtistTraits, Artist, AdminApplicationController>
{
  typedef Crails::Cms::AdminResourceController<ArtistTraits, Artist, AdminApplicationController> Super;
public:
  static constexpr const char* scope = Artist::plural_scope;

  AdminArtistController(Crails::Context& context) : Super(context)
  {
    vars["page_title"] = i18n::t("admin.menu.artists");
    vars["page_subtitle"] = i18n::t("admin.festival.artists-subtitle");
    vars["local_route"] = std::string(Super::scope) + '/' + scope;
  }

  std::string get_view_scope() const { return Artist::scope; }
};
