#ifndef PHARAOH_MAP_SERVICE_VISIT_H
#define PHARAOH_MAP_SERVICE_VISIT_H

#include <stdint.h>

enum {
    PHARAOH_SERVICE_VISIT_CHANNELS = 10,
    PHARAOH_SERVICE_VISIT_BITS = 3,
    PHARAOH_SERVICE_VISIT_MAX = 7,
    PHARAOH_SERVICE_VISIT_DECAY_DAYS = 20
};

typedef struct PharaohServiceVisitDecayState {
    uint32_t *grid;
    int32_t *counter;
    int32_t row_count;
    const int32_t *row_offsets;
    const int32_t *row_start;
    const int32_t *row_end;
} PharaohServiceVisitDecayState;

int32_t pharaoh_map_service_visit_get(
    const uint32_t *grid, int32_t service, int32_t grid_offset);
void pharaoh_map_service_visit_increment(
    uint32_t *grid, int32_t service, int32_t grid_offset);
void pharaoh_map_service_visit_decay(PharaohServiceVisitDecayState *state);

#endif
