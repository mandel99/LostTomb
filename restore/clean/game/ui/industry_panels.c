#include "pharaoh/ui/industry_panels.h"

enum {
    TEXT_DESCRIPTION = 1, TEXT_PRODUCTION_IS = 2, TEXT_COMPLETE = 3,
    TEXT_HALTED = 4, TEXT_NO_WORKERS = 5, TEXT_FULL_STAFF = 6,
    TEXT_NO_RAW = 11, TEXT_STORED_RAW = 12, TEXT_STORED_FIRST = 13,
    TEXT_STORED_SECOND = 14, TEXT_STORED = 2, FARM_BLIGHTED = 11,
    FARM_LAND_IS = 12, FARM_FERTILE = 13, FARM_NEXT_HARVEST = 14,
    FARM_IRRIGATED = 0, FARM_NOT_IRRIGATED = 1, FARM_NEXT_FLOODS = 2,
    CAMP_NEEDS_WORKERS = 2, CAMP_SUPPLYING = 3, CAMP_LOOKING = 4,
    CAMP_ON_FARMS = 5, CAMP_ON_MONUMENTS = 6,
    ARTISANS_NO_WORKERS = 2, ARTISANS_FULL = 3, ARTISANS_NO_PAINT = 8,
    ARTISANS_NO_CLAY = 9, ARTISANS_PAINT = 10, ARTISANS_CLAY = 11,
    PRODUCTION_Y = 44, WORKSHOP_PRODUCTION_Y = 40, PRODUCER_STATE_Y = 70,
    WORKSHOP_STATE_Y = 86, DESCRIPTION_FROM_BOTTOM = 113,
    REED_STOCK_Y = 54, CATTLE_STOCK_Y = 100, WORKSHOP_STOCK_Y = 60,
    LOCUST_BLIGHT_DAYS = 5, CATTLE_PROGRESS_MAX = 400,
    WORKSHOP_PROGRESS_MAX = 400, RAW_PROGRESS_MAX = 200,
    FARM_CROP_FROM_BOTTOM = 158, FARM_IRRIGATION_FROM_BOTTOM = 143
};

static void emit(PharaohIndustryPanelContext *c,
    enum PharaohIndustryCommandKind kind, int32_t group, int32_t text,
    int32_t resource, int32_t value, int32_t x, int32_t y)
{
    PharaohIndustryCommand command = { kind, group, text, resource, value, x, y };
    c->emit(c->user_data, &command);
}

static int32_t percentage(int32_t value, int32_t total)
{ return total == 0 ? 0 : value * 100 / total; }

static bool halted(const PharaohIndustryPanelContext *c, int32_t resource)
{ return resource != c->no_resource && c->mothballed[resource] != 0u; }

static void producer_state(PharaohIndustryPanelContext *c, int32_t group,
    int32_t stock_resource, bool blight, int32_t y)
{
    int32_t text;
    if (c->road_access < 1) { emit(c, PHARAOH_INDUSTRY_NO_ROAD, 0, 0, 0, 0, 0, y); return; }
    if (halted(c, stock_resource)) text = TEXT_HALTED;
    else if (blight && c->building->curse_days_left >= LOCUST_BLIGHT_DAYS) text = FARM_BLIGHTED;
    else if (c->building->num_workers < 1) text = TEXT_NO_WORKERS;
    else text = c->worker_percentage_text(c->user_data, TEXT_FULL_STAFF);
    emit(c, PHARAOH_INDUSTRY_TEXT, group, text, 0, 0, 0, y);
}

static void workshop_state(PharaohIndustryPanelContext *c, int32_t group,
    int32_t halt_resource, int32_t no_workers, int32_t no_raw,
    int32_t no_raw2, int32_t full_staff)
{
    int32_t text, y = c->panel_y + WORKSHOP_STATE_Y;
    if (c->road_access < 1) { emit(c, PHARAOH_INDUSTRY_NO_ROAD, 0, 0, 0, 0, 0, y); return; }
    if (halted(c, halt_resource)) text = TEXT_HALTED;
    else if (c->building->num_workers < 1) text = no_workers;
    else if (no_raw >= 0 && c->building->loads_stored < 1) text = no_raw;
    else if (no_raw2 >= 0 && c->building->second_input_stock < 1) text = no_raw2;
    else text = c->worker_percentage_text(c->user_data, full_staff);
    emit(c, PHARAOH_INDUSTRY_TEXT, group, text, 0, 0, 0, y);
}

