#ifndef PHARAOH_MONUMENTS_STAGES_H
#define PHARAOH_MONUMENTS_STAGES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "pharaoh/building/type_queries.h"

typedef struct PharaohMonumentBuilding {
    int32_t x;
    int32_t y;
    int16_t type;
    int16_t subtype;
    int16_t road_access_x;
    int16_t road_access_y;
    uint16_t created_sequence;
    int32_t stage;
    int32_t city_slot;
    int32_t work_flags;
    uint8_t is_main_part;
    uint8_t completion_shown;
    uint8_t built_parts[16];
    int32_t tomb_stage;
    int32_t tomb_remaining_work[4];
    int32_t tomb_worker_id;
    int32_t tomb_chamber_kind;
    int32_t tomb_neighbour_offsets[6];
    uint8_t tomb_can_take_workers;
    uint8_t tomb_completed;
    int32_t tomb_stock;
} PharaohMonumentBuilding;

typedef struct PharaohMonumentWalker {
    int32_t type;
    int32_t action_state;
    int32_t wait_ticks;
    uint8_t is_ghost;
} PharaohMonumentWalker;

typedef struct PharaohTombWorkSpot {
    int32_t stage;
    int32_t walker_type;
    int32_t walker_id;
} PharaohTombWorkSpot;

typedef void (*PharaohMonumentImageFunction)(
    void *user_data, void *object, int32_t building_id);
typedef int32_t (*PharaohMonumentMainIdFunction)(void *user_data, int32_t building_id);
typedef void (*PharaohMonumentSpotTickFunction)(
    void *user_data, int32_t building_id, int32_t walker_id, uint8_t done_action);
typedef void (*PharaohMonumentWorkTileFunction)(void *user_data, void *object,
    int32_t building_id, int32_t *x, int32_t *y);
typedef bool (*PharaohTombSpotWorkFunction)(void *user_data,
    const PharaohTombWorkSpot *spot, const PharaohMonumentBuilding *building,
    int32_t stage);
typedef int32_t (*PharaohTombTableFunction)(void *user_data, int32_t chamber_kind);
typedef void (*PharaohSpotAnimationFunction)(
    void *user_data, PharaohTombWorkSpot *spot);
typedef bool (*PharaohVictoryFunction)(void *user_data);
typedef int32_t (*PharaohTombProgressFunction)(
    void *user_data, int32_t building_id, int16_t building_type);
typedef void (*PharaohTombFinalImageFunction)(
    void *user_data, int32_t building_id, int16_t building_type);
typedef int32_t (*PharaohObjectCountFunction)(void *user_data, void *object);

typedef struct PharaohMonumentStageContext {
    PharaohMonumentBuilding *buildings;
    PharaohMonumentWalker *walkers;
    const int32_t *caesareum_part_kinds;
    const int32_t *lighthouse_part_kinds;
    const uint8_t *abu_simbel_stage_table;
    PharaohTombWorkSpot *tomb_work_spots;
    void *small_tomb_object;
    void *medium_tomb_object;
    void *large_tomb_object;
    void *grand_tomb_object;
    PharaohMonumentImageFunction update_image;
    PharaohMonumentMainIdFunction main_part_id;
    PharaohMonumentSpotTickFunction caesareum_spot_tick;
    PharaohMonumentSpotTickFunction library_spot_tick;
    PharaohMonumentSpotTickFunction lighthouse_spot_tick;
    PharaohMonumentWorkTileFunction get_work_tile;
    PharaohTombSpotWorkFunction tomb_spot_has_work;
    PharaohTombTableFunction tomb_spot_table;
    PharaohSpotAnimationFunction start_spot_animation;
    PharaohVictoryFunction victory_goals_reached;
    PharaohTombProgressFunction tomb_progress;
    PharaohTombFinalImageFunction update_tomb_final_image;
    PharaohObjectCountFunction object_part_count;
    PharaohObjectCountFunction object_chamber_count;
    void *user_data;
} PharaohMonumentStageContext;

void pharaoh_caesareum_walker_work_tick(PharaohMonumentStageContext *context,
    void *object, int32_t building_id, int32_t walker_id, uint8_t done_action);
void pharaoh_library_walker_work_tick(PharaohMonumentStageContext *context,
    void *object, int32_t building_id, int32_t walker_id, uint8_t done_action);
void pharaoh_lighthouse_walker_work_tick(PharaohMonumentStageContext *context,
    void *object, int32_t building_id, int32_t walker_id, uint8_t done_action);
void pharaoh_library_set_completed(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_lighthouse_complete_instant(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_library_advance_stage(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_abu_simbel_advance_stage(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_caesareum_advance_stage(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_lighthouse_advance_stage(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_royal_tomb_part_check_stage_done(
    PharaohMonumentStageContext *context, int32_t building_id);
void pharaoh_royal_tomb_walker_work_tick(PharaohMonumentStageContext *context,
    void *object, int32_t building_id, int32_t walker_id, uint8_t next_action);
void pharaoh_royal_tomb_part_advance_stage(PharaohMonumentStageContext *context,
    void *object, int32_t building_id, int32_t part_count);
void pharaoh_royal_tomb_set_completed(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_royal_tomb_update_part_images(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_small_royal_tomb_part_advance(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_medium_royal_tomb_part_advance(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_large_royal_tomb_part_advance(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);
void pharaoh_grand_royal_tomb_part_advance(PharaohMonumentStageContext *context,
    void *object, int32_t building_id);

typedef int32_t (*PharaohRoyalTombChamberCount)(void *object);
typedef uint8_t (*PharaohRoyalTombHasWorker)(void *object,int32_t building);
int32_t pharaoh_royal_tomb_is_idle(void *object,
    const PharaohBuildingTypeRecord buildings[PHARAOH_BUILDING_RECORD_COUNT],
    int32_t building,PharaohRoyalTombChamberCount chambers,PharaohRoyalTombHasWorker has_worker);
uint8_t pharaoh_monument_resources_complete(
    const int32_t *stored_amounts,const int32_t *required_amounts);
uint8_t pharaoh_monument_resources_complete_thunk(
    const int32_t *stored_amounts,const int32_t *required_amounts);

void pharaoh_city_set_monument_building_id(
    int32_t *monument_building_ids, int32_t building_id, int32_t slot);
int32_t pharaoh_city_find_monument_slot(
    const int32_t monument_building_ids[3], int32_t building_id);
#endif
