#include "event.hpp"
#include "lib/plugin-odb.hxx"
#include <crails/utils/split.hpp>
#include <crails/utils/string.hpp>
#include <crails/url.hpp>
#include <sstream>
#include <chrono>
#include <iostream>

odb_instantiable_impl(Event)

using namespace std;

void Event::edit(Data params)
{
  Crails::Cms::Editable::edit(params);
  if (params["dates"].exists())
    date_json = params["dates"].as<string>();
  if (params["programme"].exists())
    set_programme(params["programme"].as<string>());
  if (params["order"].exists())
    set_order(params["order"].as<int>());
}

string Event::get_html_id() const
{
  stringstream stream;

  stream << "festival-event-" << get_id();
  return stream.str();
}

void Event::set_dates(const vector<time_t>& value)
{
  stringstream stream;

  stream << '[';
  for (auto it = value.begin(); it != value.end() ; ++it)
  {
    if (it != value.begin())
      stream << ',';
    stream << *it;
  }
  stream << ']';
  date_json = stream.str();
}

void Event::set_dates(const map<time_t, string>& source)
{
  stringstream stream;

  stream << '[';
  for (auto it = source.begin() ; it != source.end() ; ++it)
  {
    if (it != source.begin())
      stream << ',';
    stream << '[';
    stream << it->first;
    if (it->second.length())
      stream << ',' << std::quoted(it->second);
    stream << ']';
  }
  stream << ']';
  date_json = stream.str();
}

map<time_t, string> Event::get_dates_map() const
{
  DataTree tree;
  std::map<std::time_t, std::string> result;

  tree.from_json(date_json);
  for (Data date : tree.as_data())
  {
    std::time_t timestamp = 0;
    std::string comment;

    switch (date.count())
    {
    case 0:
      timestamp = date.as<std::time_t>();
      break ;
    case 2:
      comment = date.at(1).as<std::string>();
    case 1:
      timestamp = date.at(0).as<std::time_t>();
    }
    result.emplace(timestamp, comment);
  }
  return result;
}

vector<time_t> Event::get_dates() const
{
  vector<time_t> result;
  auto src = get_dates_map();

  for (auto it = src.begin() ; it != src.end() ; ++it)
    result.push_back(it->first);
  return result;
}

optional<time_t> Event::get_start_date() const
{
  vector<time_t> dates = get_dates();

  if (dates.size())
  {
    sort(dates.begin(), dates.end());
    return *dates.begin();
  }
  return {};
}

optional<time_t> Event::get_end_date() const
{
  vector<time_t> dates = get_dates();

  if (dates.size())
  {
    sort(dates.begin(), dates.end());
    return *dates.rbegin();
  }
  return {};
}

static time_t require_event_start_date(const Event& event)
{
  optional<time_t> tmp = event.get_start_date();

  return tmp.has_value()
    ? *tmp
    : chrono::system_clock::to_time_t(chrono::system_clock::now());
}

string Event::slug_from_title(const string& title) const
{
  stringstream slug;
  auto tmp = require_event_start_date(*this);
  auto t = std::time(&tmp);
  auto tm = *std::localtime(&t);

  slug << put_time(&tm, "%d-%m-%Y") << '-';
  slug << Crails::Url::encode(Crails::dasherize(title));
  return slug.str();
}

void Event::before_save()
{
  // Despite implementing Crails::Cms::Sortable,
  // the events are unknowing children of Festival objects,
  // so matters of ordering are handled in Festival::set_events
  //Crails::Cms::before_sortable_save(*this);
}
