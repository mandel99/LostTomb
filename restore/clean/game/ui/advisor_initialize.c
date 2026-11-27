#include "pharaoh/ui/advisor_initialize.h"
#include "pharaoh/runtime/platform_measure_speed.h"
#include "pharaoh/walker/record_queries.h"

PharaohPercentageScaleStatus pharaoh_advisors_initialize(
    const PharaohAdvisorInitializeContext *c)
{
    PharaohPercentageScaleStatus status=
        pharaoh_city_labor_allocate_categories(c->labor);
    if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return status;
    pharaoh_city_labor_rebuild_building_shares(c->labor);
    /* Both native0051F780 calls really are RET instructions. */
    pharaoh_actor_walking_no_operation();
    pharaoh_actor_walking_no_operation();
    pharaoh_city_labor_assign_buildings(c->labor,c->ferry);
    pharaoh_city_taxes_estimate(c->taxes);
    pharaoh_city_finance_estimate_wages(c->finance);
    pharaoh_city_finance_estimate_interest(c->finance);
    pharaoh_city_finance_estimate_salary(c->finance);
    pharaoh_city_finance_calculate_totals(c->finance);
    pharaoh_city_finance_calculate_tribute(c->finance);
    pharaoh_city_ratings_advisor_reasons(c->reasons);
    pharaoh_city_update_service_coverage(c->coverage);
    pharaoh_city_update_food(c->food);
    pharaoh_formation_calculate_figures(c->formations);
    pharaoh_walker_recount_player_naval_types(c->walkers,*c->current_player,
        c->naval_77,c->naval_78,c->naval_flagged_78);
    pharaoh_city_ratings_update_culture_advisor(c->culture);
    status=pharaoh_city_ratings_prosperity_explanation(c->prosperity);
    if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE) return status;
    pharaoh_city_ratings_kingdom_explanation(c->kingdom);
    pharaoh_city_victory_update_win_houses(c->buildings,
        *c->win_house_threshold,c->win_house_count);
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}
