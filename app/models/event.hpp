#pragma once
#include <crails/cms/models/editable.hpp>
#include <optional>

#pragma db object
class Event : public Crails::Cms::Editable
{
  odb_instantiable()
public:
  static constexpr const char* scope = "festival-event";
  static constexpr const char* plural_scope = "festival-events";

  #pragma db view object(Event)
  struct Count
  {
    #pragma db column("count(" + Event::id + ")")
    size_t value;
  };

  template<typename QUERY>
  static QUERY default_order_by(const QUERY& query)
  {
    return query + "ORDER BY" + QUERY::event_start;
  }

  void edit(Data params);

  const std::string& get_programme() const { return programme; }
  void set_programme(const std::string& value) { programme = value; }
  void set_dates(const std::vector<std::time_t>&);
  std::vector<std::time_t> get_dates() const;
  void set_order(int value) { order = value; }
  int get_order() const { return order; }
  const std::string& get_date_json() const { return date_json; }

  std::optional<std::time_t> get_start_date() const;
  std::optional<std::time_t> get_end_date() const;

private:
  std::string programme;
  std::string date_json;
  int order;
};
