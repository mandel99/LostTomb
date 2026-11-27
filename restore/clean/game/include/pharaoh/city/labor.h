#ifndef PHARAOH_CITY_LABOR_H
#define PHARAOH_CITY_LABOR_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/simulation/industry_queries.h"

typedef struct PharaohLaborCategory {
    int32_t demand,allocated,covered,building_count,priority;
} PharaohLaborCategory;
typedef struct PharaohLaborTypeCategory {
    int32_t category;
    uint8_t opaque_04[16];
} PharaohLaborTypeCategory;
typedef struct PharaohLaborBuildingModel {
    int32_t workers;
    uint8_t opaque_04[36];
} PharaohLaborBuildingModel;
typedef struct PharaohLaborWeight {
    int32_t category,weight;
} PharaohLaborWeight;

typedef struct PharaohCityLaborContext {
    /* Ten20-byte rows atEA439C + actual player*49D8. Category9 participates
       in total demand and the sufficient-workers path; priority/weight and
       shortage sums use categories0..8, as in the original. */
    PharaohLaborCategory *categories;
    const int32_t *available; /* EA4398 */
    int32_t *employed,*free_workers,*unemployment_rate,*unfilled;
    /* EA4464/EA4468/EA446C/EA4474, all for the same actual player. */
    const PharaohLaborWeight *weights; /* nine pairs5DA44C, stride8 */
    const PharaohLaborTypeCategory *type_categories; /* 5D06EC, stride20 */
    const PharaohLaborBuildingModel *models; /* 705EB8, stride40 */
    PharaohBuildingTypeRecord *buildings;
    const int32_t *highest_building_id; /* actual78D790 */
    const int16_t *resource_flags; /* EA4126 + player*49D8 */
    /* Existing real488050 C owner and its genuine predicate bindings. */
    const PharaohIndustryQueryContext *industry;
} PharaohCityLaborContext;

/* Complete00459100/00459400/004595A0. Borrow actual live fields and tables.
   The allocator reports the existing percentage helper's native IDIV fault
   status; on that fault the final rate store is not reached. This status is
   a C fault channel, not a claim about residual native EAX. */
void pharaoh_city_labor_rebuild_demand(const PharaohCityLaborContext *);
PharaohPercentageScaleStatus pharaoh_city_labor_allocate_categories(
    const PharaohCityLaborContext *);
void pharaoh_city_labor_rebuild_building_shares(const PharaohCityLaborContext *);

#endif