static void description(PharaohIndustryPanelContext *c, int32_t group)
{
    emit(c, PHARAOH_INDUSTRY_DESCRIPTION, group, TEXT_DESCRIPTION, 0, 0, 0,
        c->panel_height_blocks * c->block_height - DESCRIPTION_FROM_BOTTOM + c->panel_y);
}

static void title(PharaohIndustryPanelContext *c, int32_t group, int32_t resource)
{ emit(c, PHARAOH_INDUSTRY_TITLE_ICON, group, 0, resource, 0, 0, 0); }

static void stock(PharaohIndustryPanelContext *c, int32_t resource,
    int32_t group, int32_t text, int32_t slot, int32_t y, int32_t amount)
{ emit(c, PHARAOH_INDUSTRY_STOCK, group, text, resource, amount, slot, y); }

static void production(PharaohIndustryPanelContext *c, int32_t group,
    int32_t percent, int32_t y)
{ emit(c, PHARAOH_INDUSTRY_PRODUCTION, group, TEXT_PRODUCTION_IS, 0, percent, 0, y); }

static void draw_farm(PharaohIndustryPanelContext *c,
    const PharaohIndustryPanelSpec *s)
{
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, s->help, 0, 0);
    title(c, s->group, s->product);
    production(c, s->group, c->farm_growth_percent(c->user_data), PRODUCTION_Y);
    emit(c, PHARAOH_INDUSTRY_LINE, c->farm_group, FARM_LAND_IS, 0,
        0, 0, PRODUCTION_Y);
    emit(c, PHARAOH_INDUSTRY_NUMBER, 0, 0, 0,
        c->farm_fertility(c->user_data), 0, PRODUCTION_Y);
    emit(c, PHARAOH_INDUSTRY_LINE, c->farm_group, FARM_FERTILE, 0,
        0, 0, PRODUCTION_Y);
    producer_state(c, s->group, s->product, true, c->panel_y + PRODUCER_STATE_Y);
    emit(c, PHARAOH_INDUSTRY_INNER_PANEL, 0, 0, 0, 0, 0, 0);
    emit(c, PHARAOH_INDUSTRY_FARM_WORKERS, 0, 0, 0, 0, 0, 0);
    pharaoh_industry_draw_farm_crop(c, s->group);
    description(c, s->group);
}

static void draw_raw(PharaohIndustryPanelContext *c,
    const PharaohIndustryPanelSpec *s)
{
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, s->help, 0, 0);
    title(c, s->group, s->product);
    if (s->shows_stock) stock(c, s->product, s->group, TEXT_STORED,
        0, PRODUCTION_Y, c->building->loads_stored);
    else production(c, s->group,
        percentage(c->building->progress, RAW_PROGRESS_MAX), PRODUCTION_Y);
    producer_state(c, s->group, s->product, false, c->panel_y + PRODUCER_STATE_Y);
    emit(c, PHARAOH_INDUSTRY_WORKERS_PANEL, 0, 0, 0, 0, 0, 0);
    description(c, s->group);
}

static void draw_workshop(PharaohIndustryPanelContext *c,
    const PharaohIndustryPanelSpec *s)
{
    bool second = s->second_raw_resource != c->no_resource;
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, s->help, 0, 0);
    title(c, s->group, s->product);
    production(c, s->group,
        percentage(c->building->progress, WORKSHOP_PROGRESS_MAX), WORKSHOP_PRODUCTION_Y);
    stock(c, s->raw_resource, s->group,
        second ? TEXT_STORED_FIRST : TEXT_STORED_RAW,
        0, WORKSHOP_STOCK_Y, c->building->loads_stored);
    if (second) stock(c, s->second_raw_resource, s->group, TEXT_STORED_SECOND,
        1, WORKSHOP_STOCK_Y, c->building->second_input_stock);
    workshop_state(c, s->group, s->halt_resource, TEXT_NO_WORKERS,
        TEXT_NO_RAW, second ? 12 : -1, TEXT_FULL_STAFF);
    emit(c, PHARAOH_INDUSTRY_WORKERS_PANEL, 0, 0, 0, 0, 0, 0);
}

