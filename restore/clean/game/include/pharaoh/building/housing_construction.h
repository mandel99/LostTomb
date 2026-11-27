#ifndef PHARAOH_BUILDING_HOUSING_CONSTRUCTION_H
#define PHARAOH_BUILDING_HOUSING_CONSTRUCTION_H

#include "pharaoh/building/allocation.h"
#include "pharaoh/building/construction_basic_warnings.h"
#include "pharaoh/building/housing_warnings.h"
#include "pharaoh/scenario/scene_routing.h"

typedef struct PharaohHousingConstructionContext {
    PharaohBuildingAllocationContext *allocation;
    const PharaohConstructionBasicWarnings *food_warnings;
    const PharaohHousingWarningContext *housing_warnings;
    PharaohSceneRouting *routing;
    const uint32_t *house_image; /* Actual F5653C first-image field. */
    uint32_t *placed_count; /* BC971C, shared with all builders. */
    uint8_t *map_changed; /* E38DCA. */
} PharaohHousingConstructionContext;

/* Complete native00474130. Normal native result is void; checked C status
 * propagates the genuine food-query IDIV fault before later side effects.
 * Surface scan_x/scan_y/current_tile share BC9764/60/68 with all children;
 * allocator, undo, warnings and routing must borrow the same live world. */
PharaohPercentageScaleStatus pharaoh_construction_build_housing_rectangle(
    const PharaohHousingConstructionContext *, uint32_t preview,
    int32_t x1, int32_t y1, int32_t x2, int32_t y2);

#endif
