#include "pharaoh/scenario/scene_storage.h"

static void granary_stocks(void *user_data)
{
    PharaohSceneStorage *c = user_data;
    pharaoh_granary_supply_calculate(c->granary_supply);
}

static void repair_granaries(void *user_data)
{
    PharaohSceneStorage *c = user_data;
    pharaoh_repair_granary_records(c->buildings, c->storages);
}

static void legacy_farms(void *user_data)
{
    PharaohSceneStorage *c = user_data;
    pharaoh_game_file_upgrade_farm_progress(c->buildings);
}

static void monument_slots(void *user_data)
{
    PharaohSceneStorage *c = user_data;
    pharaoh_monument_rebuild_player_slots(c->monuments);
}

void pharaoh_saved_game_bind_storage_steps(
    PharaohSavedGameInitializeOperations *ops, PharaohSceneStorage *c)
{
    c->initialize->operations = ops;
    c->granary_supply->buildings = c->buildings;
    c->granary_supply->storages = c->storages;
    c->monuments->buildings = c->buildings;
    c->monuments->players = c->initialize->players;
    c->monuments->current_player = c->initialize->current_player;
    c->monuments->highest_building = c->granary_supply->highest_building;
    c->monuments->goal_definition_ids = c->asset_monuments->goal_definition_ids;
    c->monuments->pyramid_style = c->asset_monuments->pyramid_style;
    c->monuments->pyramid_parts = pharaoh_pyramid_part_counts;
    c->monuments->mastaba_parts = pharaoh_mastaba_part_counts;
    ops->step[PHARAOH_INIT_GRANARY_STOCKS] =
        (PharaohSavedGameStepBinding){c, granary_stocks};
    ops->step[PHARAOH_INIT_REPAIR_GRANARIES] =
        (PharaohSavedGameStepBinding){c, repair_granaries};
    ops->step[PHARAOH_INIT_LEGACY_FARMS] =
        (PharaohSavedGameStepBinding){c, legacy_farms};
    ops->step[PHARAOH_INIT_MONUMENT_SLOTS] =
        (PharaohSavedGameStepBinding){c, monument_slots};
}
