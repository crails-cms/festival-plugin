#include "event.hpp"
#include "lib/plugin-odb.hxx"
#include <crails/utils/split.hpp>
#include <iostream>

odb_instantiable_impl(Event)

using namespace std;

void Event::edit(Data params)
{
  cout << "Event::edit:\n" << params.to_json() << endl << endl;
  Crails::Cms::Editable::edit(params);
  if (params["dates"].exists())
    date_json = params["dates"].as<string>();
  if (params["programme"].exists())
    set_programme(params["programme"].as<string>());
  if (params["order"].exists())
    set_order(params["order"].as<int>());
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

vector<time_t> Event::get_dates() const
{
  vector<time_t> result;

  if (date_json.length() > 2)
  {
    string_view data(&date_json[1], date_json.length() - 2);
    list<string_view> parts = Crails::split(data, ',');

    result.reserve(parts.size());
    for (const auto date_string : parts)
    {
      stringstream stream;
      time_t date;

      stream << date_string;
      stream >> date;
      result.push_back(date);
    }
  }
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
