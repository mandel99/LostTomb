#include "pharaoh/scenario/scene_initialize_bindings.h"
#include "pharaoh/building/venue_images.h"
#include "pharaoh/scenario/saved_scene_storage.h"

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
    c->rotated->surface = c->surface;
    c->rotated->venue->cleared = c->routing->citizen;
    c->rotated->highest_id = c->records->highest_building;
    c->rotated->pyramid_style = c->storage->asset_monuments->pyramid_style;
    c->rotated->pyramid_part_counts = (const int32_t *)pharaoh_pyramid_part_counts;
    c->rotated->mastaba_part_counts = (const int32_t *)pharaoh_mastaba_part_counts;
    c->rotated_operations = c->rotated->operations != NULL ?
        *c->rotated->operations : (PharaohRotatedBuildingOperations){0};
    c->surface->pyramid_style =
        (const uint32_t *)c->storage->asset_monuments->pyramid_style;
    c->surface->pyramid_regular = pharaoh_pyramid_regular_special;
    c->surface->pyramid_style_five = pharaoh_pyramid_mastaba_special;
    c->surface->pyramid_complex_offsets = pharaoh_pyramid_complex_temple_offsets;
    c->pyramid_images = (PharaohPyramidImageContext){c->surface,
        &pharaoh_pyramid_image_initial_data,
        pharaoh_pyramid_part_counts, pharaoh_mastaba_part_counts};
    pharaoh_rotated_bind_venue_adjacent_images(&c->rotated_operations,c->surface);
    pharaoh_rotated_bind_pyramid_image(&c->rotated_operations,&c->pyramid_images);
    c->rotated->operations = &c->rotated_operations;
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
    pharaoh_saved_game_bind_rotated_buildings_step(ops, c->rotated);
    pharaoh_saved_game_bind_export_keep(ops, c->export_keep);
    ops->user_data = c;
    ops->map_inside = inside;
    ops->build_floods = floods;
    ops->calculate_lookup = lookup;
    ops->count_warship_wharves = warships;
}

/* Attach the deserialized scene, not preview/map-only storage. Run after the
 * reader and before initialization: map origin/dimensions are serialized and
 * may change between missions. Mutable process-lifetime tables and child
 * providers are retained in the caller's contexts. */
void pharaoh_saved_game_bind_scene_storage(
    PharaohSceneInitializeBindings *c, PharaohSavedSceneStorage *scene,
    const PharaohSavedGameReadContext *reader)
{
    const PharaohSavedSceneViews *const v = &scene->view;
    PharaohSavedGameInitializeContext *const init = c->initialize;
    PharaohMapSurfaceContext *const surface = c->surface;
    const uint8_t *const metadata = v->metadata;

    /* 004D52A0: source7848D8, climate7848DC, geometry784904,
     * invasion pairs784BC0/784BE0. These are byte offsets in chunk75;
     * they do not depend on Ghidra DAT pointer units. */
    init->scenario_climate = (const int16_t *)(const void *)(metadata + 4);
    init->scenario_geometry =
        (const PharaohSavedGameGeometry *)(const void *)(metadata + 0x2c);
    init->invasion_x = (int16_t *)(void *)(v->metadata + 0x2e8);
    init->invasion_y = (int16_t *)(void *)(v->metadata + 0x308);
    init->map_image_grid = v->images;
    init->players = v->players;
    init->current_player = v->current_player;
    init->city_view_orientation = v->city_orientation;
    init->file_version = &reader->chunk_io->file_version;

    c->records->terrain = v->terrain;
    c->records->property_flags = v->properties;
    c->records->buildings = v->buildings;
    c->records->walkers = v->walkers;
    c->records->route_walker_ids = scene->destinations[22];

    c->routing->geometry = init->geometry;
    c->routing->terrain = v->terrain;
    c->routing->building_ids = v->building_ids;
    c->routing->buildings = v->buildings;
    c->routing->random = v->random;
    c->routing->images = v->images;
    c->routing->edges = v->edges;
    c->routing->city_orientation = v->city_orientation;

    c->counts->buildings = v->buildings;
    c->counts->walkers = v->walkers;
    c->counts->roads->terrain = v->terrain;
    c->counts->roads->citizen_routing =
        (const int8_t *)c->routing->citizen;
    c->counts->access->building_ids = v->building_ids;

    c->storage->buildings = v->buildings;
    c->storage->storages = (PharaohBuildingStorageRecord *)(void *)v->storages;
    c->kingdom->buildings = v->buildings;
    c->kingdom->players = v->players;
    c->kingdom->current_player = v->current_player;
    c->kingdom->routes = c->routing;
    c->kingdom->surface = surface;
    c->kingdom->view = c->records->view;
    c->export_keep->events =
        (PharaohScenarioEvent *)(void *)((uint8_t *)scene->destinations[136] + 4);
    c->export_keep->event_count = scene->destinations[136];
    c->export_keep->monuments->players = v->players;
    c->export_keep->monuments->current_player = v->current_player;

    surface->map->terrain = v->terrain;
    surface->map->origin = init->scenario_geometry->grid_start;
    surface->map->width = init->scenario_geometry->width;
    surface->map->height = init->scenario_geometry->height;
    surface->geometry = c->records->grid;
    surface->border_size = &init->geometry->grid_border;
    surface->row_x_start = c->records->row_x_start;
    surface->row_x_end = c->records->row_x_end;
    surface->row_start_offset = c->records->row_start_offset;
    surface->height_values = v->height_values;
    surface->image_ids = v->images;
    surface->property_flags = v->properties;
    surface->edge_flags = v->edges;
    surface->canal_water = v->canal_water;
    surface->fertility = v->fertility;
    surface->variation_tag = v->variation_tag;
    surface->meadow_value = v->meadow_value;
    surface->random = v->random;
    /* surface->climate remains the caller-owned current-climate view;
       native004D52A0 updates it from scenario_climate during initialize. */
    surface->building_ids = v->building_ids;
    surface->buildings = v->buildings;
    c->floods->map = surface->map;
    c->floods->surface = surface;
}
