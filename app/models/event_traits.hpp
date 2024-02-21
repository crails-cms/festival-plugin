#pragma once
#include "app/models/traits.hpp"
#include "event.hpp"

struct EventTraits : public ApplicationTraits
{
  typedef Event Model;
  typedef Event IndexModel;
};

