#ifndef PHARAOH_BUILDING_TYPE_QUERIES_H
#define PHARAOH_BUILDING_TYPE_QUERIES_H

#include <stdbool.h>
#include <stdint.h>

#include "pharaoh/walker/record_queries.h"
#include "pharaoh/building/model_types.h"

enum { PHARAOH_BUILDING_RECORD_SIZE = 0x108 };
enum { PHARAOH_BUILDING_RECORD_COUNT = 4000 };
enum { PHARAOH_DOCK_REGISTRY_CAPACITY = 10 };

typedef struct PharaohSunTemplePartDefinition {
    int32_t size;
    uint8_t reserved_04[0x30];
} PharaohSunTemplePartDefinition;

typedef struct PharaohConstructionCancelState {
    uint8_t current_window;
    int32_t editor_construction_tool;
    int32_t canal_connector_drag_started;
    int32_t field_6059dc;
    int32_t field_6059e0;
    int32_t monument_selections;
    int32_t field_6059d8;
    int32_t ferry_state;
    int32_t ferry_first_building;
    uint8_t redraw_requested;
} PharaohConstructionCancelState;

typedef struct PharaohBuildingTypeRecord {
    uint8_t state;
    uint8_t player;
    uint8_t reserved_id;
    uint8_t size;
    uint8_t house_is_merged;
    uint8_t house_size;
    int16_t x;
    int16_t y;
    uint8_t reserved_0a[2];
    int32_t grid_offset;
    int16_t building_type;
    int16_t subtype;
    union {
        int16_t loads_stored;
        struct { uint8_t road_network_id, native_15; };
    };
    int16_t runtime_value_016;
    int16_t houses_covered;
    uint8_t reserved_1a[2];
    int16_t house_population;
    int16_t house_population_room;
    int16_t distance_from_entry;
    union {
        uint8_t reserved_22[0x26 - 0x22];
        struct { uint8_t access_native_22[2]; uint16_t access_failure_ticks; };
    };
    int16_t road_access_x;
    int16_t road_access_y;
    int16_t walker_id;
    int16_t walker_id2;
    int16_t immigrant_walker_id;
    int16_t walker_id4;
    uint8_t reserved_32[0x35 - 0x32];
    uint8_t farm_water_access;
    uint8_t reserved_36[2];
    int16_t runtime_value_038;
    union { int16_t next_storage_building_id, next_part_building_id; };
    int16_t storage_link_amount;
    uint8_t reserved_3e[2];
    int16_t num_workers;
    uint8_t reserved_42;
    uint8_t resource_id;
    union {
        uint8_t reserved_44[0x4e - 0x44];
        struct {
            uint8_t has_road_access;
            uint8_t reserved_45[0x4e - 0x45];
        };
    };
    uint8_t tax_collector_service;
    uint8_t reserved_4f;
    int16_t formation_id;
    uint8_t reserved_52[2];
    union {
        int32_t abu_simbel_stage;
        struct {
        uint8_t pyramid_size_variant;
        uint8_t pyramid_work_phase;
        uint8_t pyramid_work_column;
        uint8_t pyramid_work_row;
        union {
            int32_t monument_stage;
            struct {
                uint8_t pyramid_target_layers;
                uint8_t pyramid_ramp1_height, pyramid_ramp2_height;
                uint8_t pyramid_prep_mask;
            };
        };
        int32_t monument_worker_id;
        union { uint8_t reserved_60[4]; int32_t royal_tomb_progress; };
        union {
            struct { int16_t pyramid_worker_id_64; uint8_t reserved_66[2]; };
            uint32_t complex_progress_flags;
        };
        union { int32_t origin_x, royal_tomb_chamber_kind; };
        int32_t origin_y;
        union {
            uint8_t reserved_70[0x84 - 0x70];
            struct { uint32_t mausoleum_orientation; uint8_t mausoleum_native_74[0x10]; };
        };
        int16_t fishing_boat_id;
        union { uint8_t reserved_86[0x8c - 0x86]; struct { uint8_t render_native_086,render_warning_087,render_native_088[4]; }; };
        int32_t house_tax_income;
        union {
            uint8_t reserved_90[8];
            struct { uint32_t royal_tomb_view; uint8_t royal_native_94[4]; };
        };
        int32_t royal_tomb_stock;
        uint8_t reserved_9c[2];
        int16_t bazaar_pottery_supply;
        int16_t bazaar_jewelry_supply;
        int16_t bazaar_linen_supply;
        int16_t bazaar_beer_supply;
        uint8_t runtime_value_a6;
        uint8_t runtime_value_a7;
        uint8_t runtime_value_a8;
        uint8_t temple_upgrade;
        uint8_t reserved_aa[0xae - 0xaa];
        int16_t dock_walker_ids[3];
        int16_t runtime_value_b4;
        int16_t shipwright_boat_id;
        };
        struct {
            uint8_t granary_reserved_54[2];
            int16_t granary_resource_stored[36];
            uint8_t granary_reserved_9e[0xb8 - 0x9e];
        };
        struct {
            int16_t farm_progress;
            uint8_t farm_reserved_56[0xb8 - 0x56];
        };
    };
    uint32_t money_value;
    uint8_t reserved_bc;
    uint8_t ruin_has_plague;
    union { uint8_t reserved_be[0xc1 - 0xbe];
        struct { uint8_t render_native_be, render_tint_bf, render_native_c0; };
    };
    union { uint8_t runtime_value_c1; uint8_t storage_id; };
    uint8_t reserved_c2[2];
    int32_t linked_ferry_building_id;
    int16_t monument_remaining_blocks;
    uint8_t runtime_value_ca;
    uint8_t repair_queue_id;
    int16_t secondary_input_stock;
    int16_t secondary_resource_model;
    union {
        uint8_t reserved_d0[0x0c];
        struct { uint32_t venue_grid_offset; int32_t venue_orientation, venue_has_plaza; };
    };
    int32_t orders_view;
    uint8_t reserved_e0[0xfc - 0xe0];
    int32_t house_plague_risk;
    uint8_t reserved_100[4];
    int16_t destination_walker_count;
    int16_t destination_payload_total;
} PharaohBuildingTypeRecord;

