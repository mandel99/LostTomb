#include "pharaoh/simulation/flood_settings.h"

#include <stddef.h>

_Static_assert(sizeof(PharaohFloodplainSettings) == 0x24 &&
    offsetof(PharaohFloodplainSettings, quality) == 0x14 &&
    offsetof(PharaohFloodplainSettings, next_quality) == 0x1c &&
    offsetof(PharaohFloodplainSettings, last_quality) == 0x20,
    "native flood settings layout");

/* Existing completed leaves moved from flood_simulation.c without changing
 * their algorithms. Native entries004BD330,004BD360,004BD580,004BD5D0. */
void pharaoh_floodplain_settings_set_defaults(PharaohFloodplainSettings *s)
{
    s->initial_season = 180;
    s->initial_duration = 70;
    s->initial_quality = 100;
    s->season = 180;
    s->duration = 70;
    s->quality = 100;
    s->reserved = 0;
    s->next_quality = 100;
}

void pharaoh_floodplain_settings_initialize(PharaohFloodplainSettings *s)
{
    s->season = s->initial_season;
    s->duration = s->initial_duration;
    s->quality = s->initial_quality;
    s->next_quality = s->initial_quality;
    s->reserved = 0;
}

void pharaoh_floodplain_settings_reset_season(PharaohFloodplainSettings *s)
{
    s->season = s->initial_season;
    s->duration = s->initial_duration;
}

void pharaoh_floodplain_settings_reset_next_quality(PharaohFloodplainSettings *s)
{
    s->next_quality = s->initial_quality;
}
