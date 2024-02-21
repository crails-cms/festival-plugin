#include <crails/i18n_settings.hpp>
#include <string>
#include <iostream>

using namespace std;

void initialize_plugin_routes();
void initialize_plugin_renderers();
void install_plugin_database();
void uninstall_plugin_database();

extern unsigned char festival_plugin_application_css[];
extern unsigned int  festival_plugin_application_css_len;
extern unsigned char festival_plugin_application_js[];
extern unsigned int  festival_plugin_application_js_len;
extern unsigned char festival_plugin_admin_js[];
extern unsigned int  festival_plugin_admin_js_len;

extern "C"
{
  void initialize()
  {
    i18n::Settings& tsettings = i18n::Settings::singleton::require();

    tsettings.t.add_locale("fr", "locales/festival.fr.json");
    initialize_plugin_routes();
    initialize_plugin_renderers();
  }

  void install()
  {
    install_plugin_database();
  }

  void uninstall()
  {
    uninstall_plugin_database();
  }

  std::string_view plugin_stylesheet()
  {
    return std::string_view(
      reinterpret_cast<const char*>(festival_plugin_application_css),
      festival_plugin_application_css_len
    );
  }

  std::string_view plugin_javascript()
  {
    return std::string_view(
      reinterpret_cast<const char*>(festival_plugin_application_js),
      festival_plugin_application_js_len
    );
  }

  std::string_view plugin_admin_javascript()
  {
    return std::string_view(
      reinterpret_cast<const char*>(festival_plugin_admin_js),
      festival_plugin_admin_js_len
    );
  }
}