typedef struct PharaohHouseExpansionRecord {
    uint8_t state;
    uint8_t reserved_01[2];
    uint8_t size;
    uint8_t house_is_merged;
    uint8_t house_size;
    int16_t x;
    int16_t y;
    uint8_t reserved_0a[2];
    int32_t grid_offset;
    int16_t building_type;
    int16_t subtype;
    uint8_t reserved_14[8];
    int16_t house_population;
    uint8_t reserved_1e[0x54 - 0x1e];
    int16_t inventory[12];
    uint8_t reserved_6c[0x7e - 0x6c];
    uint8_t no_space_to_expand;
    uint8_t reserved_7f[PHARAOH_BUILDING_RECORD_SIZE - 0x7f];
} PharaohHouseExpansionRecord;

typedef struct PharaohWorkshopInputState {
    int32_t *treasury;
    int32_t *gold_delivered;
    int32_t *gold_delivery_total;
    int32_t scenario_mode;
    uint32_t tutorial_scenario;
    uint8_t mission_id;
    uint8_t *tutorial_gold_complete;
    void (*set_tutorial_flag)(void *user_data, int32_t flag);
    void *user_data;
} PharaohWorkshopInputState;

/* Complete typed translation of native 0x004D5BC0. */
void pharaoh_city_find_type_d1_anchor(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t *anchor_x, int32_t *anchor_y);
void pharaoh_buildings_reset_runtime_fields(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT]);
void pharaoh_buildings_migrate_record_layout(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT]);
void pharaoh_monuments_reset_runtime_fields(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t pyramid_style, const int32_t *pyramid_part_counts,
    const int32_t *mastaba_part_counts, int32_t *sphinx_completed);

