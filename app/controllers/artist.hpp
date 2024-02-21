#pragma once
#include <crails/cms/controllers/resource.hpp>
#include "app/models/artist_traits.hpp"
#include "app/controllers/application.hpp"
#include "lib/artist-odb.hxx"

class ArtistController : public Crails::Cms::ResourceController<ArtistTraits, ApplicationController>
{
  typedef Crails::Cms::ResourceController<ArtistTraits, ApplicationController> Super;
public:
  static constexpr const char* scope = Artist::plural_scope;

  ArtistController(Crails::Context& context) : Super(context)
  {
    vars["local_route"] = "/festival/artists";
  }

  void render_model(const Artist& model) override
  {
    render("artist/show", {
      {"model", &model}
    });
  }
};
