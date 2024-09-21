#pragma once
#include <crails/cms/sitemap.hpp>

class FestivalSiteMap : public Crails::Cms::SiteMap::Index
{
  Crails::Cms::SiteMap::UrlSet url_set() const override;
};