typedef void (*PharaohBuildingConstructionAddToMap)(void *user_data,
    int32_t player, int32_t building_type, int32_t building_id,
    int32_t x, int32_t y, int32_t size, int32_t update_image,
    int32_t reserved);

void pharaoh_city_rebuild_building_tiles_and_walker_chains(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const int32_t *building_properties, const int32_t *current_player,
    int32_t *construction_size,
    PharaohBuildingConstructionAddToMap add_to_map, void *user_data);

typedef struct PharaohLegacyBuildingRecord {
    uint8_t state;
    uint8_t reserved_01[0xc3];
} PharaohLegacyBuildingRecord;

typedef struct PharaohShipwrightOrderRecord {
    int32_t rank;
    uint8_t reserved_04[0x18];
} PharaohShipwrightOrderRecord;

typedef struct PharaohHousePopulationPlayerMetrics {
    uint8_t reserved_0000[0x2708];
    int32_t people_in_huts_and_shanties;
    int32_t people_in_manors_and_up;
    uint8_t reserved_2710[8];
    int32_t last_used_house_add;
    int16_t last_used_house_remove;
    uint8_t reserved_271e[0x463c - 0x271e];
    int32_t people_in_huts;
    int32_t people_in_residences_and_up;
} PharaohHousePopulationPlayerMetrics;

typedef struct PharaohBuildingRepairQueueEntry {
    uint8_t active;
    uint8_t reserved_01[0x17];
    int32_t building_id;
} PharaohBuildingRepairQueueEntry;

typedef struct PharaohFireSearchState {
    int32_t best_unassigned_distance;
    int32_t search_remaining;
    int32_t distance_x;
    int32_t distance_y;
} PharaohFireSearchState;

int32_t pharaoh_building_is_mansion(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
int32_t pharaoh_dock_registry_index(int32_t dock_count,
    const int16_t dock_ids[PHARAOH_DOCK_REGISTRY_CAPACITY],
    int32_t building_id);
void pharaoh_building_update_highest_ids(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohLegacyBuildingRecord legacy_buildings[200],
    int32_t *highest_id, int32_t *highest_id_ever,
    int32_t *legacy_highest_id);
void pharaoh_buildings_decay_houses_covered(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t highest_id);
void pharaoh_bazaar_update_supply(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t highest_id, const PharaohHouseModel *house_models,
    int32_t bazaar_id, int32_t radius);
void pharaoh_city_ratings_prosperity_cap(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t highest_id, const PharaohHouseModel *house_models,
    int32_t *prosperity_cap);
uint32_t pharaoh_building_house_can_expand(
    PharaohHouseExpansionRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const uint32_t *terrain, const int16_t *building_ids,
    const int32_t direction_offsets[4], const int32_t *tile_offsets,
    const int32_t expansion_deltas[8], int32_t building_id,
    int32_t tile_count, int32_t *expansion_x, int32_t *expansion_y);
int32_t pharaoh_building_house_collect_expansion_stock(
    PharaohHouseExpansionRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const uint32_t *terrain, const int16_t *building_ids,
    const int32_t *tile_offsets, int32_t map_origin,
    int32_t expansion_x, int32_t expansion_y,
    int32_t building_id, int32_t tile_count,
    uint32_t stock_totals[12]);
void pharaoh_building_house_repair_map_position(
    PharaohHouseExpansionRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const int16_t *building_ids, int32_t building_id,
    int32_t grid_start, int32_t grid_width, int32_t grid_height,
    int32_t grid_border_size, uint32_t *repair_attempts,
    uint32_t *repair_failures);
int32_t pharaoh_building_maintenance_get_closest_burning_ruin(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const int16_t *burning_building_ids, int32_t burning_building_count,
    int32_t x, int32_t y, PharaohFireSearchState *search);
uint32_t pharaoh_building_advisor_option_category(uint32_t building_type);
int32_t pharaoh_laborer_pyramid_set_layout_target(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t walker_id, int32_t building_id, int32_t pyramid_style,
    const int32_t *mastaba_first_part_by_size,
    const int32_t *pyramid_first_part_by_size);
int32_t pharaoh_building_is_water_lift_footprint(
    const int16_t *building_ids,
    const PharaohBuildingTypeRecord *buildings,
    const int32_t footprint_offsets[4], int32_t grid_offset);
uint8_t pharaoh_library_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id);
uint8_t pharaoh_caesareum_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id);
uint8_t pharaoh_mausoleum_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id);
uint8_t pharaoh_sun_temple_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id);
uint8_t pharaoh_lighthouse_monument_worker_active(
    PharaohBuildingTypeRecord *buildings,
    const PharaohWalkerQueryRecord *walkers, int32_t building_id);