static void draw_guild(PharaohIndustryPanelContext *c,
    const PharaohIndustryPanelSpec *s)
{
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, s->help, 0, 0);
    title(c, s->group, s->product);
    workshop_state(c, s->group, c->no_resource, TEXT_NO_WORKERS,
        s->no_raw_text, -1, TEXT_FULL_STAFF);
    emit(c, PHARAOH_INDUSTRY_WORKERS_PANEL, 0, 0, 0, 0, 0, 0);
}

void pharaoh_industry_draw_work_camp(PharaohIndustryPanelContext *c)
{
    int32_t text, y = c->panel_y + 56;
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, c->work_camp_help, 0, 0);
    emit(c, PHARAOH_INDUSTRY_TITLE, c->work_camp_group, 0, 0, 0, 0, 0);
    if (c->road_access < 1) emit(c, PHARAOH_INDUSTRY_NO_ROAD, 0, 0, 0, 0, 0, y);
    else {
        if (c->building->num_workers < 1) text = CAMP_NEEDS_WORKERS;
        else if (c->building->worker_count == 0) text = CAMP_SUPPLYING;
        else text = CAMP_LOOKING + (c->building->labor_days_left[0] != 0) +
            ((c->building->labor_days_left[1] != 0) * (CAMP_ON_MONUMENTS - CAMP_LOOKING));
        emit(c, PHARAOH_INDUSTRY_TEXT, c->work_camp_group, text, 0, 0, 0, y);
    }
    emit(c, PHARAOH_INDUSTRY_INNER_PANEL, 0, 0, 0, 6, 0, 136);
    emit(c, PHARAOH_INDUSTRY_EMPLOYMENT, 0, 0, 0, 0, 0, c->panel_y + 138);
    description(c, c->work_camp_group);
}

void pharaoh_industry_draw_farm_crop(PharaohIndustryPanelContext *c, int32_t group)
{
    int32_t month = c->next_harvest_month(c->user_data);
    if (month >= 0) {
        int32_t text = FARM_NEXT_HARVEST;
        if (c->farm_on_floodplain(c->user_data)) { text = FARM_NEXT_FLOODS; group = c->farm_group; }
        emit(c, PHARAOH_INDUSTRY_LINE, group, text, 0, 0, 0,
            c->panel_height_blocks * c->block_height - FARM_CROP_FROM_BOTTOM);
        emit(c, PHARAOH_INDUSTRY_LINE, c->months_group, month, 0, 0, 0,
            c->panel_height_blocks * c->block_height - FARM_CROP_FROM_BOTTOM);
    }
    if (c->building_is_crop_farm(c->user_data))
        emit(c, PHARAOH_INDUSTRY_LINE, c->farm_group,
            c->farm_has_water(c->user_data) ? FARM_IRRIGATED : FARM_NOT_IRRIGATED,
            0, 0, 0, c->panel_height_blocks * c->block_height - FARM_IRRIGATION_FROM_BOTTOM);
}

#define FARM_FN(name,id) void name(PharaohIndustryPanelContext *c){draw_farm(c,&c->specs[id]);}
#define RAW_FN(name,id) void name(PharaohIndustryPanelContext *c){draw_raw(c,&c->specs[id]);}
#define WORK_FN(name,id) void name(PharaohIndustryPanelContext *c){draw_workshop(c,&c->specs[id]);}
#define GUILD_FN(name,id) void name(PharaohIndustryPanelContext *c){draw_guild(c,&c->specs[id]);}
FARM_FN(pharaoh_industry_draw_barley_farm,PHARAOH_PANEL_BARLEY_FARM)
FARM_FN(pharaoh_industry_draw_grain_farm,PHARAOH_PANEL_GRAIN_FARM)
FARM_FN(pharaoh_industry_draw_lettuce_farm,PHARAOH_PANEL_LETTUCE_FARM)
FARM_FN(pharaoh_industry_draw_flax_farm,PHARAOH_PANEL_FLAX_FARM)
FARM_FN(pharaoh_industry_draw_henna_farm,PHARAOH_PANEL_HENNA_FARM)
RAW_FN(pharaoh_industry_draw_stone_quarry,PHARAOH_PANEL_STONE_QUARRY)
RAW_FN(pharaoh_industry_draw_gold_mine,PHARAOH_PANEL_GOLD_MINE)
RAW_FN(pharaoh_industry_draw_copper_mine,PHARAOH_PANEL_COPPER_MINE)
RAW_FN(pharaoh_industry_draw_gemstone_mine,PHARAOH_PANEL_GEMSTONE_MINE)
RAW_FN(pharaoh_industry_draw_clay_pit,PHARAOH_PANEL_CLAY_PIT)
WORK_FN(pharaoh_industry_draw_brewery,PHARAOH_PANEL_BREWERY)
WORK_FN(pharaoh_industry_draw_weaver,PHARAOH_PANEL_WEAVER)
WORK_FN(pharaoh_industry_draw_weaponsmith,PHARAOH_PANEL_WEAPONSMITH)
WORK_FN(pharaoh_industry_draw_carpenters_guild,PHARAOH_PANEL_CARPENTERS_GUILD)
GUILD_FN(pharaoh_industry_draw_bricklayers_guild,PHARAOH_PANEL_BRICKLAYERS_GUILD)
GUILD_FN(pharaoh_industry_draw_stonemasons_guild,PHARAOH_PANEL_STONEMASONS_GUILD)

