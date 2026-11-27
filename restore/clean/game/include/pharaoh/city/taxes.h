#ifndef PHARAOH_CITY_TAXES_H
#define PHARAOH_CITY_TAXES_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/core/percentage_scale.h"

typedef struct PharaohHouseTaxMetric {
    uint32_t metric;
    uint8_t opaque_04[84];
} PharaohHouseTaxMetric;
typedef struct PharaohTaxWeightedBand {
    uint32_t collected,uncollected;
} PharaohTaxWeightedBand;

typedef struct PharaohCityTaxContext {
    PharaohBuildingTypeRecord *buildings;
    const int32_t *highest_building_id; /* 78D790 */
    const PharaohHouseTaxMetric *metrics; /* strided field70CE18,88 bytes */
    const int32_t *tax_rate; /* EA1878 */
    const uint32_t *difficulty,*month; /* E39080/78D7C0, full DWORDs */
    const int32_t *difficulty_scales; /* 5D1974 */
    /* All city fields below belong to the actual current player; native
       per-player stride49D8. Percentage children do not change that player. */
    uint32_t *population_collected,*population_uncollected; /* 2 each,EA4490/98 */
    PharaohTaxWeightedBand *weighted; /* 2 rows atEA44C4 */
    uint32_t *tier_population; /* 20 DWORDs atEA3EFC */
    uint32_t *year_collected,*year_uncollected; /* 2 each,EA44AC/B4 */
    uint32_t *coverage; /* 3 DWORDs atEA44A0 */
    uint32_t *treasury,*taxes_this_year,*forecast; /* EA187C/EA44BC/EA5E5C */
} PharaohCityTaxContext;

/* Complete004599D0 and00459D20. Inputs bind actual shared native fields.
   Collection reports native percentage IDIV overflow via the existing C
   status, retaining prior effects and skipping the faulted coverage store. */
PharaohPercentageScaleStatus pharaoh_city_taxes_collect_month(
    const PharaohCityTaxContext *);
void pharaoh_city_taxes_estimate(const PharaohCityTaxContext *);

#endif
