#include "pharaoh/military/ship_status.h"
static int32_t percentage(int32_t value, int32_t maximum)
{ return maximum == 0 ? 0 : value * 100 / maximum; }
int32_t pharaoh_ship_damage_text(const PharaohShipStatus *ship,
    const PharaohShipStatusRules *rules)
{
    int32_t percent = percentage(ship->hit_points, ship->maximum_hit_points);
    int32_t rule;
    for (rule = 0; rule < 4; ++rule)
        if (percent <= rules->hull_thresholds[rule]) return rules->weak_text - rule;
    return rules->very_strong_text;
}
int32_t pharaoh_ship_crew_text(const PharaohShipStatus *ship,
    const PharaohShipStatusRules *rules)
{
    if (ship->movement_mode == 0) return rules->rested_text;
    return ship->movement_mode == 1 ? rules->tired_text : rules->exhausted_text;
}
