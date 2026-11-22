#ifndef PHARAOH_RUNTIME_CITY_SOUND_H
#define PHARAOH_RUNTIME_CITY_SOUND_H

#include <stdint.h>

#include "pharaoh/building/type_queries.h"

enum { PHARAOH_CITY_SOUND_DIRECTION_COUNT = 29 };

typedef struct PharaohCitySoundViewRecord {
    int32_t active;
    int32_t total_buildings;
    int32_t reserved_08;
    int32_t direction_buildings[PHARAOH_CITY_SOUND_DIRECTION_COUNT];
} PharaohCitySoundViewRecord;

typedef struct PharaohCitySoundInitializeContext {
    int32_t (*records)[32]; /* 70 records */
    uint32_t *last_update_tick;
    void *clock_user_data;
    uint32_t (*get_tick)(void *user_data); /* native timeGetTime */
} PharaohCitySoundInitializeContext;

/* Completed C initializer reused from MissionSetup; native0041D730. */
void pharaoh_sound_city_initialize(const PharaohCitySoundInitializeContext *c);

void pharaoh_sound_city_mark_building_view(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building_id, int32_t direction,
    const int32_t *building_type_sound_ids, int32_t sound_suppressed,
    PharaohCitySoundViewRecord *sound_views);
void pharaoh_sound_city_play(int32_t records[70][32], int32_t current_tick,
    int32_t *last_play_tick, int32_t *last_sound_id);
int32_t pharaoh_sound_city_building_ambient_category(int32_t building_type);

#endif
