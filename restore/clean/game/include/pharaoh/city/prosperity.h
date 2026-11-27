#ifndef PHARAOH_CITY_PROSPERITY_H
#define PHARAOH_CITY_PROSPERITY_H

#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/core/percentage_scale.h"

typedef struct PharaohTempleWageBonusContext {
    const PharaohBuildingTypeRecord *buildings;
    const uint16_t *temple_count; /* EA5CE8 + player*49D8 */
    const int32_t *temple_building; /* EA5F9C + player*49D8 */
} PharaohTempleWageBonusContext;
typedef struct PharaohCompletedMonumentContext {
    PharaohMapSurfaceContext *surface;
    const int32_t *pyramid_anchor_offsets; /* actual5F5D08 table */
    const int16_t *sphinx_stage; /* EA5BAA + player*49D8 */
} PharaohCompletedMonumentContext;
typedef struct PharaohProsperityExplanationContext {
    /* Actual DWORD cells in native sum order; meanings remain qualified by
       their addresses rather than inferred from stale decompiler field names. */
    const int32_t *income[4]; /* EA601C,EA5CC4,EA44C0,EA44D8 */
    const int32_t *expense[5]; /* EA44F8,EA4500,EA44E8,EA448C,EA44E0 */
    const int32_t *value_ea44fc,*value_ea41e4;
    const int32_t *unfilled,*unemployment; /* EA4474/EA446C */
    const int32_t *value_ea5e18,*value_ea447c;
    const int32_t *value_ea3f78,*value_ea3f7c,*population; /* EA1894 */
    const int32_t *value_ea5d58,*value_ea5be8;
    const int32_t *prosperity,*cap; /* EA5B40/EA5F18 */
    int32_t *trend,*explanation; /* EA5B54/EA5CA8 */
    const int32_t *current_year; /* 78D7BC */
    const int16_t *scenario_start_year; /* 7848D8 */
    const PharaohTempleWageBonusContext *temple;
    const PharaohCompletedMonumentContext *monuments;
} PharaohProsperityExplanationContext;

/* Complete004C38F0 and00451B10. Monument result is logical AL only; no
   residual upper EAX/flags are exposed. Surface mode3 uses real map logic
   and the actual pyramid-placement scratch, without rendering callbacks. */
int32_t pharaoh_city_temple_complex_wage_bonus(
    const PharaohTempleWageBonusContext *);
uint8_t pharaoh_city_has_completed_monument(
    const PharaohCompletedMonumentContext *);
/* Complete0046E410. All player pointers bind the same actual current player.
   Real children leave that selector unchanged. Percentage faults propagate
   before monument checks/output stores, as the original exception does. */
PharaohPercentageScaleStatus pharaoh_city_ratings_prosperity_explanation(
    const PharaohProsperityExplanationContext *);

#endif
