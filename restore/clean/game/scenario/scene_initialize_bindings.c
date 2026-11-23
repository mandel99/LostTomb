#include "pharaoh/scenario/scene_initialize_bindings.h"

static uint8_t inside(void *user_data, int32_t x, int32_t y)
{
    const PharaohSceneInitializeBindings *const c = user_data;
    return pharaoh_map_grid_is_inside(c->records->grid, x, y);
}

static void floods(void *user_data, int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    const PharaohSceneInitializeBindings *const c = user_data;
    pharaoh_floods_build(c->floods, x0, y0, x1, y1);
}

static void lookup(void *user_data, int32_t orientation, int32_t force)
{
    const PharaohSceneInitializeBindings *const c = user_data;
    /* Native00430530's second stack slot is unused. */
    (void)force;
    pharaoh_city_view_calculate_lookup(c->records->view, orientation);
}

static int32_t warships(void *user_data)
{
    const PharaohSceneInitializeBindings *const c = user_data;
    return pharaoh_building_count_active_type_b6(c->records->buildings,
        *c->records->highest_building);
}

void pharaoh_saved_game_bind_scene(PharaohSavedGameInitializeOperations *ops,
    PharaohSceneInitializeBindings *c)
{
    c->initialize->operations = ops;
    c->records->initialize = c->initialize;
    c->counts->initialize = c->initialize;
    c->storage->initialize = c->initialize;
    c->export_keep->initialize = c->initialize;
    c->floods->surface = c->surface;
    c->flood_lists->random = c->surface->random;
    pharaoh_flood_build_bind_lists(c->floods, c->flood_operations, c->flood_lists);
    pharaoh_saved_game_bind_asset_steps(ops, c->assets);
    pharaoh_saved_game_bind_record_steps(ops, c->records);
    pharaoh_saved_game_bind_route_steps(ops, c->routing);
    pharaoh_saved_game_bind_count_steps(ops, c->counts);
    pharaoh_saved_game_bind_ui_steps(ops, c->ui);
    pharaoh_saved_game_bind_storage_steps(ops, c->storage);
    pharaoh_saved_game_bind_economy_steps(ops, c->economy);
    pharaoh_saved_game_bind_surface_step(ops, c->surface);
    pharaoh_saved_game_bind_moisture_step(ops, c->surface);
    pharaoh_saved_game_bind_kingdom_access_step(ops, c->kingdom);
    ops->step[PHARAOH_INIT_ROTATED_BUILDINGS] = c->rotated;
    pharaoh_saved_game_bind_export_keep(ops, c->export_keep);
    ops->user_data = c;
    ops->map_inside = inside;
    ops->build_floods = floods;
    ops->calculate_lookup = lookup;
    ops->count_warship_wharves = warships;
}
