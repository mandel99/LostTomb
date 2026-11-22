#include "pharaoh/scenario/saved_game_upgrades.h"
#include "pharaoh/ui/city_message_text.h"

/* These adapters carry context only; the native algorithms have one owner. */
#define STEP(name, expression) static void name(void *data) { \
    PharaohSavedGameUpgradeBindings *c = data; expression; }
STEP(v76, pharaoh_game_file_upgrade_empire_object_states(c->empire_objects))
STEP(v78, pharaoh_native_record90_set_word_minus_one_004d1840(
    (PharaohNativeRecord90 *)(void *)(c->formations + 1), 0x82))
STEP(v79, pharaoh_native_record30_set_three_words_minus_one_004d1860(c->messages, 0x12))
STEP(v7a, pharaoh_native_player_copy_seven_dwords_004d1880((uint8_t *)c->read->players, 2))
STEP(v7b, pharaoh_game_file_upgrade_widen_player_counts((PharaohUpgradePlayerRecord *)(void *)c->read->players))
STEP(v7c_cities, pharaoh_game_file_relayout_empire_cities(c->empire_cities))
STEP(v7c_buildings, pharaoh_buildings_migrate_record_layout(c->read->buildings))
STEP(v7c_storages, pharaoh_game_file_relayout_storages(c->storages))
STEP(v7f, pharaoh_game_file_upgrade_clear_walker_fields((PharaohUpgradeWalkerRecord *)(void *)((uint8_t *)(c->walkers + 1) + 2)))
STEP(v81, pharaoh_game_file_upgrade_message_text_ids((PharaohMenuMessageRecord *)(void *)c->messages))
STEP(names, pharaoh_saved_walker_names_repair(c->walkers, c->read->buildings, c->formations, c->name_counters))
STEP(v8b, pharaoh_walker_save_type_78_sources(c->walkers))
STEP(v8f, pharaoh_native_pairs_set_minus_one_004de140(c->saved_pairs))
STEP(v90, pharaoh_game_file_upgrade_event_fields((PharaohUpgradeEventRecord *)(void *)((uint8_t *)c->events + 0x6a)))
STEP(v92_monuments, pharaoh_monuments_reset_runtime_fields(c->read->buildings,
    *c->pyramid_style, c->pyramid_parts, c->mastaba_parts, c->sphinx_completed))
STEP(v92_formations, pharaoh_native_record90_clear_byte_004d3f40(
    (PharaohNativeRecord90 *)(void *)(c->formations + 1), 0x68))
STEP(v94, pharaoh_game_file_rebuild_ship_walker_ids((PharaohUpgradeFormationRecord *)(void *)c->formations, c->walkers))
STEP(v95, pharaoh_buildings_reset_runtime_fields(c->read->buildings))
STEP(v96, pharaoh_native_current_player_block_clear_004d4040((uint8_t *)&c->read->players[*c->read->current_player]))
STEP(v98, pharaoh_saved_events_upgrade_resources(c->event_count, c->events,
    c->resource_prices, c->default_resource_prices))
STEP(v99, pharaoh_game_file_upgrade_empire_object_flags(c->empire_objects))
STEP(v9b_buildings, pharaoh_native_building_field_clear_004d4080(
    (uint8_t *)c->read->buildings, 4000, 264, 0x86))
STEP(v9b_words, pharaoh_native_two_words_clear_004d40a0((uint8_t *)c->read->players))
STEP(v9c, pharaoh_native_player_expand_word_tables_004d1a70((uint8_t *)c->read->players))
STEP(va0, pharaoh_game_file_upgrade_building_orientation((PharaohUpgradeBuildingRecord *)(void *)c->read->buildings))
#undef STEP

static void histories(void *data, int32_t mode)
{
    PharaohSavedGameUpgradeBindings *const c = data;
    pharaoh_saved_histories_transfer(c->histories, c->history_snapshots,
        (uint8_t)mode, c->history_memory);
}

void pharaoh_saved_game_bind_upgrade_steps(PharaohSavedGameReadOperations *ops,
    PharaohSavedGameUpgradeBindings *bindings)
{
    static const struct { enum PharaohSavedGameUpgrade step;
        PharaohSavedGameStep apply; } steps[] = {
        {PHARAOH_SAVE_V76,v76},{PHARAOH_SAVE_V78,v78},{PHARAOH_SAVE_V79,v79},
        {PHARAOH_SAVE_V7A,v7a},{PHARAOH_SAVE_V7B,v7b},
        {PHARAOH_SAVE_V7C_CITIES,v7c_cities},{PHARAOH_SAVE_V7C_BUILDINGS,v7c_buildings},
        {PHARAOH_SAVE_V7C_STORAGES,v7c_storages},{PHARAOH_SAVE_V7F,v7f},
        {PHARAOH_SAVE_V81,v81},{PHARAOH_SAVE_ALWAYS_32F0,names},
        {PHARAOH_SAVE_V8B,v8b},{PHARAOH_SAVE_V8F,v8f},{PHARAOH_SAVE_V90,v90},
        {PHARAOH_SAVE_V92_MONUMENTS,v92_monuments},{PHARAOH_SAVE_V92_FORMATIONS,v92_formations},
        {PHARAOH_SAVE_V94,v94},{PHARAOH_SAVE_V95,v95},{PHARAOH_SAVE_V96,v96},
        {PHARAOH_SAVE_V98,v98},{PHARAOH_SAVE_V99,v99},
        {PHARAOH_SAVE_V9B_BUILDINGS,v9b_buildings},{PHARAOH_SAVE_V9B_WORDS,v9b_words},
        {PHARAOH_SAVE_V9C,v9c},{PHARAOH_SAVE_VA0,va0}
    };
    for (uint32_t i = 0; i < sizeof(steps)/sizeof(steps[0]); ++i) {
        ops->upgrade[steps[i].step] = steps[i].apply;
        ops->upgrade_user_data[steps[i].step] = bindings;
    }
    ops->apply_loaded_settings = histories;
    ops->settings_user_data = bindings;
}
