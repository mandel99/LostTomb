#include "pharaoh/commerce/granary_supply.h"

#include <stddef.h>
#include <string.h>

_Static_assert(sizeof(PharaohBuildingTypeRecord) == 0x108 &&
    offsetof(PharaohBuildingTypeRecord, has_road_access) == 0x44 &&
    offsetof(PharaohBuildingTypeRecord, granary_resource_stored) == 0x56 &&
    offsetof(PharaohBuildingTypeRecord, storage_id) == 0xc1 &&
    sizeof(PharaohGranarySupplyCandidates) == 200 &&
    offsetof(PharaohBuildingStorageRecord, resource_state) == 0x0a &&
    offsetof(PharaohBuildingStorageRecord, accept_amount) == 0x34 &&
    offsetof(PharaohBuildingStorageRecord, get_amount) == 0x7c &&
    offsetof(PharaohGranaryDispatchBuilding, building_type) == 0x10,
    "native granary record fields");

/* Complete prior Granary.c algorithm, native0045E040; no native callees. */
void pharaoh_granary_supply_calculate(const PharaohGranarySupplyContext *c)
{
    memset(c->candidates, 0, sizeof(*c->candidates));
    memset(c->resource_totals, 0, 36 * sizeof(*c->resource_totals));
    *c->candidate_count = 0;
    uint32_t output = 0;
    for (int32_t id = 1; id <= *c->highest_building; ++id) {
        const PharaohBuildingTypeRecord *b = &c->buildings[id];
        if (b->state != 1 || (b->building_type != 0x47 && b->building_type != 0xbf) ||
            b->has_road_access == 0 || b->distance_from_entry <= 0) continue;
        const PharaohBuildingStorageRecord *s = &c->storages[b->storage_id];
        uint32_t total = 0;
        for (int32_t resource = 1; resource < 9; ++resource) {
            if (s->resource_state[resource] == 2) continue;
            const uint32_t amount = (uint32_t)(int32_t)b->granary_resource_stored[resource];
            total += amount;
            c->resource_totals[resource] += amount;
        }
        if ((int32_t)total > 100) {
            c->candidates->building_ids[output] = (uint16_t)id;
            /* Possible native bug: slot98 keeps being overwritten but never
             * contributes to candidate_count. Original behavior retained. */
            if (output < 98) { ++*c->candidate_count; ++output; }
        }
    }
}
