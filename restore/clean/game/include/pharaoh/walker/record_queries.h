#ifndef PHARAOH_WALKER_RECORD_QUERIES_H
#define PHARAOH_WALKER_RECORD_QUERIES_H

#include <stddef.h>
#include <stdint.h>

#include "pharaoh/core/ring_queue.h"

typedef struct PharaohBuildingTypeRecord PharaohBuildingTypeRecord;
typedef struct PharaohBuildingModelRecord PharaohBuildingModelRecord;

enum {
    PHARAOH_WALKER_RECORD_COUNT = 2000,
    PHARAOH_WALKER_RECORD_SIZE = 0x184
};

typedef struct PharaohWalkerQueryRecord {
    uint8_t reserved_000;
    uint8_t animation_frame;
    uint8_t reserved_002[2];
    int32_t image_id;
    int16_t next_walker_id_on_same_tile;
    uint8_t type;
    union { uint8_t counting_action_00b, resource_id; };
    uint8_t use_cross_country;
    uint8_t reserved_00d;
    uint8_t state;
    uint8_t owner;
    uint8_t action_state_before_6_7;
    uint8_t direction;
    uint8_t previous_direction;
    uint8_t reserved_013;
    int16_t x;
    int16_t y;
    uint8_t reserved_018[4];
    uint8_t missile_damage;
    uint8_t damage;
    uint8_t reserved_01e[2];
    int32_t grid_offset;
    int16_t destination_x;
    int16_t destination_y;
    int32_t destination_grid_offset;
    int16_t source_x;
    int16_t source_y;
    int16_t ship_landing_x;
    int16_t ship_landing_y;
    uint8_t reserved_034[2];
    int16_t wait_ticks;
    uint8_t action_state;
    int8_t progress_on_tile;
    int16_t slot_link_id;
    int16_t route_progress;
    int16_t route_length;
    uint8_t reserved_040;
    uint8_t is_on_road;
    int16_t max_roam_length;
    int16_t roam_length;
    uint8_t reserved_046[4];
    union { int16_t monument_from_building, cross_country_x; };
    union { int16_t monument_from_position, cross_country_y; };
    int16_t monument_to_building;
    int16_t monument_to_position;
    int16_t monument_travel_progress;
    int16_t monument_travel_length;
    uint8_t reserved_056[0x5a - 0x56];
    int16_t building_id;
    int16_t related_building_id_5c;
    int16_t entertainment_building_id;
    union { int16_t herd_target_id_60, formation_id_60; };
    union {
        uint8_t reserved_062[0x65 - 0x62];
        struct { uint8_t displaced_reserved_062[2], displaced_people; };
    };
    uint8_t is_ghost;
    uint8_t reserved_066[2];
    int16_t leading_walker_id;
    uint8_t reserved_06a[2];
    int8_t cart_offset_x;
    int8_t cart_offset_y;
    uint8_t reserved_06e[2];
    int16_t name_id;
    uint8_t terrain_usage;
    uint8_t reserved_073;
    uint16_t destination_payload_074;
    union { uint8_t reserved_076[0x7b - 0x76];
        struct { uint8_t is_boat, high_layer_077, current_height,
            target_height, reserved_07a; };
    };
    uint8_t dock_wait_ticks;
    uint8_t reserved_07c[0x7f - 0x7c];
    uint8_t trader_record_id;
    uint8_t wait_ticks_next_target;
    uint8_t reserved_081;
    int16_t target_walker_id;
    int16_t target_id_084;
    int16_t created_sequence;
    int16_t target_walker_created_sequence;
    union { uint8_t reserved_08a, tile_chain_ordinal; };
    uint8_t combat_flag_08b;
    int16_t combat_value_08c;
    int16_t combat_value_08e;
    int16_t combat_value_090;
    union { uint8_t reserved_092[0x98 - 0x92];
        struct { uint8_t ship_formation_id_092, reserved_093[5]; }; };
    int32_t name_group;
    int32_t carried_goods[4];
    uint8_t reserved_0ac[0xbc - 0xac];
    int32_t buyer_amounts[4];
    uint8_t reserved_0cc[0x11c - 0xcc];
    int32_t formation_id;
    union { int16_t monument_action_120, pyramid_walk_mode; };
    uint8_t reserved_122[2];
    int32_t transport_state_124;
    int32_t hit_points;
    int32_t maximum_hit_points;
    uint8_t transport_state_130;
    uint8_t reserved_131;
    int16_t request_event_id;
    uint8_t reserved_134[0x13c - 0x134];
    int32_t movement_speed;
    uint8_t reserved_140[0x0c];
    int16_t saved_source_x;
    int16_t saved_source_y;
    uint8_t reserved_150[4];
    uint8_t transport_state_154;
    uint8_t reserved_155[0x15c - 0x155];
    int32_t transport_order_15c;
    uint8_t reserved_160[0x20];
    int32_t cart_image_id;
} PharaohWalkerQueryRecord;

