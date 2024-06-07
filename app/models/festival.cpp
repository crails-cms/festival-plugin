#include <optional>
#include <algorithm>
#include <crails/cms/time.hpp>
#include "festival.hpp"
#include "lib/plugin-odb.hxx"
#include <iostream>
#include <crails/logger.hpp>

odb_instantiable_impl(Festival)

using namespace std;
using namespace Crails;

static const char* json_date_format = "%Y-%m-%d";

void Festival::edit(Data data)
{
  logger << Logger::Info << "FESTIVAL EDIT RUNINNG" << Logger::endl;
  Crails::Cms::Editable::edit(data);
  if (data["start_date"].exists())
  {
    logger << "start date exists: " << data["start_date"].as<string>() << Logger::endl;
    start_date = string_to_time(data["start_date"], json_date_format);
    logger << "-> time_t value = " << start_date << Logger::endl;
  }
  if (data["end_date"].exists())
    end_date = string_to_time(data["end_date"], json_date_format);
}

static bool is_equal(const Event& a, std::shared_ptr<Event> b)
{
  return a.get_id() == b->get_id();
}

static bool sort_events_func(const std::shared_ptr<Event>& a, const std::shared_ptr<Event>& b)
{
  return a->get_order() < b->get_order();
}

void Festival::sort_events(std::vector<std::shared_ptr<Event>>& events) const
{
  std::sort(events.begin(), events.end(), sort_events_func);
}

std::vector<std::shared_ptr<Event>> Festival::sorted_events() const
{
  std::vector<std::shared_ptr<Event>> result(events);

  sort_events(result);
  return result;
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
  sort_events(events);
}

void Festival::add_event(std::shared_ptr<Event> event)
{
  auto it = std::find_if(events.begin(), events.end(), std::bind(is_equal, *event, std::placeholders::_1));

  if (it == events.end())
  {
    unsigned int max_order = 0;
    for (auto model : events)
      max_order = std::max(max_order, model->get_order());
    event->set_order(max_order + 1);
    events.push_back(event);
  }
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