void pharaoh_industry_draw_reed_gatherer(PharaohIndustryPanelContext *c)
{
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, c->reed_help, 0, 0);
    title(c, c->reed_group, c->reed_resource);
    stock(c, c->reed_resource, c->reed_group, TEXT_STORED, 0, REED_STOCK_Y,
        c->building->loads_stored);
    producer_state(c, c->reed_group, c->reed_resource, true, c->panel_y + PRODUCER_STATE_Y);
    emit(c, PHARAOH_INDUSTRY_WORKERS_PANEL, 0, 0, 0, 0, 0, 0);
    description(c, c->reed_group);
}

void pharaoh_industry_draw_cattle_ranch(PharaohIndustryPanelContext *c)
{
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, c->cattle_help, 0, 0);
    title(c, c->cattle_group, c->cattle_product);
    production(c, c->farm_group,
        percentage(c->building->progress, CATTLE_PROGRESS_MAX), PRODUCTION_Y);
    stock(c, c->cattle_raw_resource, c->resource_names_group,
        c->cattle_raw_resource, 0, CATTLE_STOCK_Y, c->building->loads_stored);
    producer_state(c, c->cattle_group, c->cattle_product, true, c->panel_y + PRODUCER_STATE_Y);
    emit(c, PHARAOH_INDUSTRY_WORKERS_PANEL, 0, 0, 0, 0, 0, 0);
    description(c, c->cattle_group);
}

void pharaoh_industry_draw_artisans_guild(PharaohIndustryPanelContext *c)
{
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, c->artisans_help, 0, 0);
    title(c, c->artisans_group, c->no_resource);
    stock(c, c->paint_resource, c->artisans_group, ARTISANS_PAINT,
        0, WORKSHOP_STOCK_Y, c->building->loads_stored);
    stock(c, c->clay_resource, c->artisans_group, ARTISANS_CLAY,
        1, WORKSHOP_STOCK_Y, c->building->second_input_stock);
    workshop_state(c, c->artisans_group, c->no_resource, ARTISANS_NO_WORKERS,
        ARTISANS_NO_PAINT, ARTISANS_NO_CLAY, ARTISANS_FULL);
    emit(c, PHARAOH_INDUSTRY_WORKERS_PANEL, 0, 0, 0, 0, 0, 0);
}

void pharaoh_industry_draw_hunting_lodge(PharaohIndustryPanelContext *c)
{
    emit(c, PHARAOH_INDUSTRY_SET_HELP, 0, 0, 0, c->hunting_help, 0, 0);
    title(c, c->hunting_group, c->no_resource);
    stock(c, c->game_meat_resource, c->hunting_group, TEXT_STORED_FIRST,
        0, WORKSHOP_STOCK_Y, c->building->loads_stored);
    workshop_state(c, c->hunting_group, c->game_meat_resource, TEXT_NO_WORKERS,
        TEXT_NO_RAW, -1, TEXT_FULL_STAFF);
    emit(c, PHARAOH_INDUSTRY_WORKERS_PANEL, 0, 0, 0, 0, 0, 0);
}
