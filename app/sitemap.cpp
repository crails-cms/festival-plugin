#include "sitemap.hpp"
#include "models/artist_traits.hpp"
#include "models/festival_traits.hpp"
#include "controllers/festival.hpp"
#include "controllers/artist.hpp"
#include "../lib/plugin-odb.hxx"
#include <crails/odb/connection.hpp>
#include <crails/cms/routes.hpp>

using namespace Crails::Cms::SiteMap;

UrlSet FestivalSiteMap::url_set() const
{
  const auto& routes = Crails::Cms::Routes::singleton::require();
  Crails::Odb::ConnectionHandle database;
  odb::result<FestivalTraits::Model> festivals;
  odb::result<ArtistTraits::IndexModel> artists;
  UrlSet urls;

  database.find<FestivalTraits::Model>(festivals);
  for (const auto& festival : festivals)
  {
    urls.push_back({
      festival.get_title(),
      routes.get_path_for<FestivalController>('/' + festival.get_slug()),
      festival.get_updated_at(),
    });
    for (const auto& event : festival.get_events())
    {
      urls.push_back({
        festival.get_title() + " > " + event->get_title(),
        routes.get_path_for<FestivalController>('/' + festival.get_slug() + '/' + event->get_slug()),
        event->get_updated_at()
      });
    }
  }

  database.find<ArtistTraits::IndexModel>(artists);
  for (const auto& artist : artists)
  {
    urls.push_back({
      artist.get_title(),
      routes.get_path_for<ArtistController>('/' + artist.get_slug()),
      artist.get_updated_at(),
      6
    });
  }
  
  return urls;
}
