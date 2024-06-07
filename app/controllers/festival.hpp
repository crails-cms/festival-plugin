#pragma once
#include <crails/cms/controllers/resource.hpp>
#include <crails/cms/controllers/injectable.hpp>
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

  class InjectableIndex : public Crails::Cms::Injectable
  {
  public:
    InjectableIndex(const Crails::SharedVars&, Crails::RenderTarget&);
    void run() override;
    void run(Data params);
  };

  FestivalController(Crails::Context&);

  void homepage();
  void index();
  void render_model(const Festival& model) override;
};

