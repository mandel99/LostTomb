#include "pharaoh/simulation/industry_queries.h"

enum {
    MONTHS_PER_YEAR = 12,
    HARVESTS_PER_CROP = 4,
    RESOURCE_LIMESTONE = 25,
    RESOURCE_GRANITE = 26,
    GROUP_LIMESTONE_QUARRY = 119,
    GROUP_GRANITE_QUARRY = 192,
    GROUP_SANDSTONE_QUARRY = 194
};

int32_t pharaoh_farm_next_harvest_month(
    const PharaohIndustryQueryContext *c, int32_t building)
{
    int32_t crop, nearest = MONTHS_PER_YEAR, month = -1, i;
    crop = c->is_farm(c->user_data, building) ?
        c->crop_for_type(c->user_data,
            c->building_type(c->user_data, building)) : -1;
    if (c->is_on_floodplain(c->user_data, building)) return c->flood_season_month;
    for (i = 0; i < HARVESTS_PER_CROP; ++i) {
        int32_t harvest = c->farm_harvest_months[crop * HARVESTS_PER_CROP + i];
        int32_t ahead;
        if (harvest == -1) break;
        ahead = harvest - c->current_month;
        if (ahead <= 0) ahead += MONTHS_PER_YEAR;
        if (ahead < nearest) { nearest = ahead; month = harvest; }
    }
    return month;
}

int32_t pharaoh_building_is_floodplain_farm(
    const PharaohIndustryQueryContext *c, int32_t building)
{
    return c->is_farm(c->user_data, building) &&
        c->is_on_floodplain(c->user_data, building);
}

void pharaoh_draw_quarry(const PharaohIndustryQueryContext *c, int32_t resource)
{
    int32_t group = resource == RESOURCE_LIMESTONE ? GROUP_LIMESTONE_QUARRY :
        (resource == RESOURCE_GRANITE ? GROUP_GRANITE_QUARRY :
            GROUP_SANDSTONE_QUARRY);
    c->draw_raw_producer(c->user_data, c->quarry_help, resource, group, 0);
}
