#pragma once
#include <crails/cms/models/editable.hpp>
#include <crails/cms/models/sortable.hpp>
#include <optional>

#pragma db object
class Event : public Crails::Cms::Editable, public Crails::Cms::Sortable
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
    return query + "ORDER BY" + QUERY::order;
  }

  void edit(Data params);

  std::string get_html_id() const;
  const std::string& get_programme() const { return programme; }
  void set_programme(const std::string& value) { programme = value; }
  void set_dates(const std::vector<std::time_t>&);
  void set_dates(const std::map<std::time_t, std::string>&);
  std::vector<std::time_t> get_dates() const;
  std::map<std::time_t, std::string> get_dates_map() const;
  const std::string& get_date_json() const { return date_json; }

  std::optional<std::time_t> get_start_date() const;
  std::optional<std::time_t> get_end_date() const;

  std::string slug_from_title(const std::string& title) const override;

  void before_save() override;

private:
  std::string programme;
  std::string date_json;
};
