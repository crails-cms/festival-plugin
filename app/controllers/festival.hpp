#pragma once
#include <crails/cms/controllers/resource.hpp>
#include <crails/paginator.hpp>
#include "app/models/festival_traits.hpp"
#include "app/controllers/application.hpp"
#include "lib/festival-odb.hxx"

class FestivalController : public Crails::Cms::ResourceController<FestivalTraits, ApplicationController>
{
  typedef Crails::Cms::ResourceController<FestivalTraits, ApplicationController> Super;
  Crails::Paginator paginator;
public:
  static constexpr const char* scope = Festival::scope;

  FestivalController(Crails::Context& context) : Super(context), paginator(Super::params)
  {
  }

  void homepage();
  void index();
  void render_model(const Festival& model) override;

  static std::string injectable_index(Crails::SharedVars);
};