typedef struct PharaohWalkerMovementRecord {
    uint8_t reserved_000[9];
    uint8_t direction;
    uint8_t reserved_00a[0x39 - 0x0a];
    uint8_t movement_tick;
    uint8_t reserved_03a[0x42 - 0x3a];
    int16_t movement_x;
    int16_t movement_y;
    uint8_t reserved_046[0x5d - 0x46];
    uint8_t ghost_visible;
    uint8_t reserved_05e[0x6f - 0x5e];
    uint8_t height_adjusted_ticks;
    uint8_t height_current;
    uint8_t height_target;
    uint8_t reserved_072[PHARAOH_WALKER_RECORD_SIZE - 0x72];
} PharaohWalkerMovementRecord;

void pharaoh_walker_movement_advance_tick(
    PharaohWalkerMovementRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id);
void pharaoh_walker_step_field_4a_toward_4e(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id);
void pharaoh_walker_step_field_4c_toward_50(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id);
void pharaoh_tower_sentry_movement_ticks(
    PharaohWalkerMovementRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t walker_id, int32_t ticks);
uint32_t pharaoh_walker_update_water_carrier(
    PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings,
    const int32_t *current_walker_id,
    int32_t map_view_orientation,
    int32_t walking_image_base,
    int32_t corpse_image_base,
    const uint8_t *corpse_frame_offsets,
    int32_t *walker_image_direction,
    void (*update_service_roaming)(void *user_data, int32_t primary_owner),
    void *user_data);
int32_t pharaoh_constable_find_closest_enemy(
    const PharaohWalkerQueryRecord *walkers,
    int32_t x, int32_t y, int32_t max_distance,
    int32_t *candidate_count, int32_t *search_best_distance,
    int32_t candidate_ids[100], int32_t candidate_offsets[100],
    int32_t (*find_first_target)(void *user_data, int32_t x, int32_t y,
        const int32_t *offsets, int32_t count),
    void *user_data);
int32_t pharaoh_constable_try_engage_enemy(
    PharaohWalkerQueryRecord *walkers, int16_t *slot_walkers,
    int32_t walker_id, int32_t security_breach_duration,
    int32_t enemy_formation_count,
    int32_t *candidate_count, int32_t *search_best_distance,
    int32_t candidate_ids[100], int32_t candidate_offsets[100],
    int32_t (*find_first_target)(void *user_data, int32_t x, int32_t y,
        const int32_t *offsets, int32_t count),
    void *user_data);

_Static_assert(sizeof(PharaohWalkerQueryRecord) == PHARAOH_WALKER_RECORD_SIZE,
    "PharaohWalkerQueryRecord size must match the native walker record");
_Static_assert(offsetof(PharaohWalkerQueryRecord, request_event_id) == 0x132,
    "request_event_id must match the native walker field");
_Static_assert(offsetof(PharaohWalkerQueryRecord, transport_order_15c) == 0x15c,
    "transport_order_15c must match the native walker field");

typedef struct PharaohWalkerTypeProperties {
    int32_t category;
    int32_t maximum_damage;
    int32_t attack;
    int32_t defense;
    int32_t missile_defense;
    int32_t missile_attack;
    int32_t missile_range;
    int32_t missile_delay;
    int32_t speed;
    int32_t invasion_percentage;
    int32_t fast_run_tile_limit;
    int32_t active_move_call_limit;
    int32_t model_column_m;
    int32_t model_column_n;
} PharaohWalkerTypeProperties;

typedef struct PharaohFerryPathRecord {
    uint8_t active;
    uint8_t reserved_01[3];
    int32_t child_walker_ids[4];
    uint8_t reserved_14[0x2c - 0x14];
} PharaohFerryPathRecord;

_Static_assert(sizeof(PharaohFerryPathRecord) == 0x2c,
    "PharaohFerryPathRecord size must match the native ferry path record");

