#include "pharaoh/scenario/scene_records.h"

#include "pharaoh/core/static_objects.h"

static void grid(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    c->grid->width = c->initialize->geometry->width;
    c->grid->height = c->initialize->geometry->height;
    pharaoh_map_grid_initialize(c->grid,
        c->row_x_start, c->row_x_end, c->row_start_offset);
}

static void shrubs(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_map_terrain_shrub_to_tree(c->terrain, c->grid->height,
        c->row_x_start, c->row_x_end, c->row_start_offset);
}

static void gardens(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_map_terrain_mark_gardens(c->terrain, c->property_flags,
        c->overlay_values, c->initialize->map_image_grid, c->grid->height,
        c->row_x_start, c->row_x_end, c->row_start_offset, *c->first_garden_image);
}

static void flag(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_native_flag_from_byte_004e7e20(c->derived_flag,
        c->initialize->players[1].native_value_0003);
}

static void reset_routes(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_native_dwords_set_minus_one_00517a20(c->route_reset_values);
}

static void clean_routes(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_walker_route_clean(c->route_walker_ids, c->walkers);
}

static void reset_pair(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_native_state_pair_reset_0041df60(
        c->state_pair_first, c->state_pair_second);
}

static void message_areas(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_city_message_initialize_problem_areas(
        c->message_problems, c->get_tick, c->clock_user_data);
}

static void building_tiles(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_city_rebuild_building_tiles_and_walker_chains(c->buildings,
        c->walkers, c->building_properties, c->initialize->current_player,
        c->construction_size, c->add_building_to_map, c->construction_user_data);
}

static void destinations(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_building_destination_counters_rebuild(c->buildings, c->walkers);
}

static void transport_counts(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_walker_count_active_transport_types(c->walkers, c->active_transport_count);
}

static void anchor(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_city_find_type_d1_anchor(c->buildings, c->anchor_x, c->anchor_y);
}

static void highest_building(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_building_update_highest_ids(c->buildings, c->legacy_buildings,
        c->highest_building, c->highest_building_ever, c->legacy_highest_building);
}

static void ferry_passengers(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_ferry_check_passengers(c->walkers, c->ferry_paths);
}

static void map_bounds(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    pharaoh_city_view_calculate_map_bounds(c->view);
}

static void camera_bounds(void *user_data)
{
    PharaohSceneRecords *c = user_data;
    c->view->debug_extra_camera_bounds_enabled = *c->extra_camera_bounds_enabled;
    c->view->event_54_suppress_flood = *c->event_54_suppress_flood;
    pharaoh_city_view_check_camera_boundaries(c->view);
}

void pharaoh_saved_game_bind_record_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneRecords *c)
{
    static const enum PharaohSavedGameInitializeStep steps[16] = {
        PHARAOH_INIT_MAP_GRID, PHARAOH_INIT_SHRUBS, PHARAOH_INIT_GARDENS,
        PHARAOH_INIT_FLAG, PHARAOH_INIT_ROUTE_RESET, PHARAOH_INIT_CLEAN_ROUTES,
        PHARAOH_INIT_STATE_PAIR, PHARAOH_INIT_MESSAGE_AREAS,
        PHARAOH_INIT_BUILDING_TILES, PHARAOH_INIT_DESTINATIONS,
        PHARAOH_INIT_TRANSPORT_COUNTS, PHARAOH_INIT_FIND_D1_ANCHOR,
        PHARAOH_INIT_HIGHEST_BUILDING, PHARAOH_INIT_FERRY_PASSENGERS,
        PHARAOH_INIT_MAP_BOUNDS, PHARAOH_INIT_CAMERA_BOUNDS
    };
    static const PharaohSavedGameStep functions[16] = {
        grid, shrubs, gardens, flag, reset_routes, clean_routes,
        reset_pair, message_areas, building_tiles, destinations,
        transport_counts, anchor, highest_building, ferry_passengers,
        map_bounds, camera_bounds
    };
    c->initialize->operations = ops;
    c->initialize->diamond_x_min = &c->grid->diamond_x_min;
    c->initialize->diamond_y_min = &c->grid->diamond_y_min;
    c->view->map_bounds = &c->grid->bounds;
    c->view->map_image_grid = c->initialize->map_image_grid;
    for (uint32_t i = 0; i < 16; ++i)
        ops->step[steps[i]] = (PharaohSavedGameStepBinding){c, functions[i]};
}
