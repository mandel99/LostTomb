#include "pharaoh/runtime/application_startup.h"

PharaohApplicationConfigurationResult pharaoh_application_load_configuration(
    const PharaohApplicationConfiguration *c)
{
    if (pharaoh_settings_load(c->settings) == 0) {
        return PHARAOH_APP_CONFIGURATION_SETTINGS_FAILED;
    }
    if (pharaoh_load_text_catalogs(c->texts, c->primary_text_filename,
            c->media_text_filename) == 0) {
        return PHARAOH_APP_CONFIGURATION_TEXT_FAILED;
    }
    pharaoh_settings_load_default_names(c->text_selection, c->default_names);
    return PHARAOH_APP_CONFIGURATION_READY;
}
