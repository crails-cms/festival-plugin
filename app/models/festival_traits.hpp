#pragma once
#include "app/models/traits.hpp"
#include "festival.hpp"

#pragma db view object(Festival)
struct FestivalIndexQuery
{
  std::string get_database_name() const { return Festival().get_database_name(); }

  Crails::Odb::id_type id;
  std::string title;
  std::string slug;
  std::string description;
  std::string thumbnail;
  std::time_t start_date, end_date;

  operator Festival() const
  {
    Festival festival;

    festival.set_id(id);
    festival.set_title(title);
    festival.set_slug(slug);
    festival.set_description(description);
    festival.set_thumbnail_url(thumbnail);
    festival.set_start_date(start_date);
    festival.set_end_date(end_date);
    return festival;
  }
};

struct FestivalTraits : public ApplicationTraits
{
  typedef Festival Model;
  typedef Festival IndexModel;
};
