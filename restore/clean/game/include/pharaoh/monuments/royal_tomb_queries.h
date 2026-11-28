#ifndef PHARAOH_MONUMENTS_ROYAL_TOMB_QUERIES_H
#define PHARAOH_MONUMENTS_ROYAL_TOMB_QUERIES_H

#include <stdint.h>
#include <stdbool.h>

#include "pharaoh/building/type_queries.h"

/* Native 32-bit prefix shared by all scripted monument objects. */
typedef struct PharaohMonumentAnimationState {
    uint32_t vtable_token;
    int32_t frame_count;
    int32_t frame;
} PharaohMonumentAnimationState;

bool pharaoh_monument_animation_tick(PharaohMonumentAnimationState *state);
int32_t pharaoh_monument_no_progress(void *self, int32_t building_id,
    int32_t *output);

int32_t pharaoh_small_royal_tomb_chamber_count(void);
int32_t pharaoh_grand_royal_tomb_chamber_count(void);
int32_t pharaoh_large_royal_tomb_chamber_count(void);
int32_t pharaoh_medium_royal_tomb_chamber_count(void);
uint32_t pharaoh_small_royal_tomb_get_access_tile(
    PharaohBuildingTypeRecord *buildings, int32_t building_id,
    int32_t *out_x, int32_t *out_y);
uint32_t pharaoh_grand_royal_tomb_get_access_tile(
    PharaohBuildingTypeRecord *buildings, int32_t building_id,
    int32_t *out_x, int32_t *out_y);
uint32_t pharaoh_large_royal_tomb_get_access_tile(
    PharaohBuildingTypeRecord *buildings, int32_t building_id,
    int32_t *out_x, int32_t *out_y);
uint32_t pharaoh_medium_royal_tomb_get_access_tile(
    PharaohBuildingTypeRecord *buildings, int32_t building_id,
    int32_t *out_x, int32_t *out_y);

#endif
