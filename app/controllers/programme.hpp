#pragma once
#include "event.hpp"

class ProgrammeController : public EventController
{
public:
  ProgrammeController(Crails::Context& context);

  void render_model(const Event& model) override;
};

