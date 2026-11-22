#include "pharaoh/population/health_coverage.h"
#include "pharaoh/core/percentage_scale.h"

_Static_assert(sizeof(PharaohHealthCoverageBuilding)==0x108 &&
    offsetof(PharaohHealthCoverageBuilding,population)==0x1c &&
    offsetof(PharaohHealthCoverageBuilding,coverage_6e)==0x6e &&
    offsetof(PharaohHealthCoverageBuilding,coverage_75)==0x75 &&
    offsetof(PharaohHealthCoverageBuilding,coverage_76)==0x76 &&
    offsetof(PharaohHealthCoverageBuilding,coverage_78)==0x78,
    "native health coverage building layout");

/* Unchanged complete C owner004E7E60, moved from culture_metrics.c. */
void pharaoh_city_culture_update_health_coverage(
    const PharaohHealthCoverageBuilding *buildings, size_t building_count,
    int32_t *coverage_76, int32_t *coverage_78,
    int32_t *coverage_6e, int32_t *coverage_75)
{
    int32_t population = 0;
    int32_t covered_76 = 0;
    int32_t covered_78 = 0;
    int32_t covered_6e = 0;
    int32_t covered_75 = 0;
    size_t building_id;

    for (building_id = 1; building_id < building_count; ++building_id) {
        const PharaohHealthCoverageBuilding *building = &buildings[building_id];
        int32_t house_population;
        if (building->state != 1u || building->house_size == 0u ||
            building->population == 0) {
            continue;
        }
        house_population = building->population;
        population += house_population;
        if (building->coverage_76 != 0u) covered_76 += house_population;
        if (building->coverage_78 != 0u) covered_78 += house_population;
        if (building->coverage_75 != 0u) covered_75 += house_population;
        if (building->coverage_6e != 0u) covered_6e += house_population;
    }
    *coverage_76 = pharaoh_scale_value_to_percent(covered_76, population).value;
    *coverage_78 = pharaoh_scale_value_to_percent(covered_78, population).value;
    *coverage_75 = pharaoh_scale_value_to_percent(covered_75, population).value;
    *coverage_6e = pharaoh_scale_value_to_percent(covered_6e, population).value;
}
