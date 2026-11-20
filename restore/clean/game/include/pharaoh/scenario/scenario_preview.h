#ifndef PHARAOH_SCENARIO_SCENARIO_PREVIEW_H
#define PHARAOH_SCENARIO_SCENARIO_PREVIEW_H

#include <stdint.h>

enum {
    PHARAOH_SCENARIO_METADATA_BYTES = 1592,
    PHARAOH_SCENARIO_EVENTS_BYTES = 18628
};

typedef struct PharaohScenarioPreview {
    uint8_t metadata[PHARAOH_SCENARIO_METADATA_BYTES];
    uint8_t events[PHARAOH_SCENARIO_EVENTS_BYTES];
    int32_t version;
    uint8_t valid;
} PharaohScenarioPreview;

/* State-0x3D portion of native 0x004D5820/0x004D4AE0. Returns zero on
   success, matching 0x004D5820. */
int32_t pharaoh_scenario_preview_load_map(
    PharaohScenarioPreview *preview, const char *path);

/* State-0x3D portion of native 0x004D58B0/0x004D41E0. Returns one on
   success, matching 0x004D58B0. */
int32_t pharaoh_scenario_preview_load_campaign_mission(
    PharaohScenarioPreview *preview, const char *package_path,
    int32_t mission_id);

#endif
