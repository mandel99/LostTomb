#include "pharaoh/map/service_visit.h"

static uint32_t service_shift(int32_t service)
{
    return ((uint32_t)service * PHARAOH_SERVICE_VISIT_BITS) & 31u;
}

/* Complete C translation of Pharaoh.exe 0x0047E230. */
int32_t pharaoh_map_service_visit_get(
    const uint32_t *grid, int32_t service, int32_t grid_offset)
{
    return (int32_t)((grid[grid_offset] >> service_shift(service)) &
        PHARAOH_SERVICE_VISIT_MAX);
}

/* Complete C translation of Pharaoh.exe 0x0047E250. */
void pharaoh_map_service_visit_increment(
    uint32_t *grid, int32_t service, int32_t grid_offset)
{
    const uint32_t shift = service_shift(service);
    const uint32_t previous = grid[grid_offset];
    const uint32_t value =
        (previous >> shift) & PHARAOH_SERVICE_VISIT_MAX;
    if (value < PHARAOH_SERVICE_VISIT_MAX) {
        const uint32_t native_clear_mask =
            (UINT32_C(7) << shift) ^ UINT32_C(0x0000ffff);
        grid[grid_offset] = (previous & native_clear_mask) |
            ((value + 1u) << shift);
    }
}

/* Complete C translation of Pharaoh.exe 0x0047E290. */
void pharaoh_map_service_visit_decay(PharaohServiceVisitDecayState *state)
{
    const uint32_t next = (uint32_t)*state->counter + 1u;
    *state->counter = (int32_t)next;
    if ((int32_t)next < PHARAOH_SERVICE_VISIT_DECAY_DAYS) return;

    *state->counter = 0;
    for (int32_t row = 0; row < state->row_count; ++row) {
        const int32_t count = state->row_end[row] - state->row_start[row];
        if (count <= 0) continue;
        uint32_t *cell = state->grid + state->row_offsets[row];
        for (int32_t x = 0; x < count; ++x, ++cell) {
            uint32_t packed = *cell;
            if (packed != 0u) {
                for (uint32_t channel = 0;
                     channel < PHARAOH_SERVICE_VISIT_CHANNELS; ++channel) {
                    const uint32_t shift = channel * PHARAOH_SERVICE_VISIT_BITS;
                    const uint32_t value =
                        (packed >> shift) & PHARAOH_SERVICE_VISIT_MAX;
                    if (value != 0u) {
                        const uint32_t native_clear_mask =
                            (UINT32_C(7) << shift) ^
                            UINT32_C(0x0000ffff);
                        packed = (packed & native_clear_mask) |
                            ((value - 1u) << shift);
                    }
                }
            }
            *cell = packed;
        }
    }
}
