#include <crails/router.hpp>
#include <crails/cms/routes.hpp>
#include <crails/cms/views/admin_menu.hpp>
#include "controllers/artist.hpp"
#include "controllers/festival.hpp"
#include "controllers/event.hpp"
#include "controllers/programme.hpp"
#include "controllers/admin/artist.hpp"
#include "controllers/admin/festival.hpp"
#include "controllers/admin/event.hpp"
#include "lib/plugin-odb.hxx"

void initialize_plugin_routes()
{
  auto& router = Crails::Router::singleton::require();
  auto& cms_routes = Crails::Cms::Routes::singleton::require();
  auto& admin_menu = Crails::Cms::AdminMenu::singleton::require();

  router.scope(FestivalController::scope, [&]()
  {
    cms_routes.set_path_for<FestivalController>(router);
    router.scope(ArtistController::scope, [&]()
    {
      cms_routes.set_path_for<ArtistController>(router);
      router.match_action("GET", "/by-id/:id", ArtistController, by_id);
      router.match_action("GET", "/:slug", ArtistController, show);
    });
    router.match_action("GET", "/",          FestivalController, index);
    router.match_action("GET", "/:slug",     FestivalController, show);
    router.match_action("GET", "/by-id/:id", FestivalController, by_id);
    router.scope("/:festival_slug", [&]()
    {
      router.match_action("GET", "/:slug", EventController, show);
      router.match_action("GET", "/:slug/programme", ProgrammeController, show);
    });
  });

  router.scope(AdminApplicationController::scope, [&]()
  {
    router.scope(AdminArtistController::scope, [&]()
    {
      admin_menu.add({4, "artists", router.get_current_scope()});
      cms_routes.set_path_for<AdminArtistController>(router);
      router.libcrails_cms_admin_preview(ArtistController);
      router.libcrails_cms_admin_crud("/", AdminArtistController);
    });

    router.scope(AdminFestivalController::scope, [&]()
    {
      admin_menu.add({4, "festivals", router.get_current_scope()});
      cms_routes.set_path_for<AdminFestivalController>(router);
      router.libcrails_cms_admin_preview(FestivalController);
      router.libcrails_cms_admin_crud("/", AdminFestivalController);
      router.scope("/:festival_id/" + std::string(AdminEventController::scope), [&]()
      {
        cms_routes.set_path_for<AdminEventController>(router);
        router.match_action("POST", "/update-order", AdminEventController, update_order);
        router.libcrails_cms_admin_preview(EventController);
        router.libcrails_cms_admin_crud("/", AdminEventController);
      });
    });
  });
}
