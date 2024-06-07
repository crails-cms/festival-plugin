#include "programme.hpp"

using namespace std;
  
ProgrammeController::ProgrammeController(Crails::Context& context)
  : EventController(context)
{
}

void ProgrammeController::render_model(const Event& model)
{
  render("festival/event/programme", {
    {"model", &model}
  });
}