typedef struct PharaohFormationTotalsRecord {
    uint8_t state;
    uint8_t reserved_01;
    uint8_t legion_id;
    uint8_t reserved_03;
    int16_t kind;
    uint8_t reserved_06[2];
    int16_t member_ids[16];
    uint8_t reserved_28[2];
    int16_t movement_mode;
    int16_t field_2c;
    int16_t anchor_x;
    int16_t anchor_y;
    uint8_t reserved_32[4];
    int16_t coordinate_x_36;
    int16_t coordinate_y_38;
    uint8_t reserved_3a[8];
    uint8_t assignment_cursor;
    uint8_t reserved_43[0x19];
    int16_t field_5c;
    int16_t field_5e;
    uint8_t reserved_60[9];
    uint8_t reserved_slot_69;
    uint8_t enemy_type;
    uint8_t direction;
    int16_t previous_x;
    int16_t previous_y;
    uint8_t direction_delay;
    uint8_t reserved_71[0x1f];
} PharaohFormationTotalsRecord;

enum {
    PHARAOH_WALKER_NAME_COUNTER_COUNT = 41
};

typedef struct PharaohWalkerNameCounters {
    int32_t by_slot[PHARAOH_WALKER_NAME_COUNTER_COUNT];
} PharaohWalkerNameCounters;

int32_t pharaoh_caravan_leader_of_donkey(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
int32_t pharaoh_walker_count_assigned_to_building(
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    uint32_t walker_type);
uint8_t pharaoh_walker_route_get_first_available(
    PharaohWalkerQueryRecord *walkers, int32_t walker_id,
    int16_t route_owners[1000], int32_t *route_slot);
void pharaoh_formation_calculate_legion_totals(
    PharaohFormationTotalsRecord formations[50],
    const PharaohWalkerQueryRecord *walkers, int32_t *legion_count,
    int32_t *last_legion_id, int32_t *kind13_count);
int32_t pharaoh_formation_allocate_group(
    PharaohFormationTotalsRecord formations[50], uint16_t kind,
    int32_t mode, int32_t orientation, uint16_t x, uint16_t y);
void pharaoh_formation_update_directions(
    PharaohFormationTotalsRecord formations[50],
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT]);
void pharaoh_monument_worker_reset_travel(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const int16_t *building_id_grid, int32_t walker_id);
uint8_t pharaoh_monument_worker_route_has_gap(
    const PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohBuildingTypeRecord *buildings,
    const int16_t *building_id_grid, int32_t walker_id,
    uint32_t pyramid_style, const int32_t *regular_part_counts,
    const int32_t *style_five_part_counts);
void pharaoh_legion_decrease_walker_damage(
    PharaohWalkerQueryRecord *walkers);
int32_t pharaoh_walker_type_is_herd_animal(int32_t walker_type);
int32_t pharaoh_herd_animals_count_active_for_building(
    const PharaohWalkerQueryRecord *walkers, int32_t building_id);
int32_t pharaoh_herd_animals_count_action_for_building(
    const PharaohWalkerQueryRecord *walkers, int32_t building_id,
    uint32_t action_state);
bool pharaoh_walker_is_queued_for_ferry(
    const PharaohWalkerQueryRecord *walkers,
    const PharaohFerryQueueRecord *ferry_queues, int32_t walker_id);
uint32_t pharaoh_schoolchild_update(PharaohWalkerQueryRecord *walkers,
    int32_t walker_id, int32_t image_base);
uint32_t pharaoh_worker_update(PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings, int32_t walker_id);
int32_t pharaoh_walker_route_find_ferry_on_path(
    const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings,
    const int16_t *building_id_grid, const uint8_t *route_paths,
    const int32_t *direction_deltas, int32_t walker_id,
    int32_t ferry_building_id);
