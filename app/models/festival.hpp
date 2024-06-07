#pragma once
#include <crails/cms/models/editable.hpp>
#include "event.hpp"

#pragma db object
class Festival : public Crails::Cms::Editable
{
  odb_instantiable()
public:
  static constexpr const char* scope = "festival";
  static constexpr const char* plural_scope = "festivals";

  #pragma db view object(Festival)
  struct Count
  {
    #pragma db column("count(" + Festival::id + ")")
    size_t value;
  };

  template<typename QUERY>
  static QUERY default_order_by(const QUERY& query)
  {
    return query + "ORDER BY" + QUERY::start_date + "DESC";
  }

  template<typename QUERY>
  static QUERY make_index_query(Data params)
  {
    using namespace std;
    auto search = params["search"].template defaults_to<string>("");

    if (search.length())
      return QUERY::title.like('%' + search + '%');
    return QUERY(true);
  }

  void edit(Data) override;

  const std::vector<std::shared_ptr<Event>>& get_events() const { return events; }
  void set_events(const std::vector<std::shared_ptr<Event>>& value);
  std::time_t get_start_date() const { return start_date; }
  void set_start_date(std::time_t value) { start_date = value; }
  std::time_t get_end_date() const { return end_date; }
  void set_end_date(std::time_t value) { end_date = value; }

  void add_event(std::shared_ptr<Event>);
  void remove_event(const Event&);
  void sort_events(std::vector<std::shared_ptr<Event>>&) const;
  std::vector<std::shared_ptr<Event>> sorted_events() const;

private:
  std::time_t start_date, end_date;
  std::vector<std::shared_ptr<Event>> events;
};
