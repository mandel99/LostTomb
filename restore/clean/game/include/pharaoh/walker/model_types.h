#ifndef PHARAOH_WALKER_MODEL_TYPES_H
#define PHARAOH_WALKER_MODEL_TYPES_H

#include <stddef.h>
#include <stdint.h>

enum {
    PHARAOH_FIGURE_MODEL_RECORD_COUNT = 110,
    PHARAOH_ENEMY_MODEL_NATION_COUNT = 12,
    PHARAOH_ENEMY_MODEL_TYPE_COUNT = 5,
    PHARAOH_FIGURE_MODEL_FIELD_COUNT = 14
};

typedef struct PharaohFigureModelRecord {
    int32_t category;
    int32_t max_damage;
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
    int32_t column_m;
    int32_t column_n;
} PharaohFigureModelRecord;

typedef PharaohFigureModelRecord PharaohFigureModelTable[
    PHARAOH_FIGURE_MODEL_RECORD_COUNT];
typedef PharaohFigureModelRecord PharaohEnemyModelTable[
    PHARAOH_ENEMY_MODEL_NATION_COUNT][PHARAOH_ENEMY_MODEL_TYPE_COUNT];

_Static_assert(sizeof(PharaohFigureModelRecord) == 0x38,
    "native figure model rows contain fourteen 32-bit values");
_Static_assert(offsetof(PharaohFigureModelRecord, missile_defense) == 0x10 &&
    offsetof(PharaohFigureModelRecord, speed) == 0x20 &&
    offsetof(PharaohFigureModelRecord, fast_run_tile_limit) == 0x28,
    "figure model members preserve the native row layout");

#endif