void pharaoh_building_destination_counters_rebuild(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT]);
int32_t pharaoh_building_stores_treasury_funds(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
int32_t pharaoh_building_workshop_add_input(
    PharaohBuildingTypeRecord *buildings, int32_t building_id,
    int32_t amount, int32_t resource,
    PharaohWorkshopInputState *state);
int32_t pharaoh_building_has_special_service_kind(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
bool pharaoh_temple_upgrade_present(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    uint32_t upgrade);
bool pharaoh_farm_has_water_access(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
int32_t pharaoh_quarry_image_group(int32_t resource);
int32_t pharaoh_building_type_is_monument(int32_t building_type);
int32_t pharaoh_building_subtype(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
void pharaoh_mausoleum_entry_tile(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    int32_t *x, int32_t *y);
int32_t pharaoh_sun_temple_part_size(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const PharaohSunTemplePartDefinition *part_definitions);
uint8_t pharaoh_sun_temple_access_tile(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const PharaohSunTemplePartDefinition *part_definitions,
    int32_t *x, int32_t *y);
bool pharaoh_ship_is_docked(const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings, int32_t walker_id);
void pharaoh_construction_ferry_cancel(int32_t *construction_state,
    PharaohBuildingTypeRecord *buildings, int32_t building_id);
bool pharaoh_construction_cancel(PharaohConstructionCancelState *state,
    PharaohBuildingTypeRecord *buildings);
void pharaoh_building_repair_queue_remove(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    PharaohBuildingRepairQueueEntry repair_queue[10]);
typedef void (*PharaohBuildingRepairDebugOutput)(
    void *user_data, const char *message);
void pharaoh_building_repair_queue_add(
    PharaohBuildingTypeRecord *buildings, int32_t building_id,
    PharaohBuildingRepairQueueEntry repair_queue[10],
    PharaohBuildingRepairDebugOutput output_debug, void *user_data);
int32_t pharaoh_shipwright_order_rank(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const PharaohShipwrightOrderRecord *orders);
int32_t pharaoh_ship_find_best_dock(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohShipwrightOrderRecord *orders, int32_t walker_id);
typedef void (*PharaohBuildingTileResolver)(void *context, int32_t x,
    int32_t y, uint8_t size, int16_t *result_x, int16_t *result_y);
void pharaoh_transport_assign_dock(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohShipwrightOrderRecord *orders, int16_t *slot_walkers,
    int32_t selected_walker_id, int32_t walker_id,
    PharaohBuildingTileResolver resolve_tile, void *resolve_context);
int32_t pharaoh_shipwright_boat_destination(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    uint32_t boat_type);
void pharaoh_building_toggle_orders_view(PharaohBuildingTypeRecord *buildings,
    int32_t building_id, uint8_t *redraw_requested);
int32_t pharaoh_building_service_need_004340e0(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled);
int32_t pharaoh_building_service_need_00434250(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled);
int32_t pharaoh_building_service_need_004343a0(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_service_need_00434510(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_service_need_00434570(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_service_need_004345d0(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_service_need_00434630(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_service_need_00434690(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_service_need_004346f0(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_service_need_00434750(
    const PharaohBuildingTypeRecord *buildings, int32_t id, int32_t enabled);
int32_t pharaoh_building_obelisk_part_number(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_building_house_tax_income(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_building_type_group_index(int32_t type);
int32_t pharaoh_building_group_index(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_building_type_in_list(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    const int32_t *zero_terminated_types);
int32_t pharaoh_building_follow_link(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_building_main_part_id(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_city_buildings_has_treasury(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id);
int32_t pharaoh_building_type_is_00489880_group(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_building_type_is_00488080_group(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_building_type_is_00487f80_group(int32_t type);
int32_t pharaoh_building_is_farm(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
uint32_t pharaoh_building_resource_is_permitted(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const int16_t *resource_permissions);
int32_t pharaoh_building_type_is_004ea9c0_group(int32_t type);
int32_t pharaoh_building_type_is_004eada0_group(int32_t type);
int32_t pharaoh_building_type_is_0046fde0_group(int32_t type);
int32_t pharaoh_building_service_status_00434060(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled);
int32_t pharaoh_building_service_need_00434150(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled, int32_t third_enabled);
int32_t pharaoh_building_service_need_004341d0(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    int32_t first_enabled, int32_t second_enabled, int32_t third_enabled);
uint8_t pharaoh_building_type_is_not_monument_group_004696b0(
    const PharaohBuildingTypeRecord *buildings, int32_t id);
int32_t pharaoh_building_count_active_type_b6(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id);
int32_t pharaoh_building_count_workshops_using_resource(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    uint32_t resource);
void pharaoh_building_adjust_active_rating_004e8e80(
    PharaohBuildingTypeRecord *buildings, int8_t delta);
int32_t pharaoh_building_count_qualifying_004ea280(
    const PharaohBuildingTypeRecord *buildings, int32_t threshold);
void pharaoh_building_clear_all(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t *highest_id_ever, int32_t *extra_sequence);
void pharaoh_city_locusts_start(
    PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t *countdown);
void pharaoh_city_victory_update_win_houses(
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t threshold, int32_t *win_house_count);
int32_t pharaoh_building_lift_has_water(
    const PharaohBuildingTypeRecord *buildings, int32_t id,
    const uint32_t *terrain, const int32_t first_offsets[4],
    const int32_t second_offsets[4]);
int32_t pharaoh_house_check_desirability(
    const PharaohBuildingTypeRecord *buildings,
    const PharaohHouseModel *house_models, int32_t id);
void pharaoh_entertainer_update_show_days(
    const PharaohWalkerQueryRecord *walkers,
    PharaohBuildingTypeRecord *buildings, int32_t walker_id);
int32_t pharaoh_formation_legion_at_building(
    const int16_t *building_id_grid,
    const PharaohBuildingTypeRecord *buildings, int32_t grid_offset);
void pharaoh_game_tick_update_tomb_material_requests(
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    uint8_t request_flags[4]);
int32_t pharaoh_building_market_max_food_stock(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id,
    const int32_t *resource_ids, int32_t resource_count);
int32_t pharaoh_building_market_max_goods_stock(
    const PharaohBuildingTypeRecord *buildings, int32_t building_id);
void pharaoh_house_service_decay_tax_collector(
    PharaohBuildingTypeRecord *buildings, int32_t highest_id);
void pharaoh_house_population_rebuild_list(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    int16_t *house_ids, int32_t *house_count);
void pharaoh_building_refresh_money_values(
    PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    uint32_t (*money_value_for_building)(void *user_data, int32_t building_id),
    void *user_data);
int32_t pharaoh_house_population_closest_with_room(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    int32_t x, int32_t y);
int32_t pharaoh_house_population_add(
    PharaohBuildingTypeRecord *buildings,
    const PharaohHouseModel *house_models,
    PharaohHousePopulationPlayerMetrics *player, int32_t requested);
int32_t pharaoh_house_population_remove(
    PharaohBuildingTypeRecord *buildings,
    PharaohHousePopulationPlayerMetrics *player, int32_t requested);
int32_t pharaoh_house_population_calculate_types(
    const PharaohBuildingTypeRecord *buildings, int32_t highest_id,
    PharaohHousePopulationPlayerMetrics *player);

#endif
