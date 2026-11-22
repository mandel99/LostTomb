#ifndef PHARAOH_SIMULATION_FLOOD_SETTINGS_H
#define PHARAOH_SIMULATION_FLOOD_SETTINGS_H

#include <stdint.h>

typedef struct PharaohFloodplainSettings {
    int32_t initial_season;
    int32_t initial_duration;
    int32_t initial_quality;
    int32_t season;
    int32_t duration;
    int32_t quality;
    int32_t reserved;
    int32_t next_quality;
    int32_t last_quality;
} PharaohFloodplainSettings;

void pharaoh_floodplain_settings_set_defaults(PharaohFloodplainSettings *settings);
void pharaoh_floodplain_settings_initialize(PharaohFloodplainSettings *settings);
void pharaoh_floodplain_settings_reset_season(PharaohFloodplainSettings *settings);
void pharaoh_floodplain_settings_reset_next_quality(
    PharaohFloodplainSettings *settings);

#endif
