#include "pharaoh/scenario/saved_game_initialize.h"

#include <string.h>

static void run_step(PharaohSavedGameInitializeContext *c,
    enum PharaohSavedGameInitializeStep step)
{
    const PharaohSavedGameStepBinding *const binding =
        &c->operations->step[step];
    binding->apply(binding->user_data);
}

static void run_range(PharaohSavedGameInitializeContext *c,
    enum PharaohSavedGameInitializeStep first,
    enum PharaohSavedGameInitializeStep last)
{
    for (int32_t i = first; i <= last; ++i)
        run_step(c, (enum PharaohSavedGameInitializeStep)i);
}

void pharaoh_saved_game_initialize(PharaohSavedGameInitializeContext *c)
{
    const PharaohSavedGameInitializeOperations *const ops = c->operations;
    memset(c->map_image_grid, 0, 0x32c40);
    *c->current_climate = *c->scenario_climate;
    c->geometry->width = c->scenario_geometry->width;
    c->geometry->height = c->scenario_geometry->height;
    c->geometry->grid_start = c->scenario_geometry->grid_start;
    c->geometry->grid_border = c->scenario_geometry->grid_border;
    run_range(c, PHARAOH_INIT_MAP_GRID, PHARAOH_INIT_SHRUBS);
    for (uint32_t i = 0; i < 16; ++i) {
        /* Native checks each pair twice, including the pair set to -1 by
         * the first check. Preserve both queries and their ordering. */
        for (uint32_t pass = 0; pass < 2; ++pass) {
            const int32_t x = (int32_t)((uint32_t)*c->diamond_x_min +
                (uint32_t)(int32_t)c->invasion_x[i]);
            const int32_t y = (int32_t)((uint32_t)*c->diamond_y_min +
                (uint32_t)(int32_t)c->invasion_y[i]);
            if (ops->map_inside(ops->user_data, x, y) == 0) {
                c->invasion_x[i] = -1;
                c->invasion_y[i] = -1;
            }
        }
    }
    run_range(c, PHARAOH_INIT_FLAG, PHARAOH_INIT_CULTURE_COVERAGE);
    if (*c->city_view_orientation < 0 || *c->city_view_orientation > 6)
        *c->city_view_orientation = 0;
    *c->map_view_orientation = *c->city_view_orientation;
    run_range(c, PHARAOH_INIT_OBELISK_SCRIPTS, PHARAOH_INIT_BUILDING_TILES);
    if (*c->file_version < 0x8d) {
        *c->flood_revalidation_pending = 1;
        run_step(c, PHARAOH_INIT_FLOOD_REVALIDATION);
    }
    if (*c->file_version < 0x8e) run_step(c, PHARAOH_INIT_LEGACY_FARMS);
    run_step(c, PHARAOH_INIT_MAP_TILES);
    ops->build_floods(ops->user_data, 0, 0,
        (int32_t)((uint32_t)c->geometry->width - 1u),
        (int32_t)((uint32_t)c->geometry->height - 1u));
    ops->calculate_lookup(ops->user_data, *c->city_view_orientation, 1);
    run_range(c, PHARAOH_INIT_MAP_BOUNDS, PHARAOH_INIT_STATE_PAIR);
    *c->removal_undo = 0;
    *c->city_overlay = 0;
    *c->previous_city_overlay = 0;
    pharaoh_mission_briefing_set_started(c->briefing);
    const int32_t player = *c->current_player;
    c->players[player].tutorial_active_b = 1;
    c->players[player].tutorial_active_a = 1;
    run_range(c, PHARAOH_INIT_GARDENS, PHARAOH_INIT_ROTATED_BUILDINGS);
    *c->message_scroll_position = 0;
    *c->message_list_scroll = 0;
    *c->ui_flag = 0;
    run_range(c, PHARAOH_INIT_DESTINATIONS, PHARAOH_INIT_TRANSPORT_COUNTS);
    *c->health_recompute_distances = 1;
    run_range(c, PHARAOH_INIT_EMPIRE_IMAGES, PHARAOH_INIT_EMPIRE_CITY_IMAGES);
    *c->warship_wharf_count = ops->count_warship_wharves(ops->user_data);
    run_range(c, PHARAOH_INIT_REPAIR_GRANARIES, PHARAOH_INIT_FIND_D1_ANCHOR);
    if (*c->scenario_mode != 0) *c->custom_value_78d828 = 0;
    *c->file_version = 0xa0;
    run_range(c, PHARAOH_INIT_EXPORT_KEEP, PHARAOH_INIT_FERRY_PASSENGERS);
}
