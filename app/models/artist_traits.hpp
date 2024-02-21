#pragma once
#include "app/models/traits.hpp"
#include "artist.hpp"

struct ArtistTraits : public ApplicationTraits
{
  typedef Artist Model;
  typedef Artist IndexModel;
};
