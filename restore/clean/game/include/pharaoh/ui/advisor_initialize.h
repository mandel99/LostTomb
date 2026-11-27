#ifndef PHARAOH_UI_ADVISOR_INITIALIZE_H
#define PHARAOH_UI_ADVISOR_INITIALIZE_H
#include "pharaoh/city/labor.h"
#include "pharaoh/city/taxes.h"
#include "pharaoh/city/advisor_finance.h"
#include "pharaoh/city/advisor_reasons.h"
#include "pharaoh/city/food_supply.h"
#include "pharaoh/city/prosperity.h"
#include "pharaoh/military/formation_counts.h"
#include "pharaoh/scenario/scene_ui.h"
#include "pharaoh/simulation/culture_ratings.h"

typedef struct PharaohAdvisorInitializeContext {
    const PharaohCityLaborContext *labor;
    PharaohFerryContext *ferry;
    const PharaohCityTaxContext *taxes;
    const PharaohAdvisorFinanceContext *finance;
    PharaohAdvisorReasonState *reasons;
    PharaohSceneUi *coverage;
    const PharaohCityFoodUpdateContext *food;
    const PharaohFormationCountContext *formations;
    const PharaohWalkerQueryRecord *walkers;
    const uint32_t *current_player; /* actual EAAC28, full DWORD */
    uint8_t *naval_77,*naval_78,*naval_flagged_78; /* EA5ED4/D8/D9 */
    const PharaohCultureRatingContext *culture;
    const PharaohProsperityExplanationContext *prosperity;
    const PharaohKingdomExplanationContext *kingdom;
    const PharaohBuildingTypeRecord *buildings;
    const int32_t *win_house_threshold; /*784C38*/
    int32_t *win_house_count; /*EADB28*/
} PharaohAdvisorInitializeContext;

/* Complete0040A160. All children borrow the actual shared pools, scratch and
   same selected city. A real percentage divide fault stops the call sequence.
   The C status is diagnostic; the checked native caller ignores the final EAX.
   Live host bindings and unobserved/indirect consumers remain unqualified. */
PharaohPercentageScaleStatus pharaoh_advisors_initialize(
    const PharaohAdvisorInitializeContext *);
#endif
