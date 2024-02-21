#include <optional>
#include <algorithm>
#include <crails/cms/time.hpp>
#include "festival.hpp"
#include "lib/plugin-odb.hxx"
#include <iostream>

odb_instantiable_impl(Festival)

using namespace std;

void Festival::edit(Data data)
{
  Crails::Cms::Editable::edit(data);
  if (data["start_date"].exists())
    start_date = string_to_time(data["start_date"]);
  if (data["end_date"].exists())
    end_date = string_to_time(data["end_date"]);
}

static bool is_equal(const Event& a, std::shared_ptr<Event> b)
{
  return a.get_id() == b->get_id();
}

static bool sort_events(const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b)
{
  return a->get_order() < b->get_order();
}

void Festival::set_events(const std::vector<std::shared_ptr<Event>>& value)
{
  std::optional<std::time_t> start, end;

  events = value;
  for (const auto& event : events)
  {
    std::optional<std::time_t> event_start, event_end;

    event_start = event->get_start_date();
    event_end = event->get_end_date();
    if (!start || (event_start && *event_start < *start))
      start = event_start;
    if (!end || (event_end && *event_end < *end))
      end = event_end;
  }
  start_date = start.value_or(0);
  end_date = end.value_or(0);
  std::sort(events.begin(), events.end(), sort_events);
}

void Festival::add_event(std::shared_ptr<Event> event)
{
  auto it = std::find_if(events.begin(), events.end(), std::bind(is_equal, *event, std::placeholders::_1));

  if (it == events.end())
    events.push_back(event);
  else
    *it = event;
  set_events(events);
}

void Festival::remove_event(const Event& event)
{
  auto it = std::find_if(events.begin(), events.end(), std::bind(is_equal, event, std::placeholders::_1));

  if (it != events.end())
  {
    events.erase(it);
    set_events(events);
  }
}
