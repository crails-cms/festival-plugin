#pragma once
#include <crails/cms/controllers/resource.hpp>
#include "app/models/programme_traits.hpp"
#include "app/controllers/application.hpp"
#include "lib/programme-odb.hxx"

class ProgrammeController : public Crails::Cms::ResourceController<ProgrammeTraits, ApplicationController>
{
  typedef Crails::Cms::ResourceController<ProgrammeTraits, ApplicationController> Super;
public:
  static constexpr const char* scope = Programme::plural_scope;

  ProgrammeController(Crails::Context& context) : Super(context)
  {
  }

  void render_model(const Programme& model) override
  {
    render("editable", {
      {"model", reinterpret_cast<const Crails::Cms::Editable*>(&model)}
    });
  }
};

