#pragma once
#include <crails/cms/models/editable.hpp>

#pragma db object
class Artist : public Crails::Cms::Editable
{
  odb_instantiable()
public:
  static constexpr const char* scope = "artist";
  static constexpr const char* plural_scope = "artists";

  #pragma db view object(Artist)
  struct Count
  {
    #pragma db column("count(" + Artist::id + ")")
    size_t value;
  };

  template<typename QUERY>
  static QUERY default_order_by(const QUERY& query)
  {
    return query + "ORDER BY" + QUERY::title;
  }

  void set_name(const std::string& value) { set_title(value); }
  const std::string& get_name() const { return get_title(); }
};
