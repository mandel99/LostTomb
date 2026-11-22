#include "pharaoh/scenario/saved_game_state_read.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohMonumentSpotAnimation) == 24,
    "saved work-spot record stride");
_Static_assert(sizeof(PharaohBuildingTypeRecord) == 0x108 &&
    offsetof(PharaohBuildingTypeRecord, building_type) == 0x10,
    "saved building scan layout");

static void upgrade(PharaohSavedGameReadContext *context,
    enum PharaohSavedGameUpgrade step)
{
    const PharaohSavedGameReadOperations *const ops = context->operations;
    ops->upgrade[step](ops->upgrade_user_data[step] != NULL ?
        ops->upgrade_user_data[step] : ops->user_data);
}

void pharaoh_saved_game_state_read(PharaohSavedGameReadContext *c,
    int32_t file)
{
    static const uint32_t spot_counts[4] = {10, 18, 22, 13};
    int32_t saved_scripts[63];
    PharaohMonumentSpotAnimation *const spots[4] = {
        c->work_spots[0], c->work_spots[1],
        c->work_spots[2], c->work_spots[3]
    };
    const PharaohSavedGameReadOperations *const ops = c->operations;
    uint32_t saved = 0;
    c->chunk_io->file_version = 0xa0;
    /* Native copies complete records to the stack, then restores only their
     * first DWORD at stride24. None of the other snapshot bytes is consumed. */
    for (uint32_t group = 0; group < 4; ++group) {
        if (group == 1) c->chunk_io->stored_schema_present = 0;
        for (uint32_t i = 0; i < spot_counts[group]; ++i)
            saved_scripts[saved++] = spots[group][i].script;
    }
    pharaoh_scenario_route_samples_free(c->routes, 50, ops->user_data,
        ops->free_route_samples);
    for (int32_t i = 0; i < 0xb5; ++i) {
        if (c->chunk_io->file_version < 0x9a && i == 0xaf) {
            ops->reset_mission_records(ops->user_data);
            break;
        }
        if (pharaoh_chunk_read_migrate(c->chunk_io, i,
                c->descriptors, file) == 0) return;
    }
    c->routes[0].point_count = 0;
    c->routes[0].sample_count = 0;
    pharaoh_scenario_routes_rebuild(c->routes, 50, ops->user_data,
        ops->rebuild_route);
    int32_t version = c->chunk_io->file_version;
    if (c->chunk_io->file_version < 0x75)
        for (uint32_t i = 0; i < 100; ++i)
            c->allowed_buildings[i] = (uint16_t)(i < 50);
    if (version < 0x97) *c->predator_variant = 0;

#define UPGRADE_BEFORE(limit, step) \
    if (version < (limit)) { \
        upgrade(c, (step)); version = c->chunk_io->file_version; \
    }
    UPGRADE_BEFORE(0x76, PHARAOH_SAVE_V76)
    UPGRADE_BEFORE(0x78, PHARAOH_SAVE_V78)
    UPGRADE_BEFORE(0x79, PHARAOH_SAVE_V79)
    UPGRADE_BEFORE(0x7a, PHARAOH_SAVE_V7A)
    UPGRADE_BEFORE(0x7b, PHARAOH_SAVE_V7B)
    if (version < 0x7c) {
        upgrade(c, PHARAOH_SAVE_V7C_CITIES);
        upgrade(c, PHARAOH_SAVE_V7C_1AD0);
        upgrade(c, PHARAOH_SAVE_V7C_BUILDINGS);
        upgrade(c, PHARAOH_SAVE_V7C_STORAGES);
        version = c->chunk_io->file_version;
    }
    if (version < 0x7d) {
        pharaoh_floodplain_settings_set_defaults(c->floodplain_settings);
        version = c->chunk_io->file_version;
    }
    if (version < 0x84)
        pharaoh_floodplain_settings_reset_next_quality(c->floodplain_settings);
    ops->apply_loaded_settings(ops->settings_user_data != NULL ?
        ops->settings_user_data : ops->user_data, 1);
    if (c->chunk_io->file_version < 0x7f) upgrade(c, PHARAOH_SAVE_V7F);
    version = c->chunk_io->file_version;
    if (c->chunk_io->file_version < 0x80)
        memset(c->service_visit_grid, 0, 0x32c40);
    if (version < 0x81) upgrade(c, PHARAOH_SAVE_V81);
    upgrade(c, PHARAOH_SAVE_ALWAYS_32F0);
    version = c->chunk_io->file_version;
    if (c->chunk_io->file_version < 0x85) *c->game_is_replay = 0;
    if (c->chunk_io->file_version < 0x86)
        c->players[*c->current_player].difficulty = c->settings->difficulty;
    UPGRADE_BEFORE(0x88, PHARAOH_SAVE_V88)
    UPGRADE_BEFORE(0x8b, PHARAOH_SAVE_V8B)
    UPGRADE_BEFORE(0x8f, PHARAOH_SAVE_V8F)
    UPGRADE_BEFORE(0x90, PHARAOH_SAVE_V90)
    if (version < 0x92) {
        upgrade(c, PHARAOH_SAVE_V92_MONUMENTS);
        upgrade(c, PHARAOH_SAVE_V92_FORMATIONS);
        version = c->chunk_io->file_version;
    }
    if (version < 0x93)
        c->floodplain_settings->last_quality = c->floodplain_settings->quality;
    UPGRADE_BEFORE(0x94, PHARAOH_SAVE_V94)
    UPGRADE_BEFORE(0x95, PHARAOH_SAVE_V95)
    UPGRADE_BEFORE(0x96, PHARAOH_SAVE_V96)
    UPGRADE_BEFORE(0x98, PHARAOH_SAVE_V98)
    UPGRADE_BEFORE(0x99, PHARAOH_SAVE_V99)
    if (version < 0x9b) {
        upgrade(c, PHARAOH_SAVE_V9B_BUILDINGS);
        upgrade(c, PHARAOH_SAVE_V9B_WORDS);
        memset(c->allowed_buildings_tail, 0, 110);
        version = c->chunk_io->file_version;
    }
    UPGRADE_BEFORE(0x9c, PHARAOH_SAVE_V9C)
#undef UPGRADE_BEFORE
    if (version < 0x9d) {
        *c->map_sweep_countdown = 0;
        *c->frogs_countdown = 0;
        *c->pyramid_speedup_countdown = 0;
        c->animation_set_bytes[0] = 1;
    }
    if (version < 0x9e) *c->map_sweep_total = 0;
    if (version < 0x9f) c->animation_set_bytes[1] = 0;
    if (version < 0xa0) {
        *c->locusts_countdown = 0;
        upgrade(c, PHARAOH_SAVE_VA0);
        *c->mummies_max = 10;
        *c->hailstorm_countdown = 0;
    }
    uint8_t present[5] = {0};
    for (uint32_t id = 1; id < 4000; ++id) {
        const PharaohBuildingTypeRecord *const b = &c->buildings[id];
        if (b->state != 1) continue;
        switch (b->building_type) {
        case 0xe3: present[0] = 1; break;
        case 0xe4: present[1] = 1; break;
        case 0xe1: present[2] = 1; break;
        case 0xe6: present[3] = 1; break;
        case 0xe5: case 0xea: case 0xeb: case 0xec:
            present[4] = 1; break;
        default: break;
        }
    }
    for (uint32_t i = 0; i < 5; ++i)
        if (present[i] == 0) ops->reset_work_spots[i](ops->user_data);
    saved = 0;
    for (uint32_t group = 0; group < 4; ++group)
        for (uint32_t i = 0; i < spot_counts[group]; ++i)
            spots[group][i].script = saved_scripts[saved++];
    *c->pyramid_speedup_active = (uint8_t)(*c->pyramid_speedup_countdown > 0);
}