int32_t pharaoh_walker_action_is_6_7_22_or_23(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
int32_t pharaoh_warship_strength(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
void pharaoh_walker_set_state_dead(
    PharaohWalkerQueryRecord *walkers, int32_t walker_id);
void pharaoh_figure_seagulls_action(
    PharaohWalkerQueryRecord *walkers, int32_t current_walker_id);
void pharaoh_boat_init_transport(PharaohWalkerQueryRecord *walkers,
    int32_t walker_id, const PharaohWalkerTypeProperties *properties);
int32_t pharaoh_warship_find_target_at(
    const PharaohWalkerQueryRecord *walkers, const int16_t *walker_id_grid,
    int32_t grid_offset, int32_t excluded_walker_id);
int32_t pharaoh_warship_find_ram_target(
    const PharaohWalkerQueryRecord *walkers, const int16_t *walker_id_grid,
    const int32_t direction_deltas[8], int32_t grid_start,
    int32_t grid_width, int32_t walker_id);
int32_t pharaoh_walker_find_combat_target_on_tile(
    const PharaohWalkerQueryRecord *walkers, const int16_t *walker_id_grid,
    int32_t walker_id);
int32_t pharaoh_walker_find_projectile_victim_on_tile(
    const PharaohWalkerQueryRecord *walkers, const int16_t *walker_id_grid,
    int32_t walker_id);
int32_t pharaoh_walker_find_nearest_unassigned_soldier(
    const PharaohWalkerQueryRecord *walkers, int32_t x, int32_t y,
    uint32_t *best_distance, uint32_t *distance_dx, uint32_t *distance_dy);
int32_t pharaoh_buyer_assert_no_predecessor(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
int32_t pharaoh_trade_ship_can_leave_dock(
    PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings, int32_t walker_id);
int32_t pharaoh_trade_ship_dock_activity(
    const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings, int32_t walker_id);
void pharaoh_ferry_check_passengers(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    const PharaohFerryPathRecord paths[50]);
int32_t pharaoh_walker_enemy_near_building_site(
    const PharaohWalkerQueryRecord *walkers,
    int32_t first_x, int32_t first_y, int32_t second_x, int32_t second_y);
void pharaoh_walker_kill_tower_sentries_at(
    PharaohWalkerQueryRecord *walkers, int32_t x, int32_t y,
    uint32_t *distance_dx, uint32_t *distance_dy);
uint8_t pharaoh_sphinx_walker_is_working_on(
    const PharaohWalkerQueryRecord *walkers, const int16_t *building_id_grid,
    const int16_t *building_types, uint32_t walker_type);
uint8_t pharaoh_obelisk_walker_is_working_on(
    const PharaohWalkerQueryRecord *walkers, const int16_t *building_id_grid,
    const int16_t *building_types, uint32_t walker_type,
    int32_t building_id);
int32_t pharaoh_direction_is_same_or_adjacent(int32_t first, int32_t second);
int32_t pharaoh_walker_find_linked_type_77(
    const PharaohWalkerQueryRecord *walkers, const int16_t *heads,
    int32_t head_index);
int32_t pharaoh_walker_find_linked_type_78(
    const PharaohWalkerQueryRecord *walkers, const int16_t *heads,
    int32_t head_index);
uint8_t pharaoh_walker_has_any_value_bc(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
void pharaoh_walker_detach_slot_link(
    PharaohWalkerQueryRecord *walkers, int16_t *slot_walkers,
    int32_t walker_id);
void pharaoh_enemy_transport_wait(
    PharaohWalkerQueryRecord *walkers, int16_t *slot_walkers,
    int32_t walker_id, int32_t *moving, int16_t river_exit_x,
    int16_t river_exit_y,
    int32_t (*water_farthest_on_line)(void *, int32_t, int32_t,
        int32_t, int32_t, int32_t *, int32_t *), void *user_data);
void pharaoh_enemy_warship_wait(
    PharaohWalkerQueryRecord *walkers, int16_t *slot_walkers,
    int32_t walker_id, int16_t river_entry_x, int16_t river_entry_y);
typedef void (*PharaohShipMove)(void *user_data, int32_t walker_id,
    int32_t move_mode);
void pharaoh_enemy_warship_move(
    PharaohWalkerQueryRecord *walkers, int16_t *slot_walkers,
    int32_t walker_id, int32_t *moving,
    PharaohShipMove ship_move, void *user_data);
void pharaoh_enemy_warship_leave(
    PharaohWalkerQueryRecord *walkers, int16_t *slot_walkers,
    int32_t walker_id, int32_t current_walker_id, int32_t *moving,
    PharaohShipMove ship_move, void *user_data);
int32_t pharaoh_walker_formation_positive_count(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id,
    const int32_t (*formation_values)[11]);
void pharaoh_walker_wait_tick(
    PharaohWalkerQueryRecord *walkers, int32_t walker_id);
void pharaoh_protestor_action(PharaohWalkerQueryRecord *walkers,
    int32_t walker_id, const uint8_t *corpse_frame_offsets,
    int32_t corpse_image_base, int32_t protestor_image_base);
int32_t pharaoh_walker_find_chain_type_55(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id,
    int32_t *depth);
int32_t pharaoh_walker_chain_count(
    const PharaohWalkerQueryRecord *walkers, int32_t first_walker_id);
int32_t pharaoh_walker_nth_active_player_type_78(
    const PharaohWalkerQueryRecord *walkers, int32_t ordinal,
    uint8_t current_player);
void pharaoh_walker_count_active_transport_types(
    const PharaohWalkerQueryRecord *walkers, int32_t *count);
void pharaoh_walker_recount_player_naval_types(
    const PharaohWalkerQueryRecord *walkers, uint32_t current_player,
    uint8_t *type_77_count, uint8_t *type_78_count,
    uint8_t *flagged_type_78_count);
void pharaoh_walker_save_type_78_sources(
    PharaohWalkerQueryRecord *walkers);
uint8_t pharaoh_animal_is_amphibious_climate(
    uint8_t is_prey, uint8_t climate, int8_t predator_kind,
    const uint8_t *prey_table, const uint8_t *predator_table);
int32_t pharaoh_minimap_walker_ship_kind(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
uint8_t pharaoh_pyramid_linked_worker_active(
    const PharaohWalkerQueryRecord *walkers,
    int32_t walker_id, int32_t owner_id);
void pharaoh_walker_route_clean(
    int16_t route_walker_ids[1000], const PharaohWalkerQueryRecord *walkers);
void pharaoh_walker_sink_all_ships(
    PharaohWalkerQueryRecord *walkers, PharaohBuildingTypeRecord *buildings);
int32_t pharaoh_delivery_boy_find_buyer(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
int32_t pharaoh_warship_find_enemy_at(
    const PharaohWalkerQueryRecord *walkers,
    const PharaohWalkerTypeProperties *properties, int32_t current_walker_id,
    int32_t x, int32_t y, int32_t range,
    int32_t *result_x, int32_t *result_y, int32_t *best_distance);
int32_t pharaoh_warship_find_enemy(
    const PharaohWalkerQueryRecord *walkers,
    const PharaohWalkerTypeProperties *properties, int32_t walker_id,
    int32_t range, int32_t *result_x, int32_t *result_y,
    int32_t *best_distance);
int32_t pharaoh_cart_resource_sprite_group(int32_t resource_kind);
void pharaoh_ferry_paths_clear_all(
    PharaohFerryPathRecord paths[PHARAOH_FERRY_QUEUE_COUNT]);
int32_t pharaoh_ferry_path_allocate(
    PharaohFerryPathRecord paths[PHARAOH_FERRY_QUEUE_COUNT]);
void pharaoh_ferry_queues_clear_all(
    PharaohFerryQueueRecord queues[PHARAOH_FERRY_QUEUE_COUNT]);
void pharaoh_enemy_ship_sink(
    PharaohWalkerQueryRecord *walkers, int32_t walker_id);
int32_t pharaoh_walker_train_leader(
    const PharaohWalkerQueryRecord *walkers, int32_t walker_id);
void pharaoh_buyer_return_home(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int16_t *slot_walkers, int32_t walker_id);
void pharaoh_walker_init_scenario(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t *highest_id, int32_t *created_sequence);
void pharaoh_formation_clear_all(
    PharaohFormationTotalsRecord formations[50], int32_t *last_id_in_use,
    int32_t *reserved_count, int32_t *legion_count);
void pharaoh_walker_combat_resume_activity_after_attack(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int16_t *slot_walkers, int32_t walker_id);
void pharaoh_laborer_arrive_at_farm(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    PharaohBuildingTypeRecord *buildings,
    const PharaohBuildingModelRecord *models,
    int32_t city_locusts_countdown, int32_t walker_id);
void pharaoh_walker_name_set(
    PharaohWalkerQueryRecord *walkers,
    const PharaohFormationTotalsRecord formations[50],
    PharaohWalkerNameCounters *counters, int32_t walker_id);

void pharaoh_walker_name_set(
    PharaohWalkerQueryRecord *walkers,
    const PharaohFormationTotalsRecord formations[50],
    PharaohWalkerNameCounters *counters, int32_t walker_id);
void pharaoh_walker_init_scenario(
    PharaohWalkerQueryRecord walkers[PHARAOH_WALKER_RECORD_COUNT],
    int32_t *highest_id, int32_t *created_sequence);
#endif
