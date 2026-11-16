#ifndef PHARAOH_UI_INDUSTRY_PANELS_H
#define PHARAOH_UI_INDUSTRY_PANELS_H

#include <stdbool.h>
#include <stdint.h>

enum PharaohIndustryPanelId {
    PHARAOH_PANEL_BARLEY_FARM,
    PHARAOH_PANEL_GRAIN_FARM,
    PHARAOH_PANEL_LETTUCE_FARM,
    PHARAOH_PANEL_FLAX_FARM,
    PHARAOH_PANEL_HENNA_FARM,
    PHARAOH_PANEL_STONE_QUARRY,
    PHARAOH_PANEL_GOLD_MINE,
    PHARAOH_PANEL_COPPER_MINE,
    PHARAOH_PANEL_GEMSTONE_MINE,
    PHARAOH_PANEL_CLAY_PIT,
    PHARAOH_PANEL_BREWERY,
    PHARAOH_PANEL_WEAVER,
    PHARAOH_PANEL_WEAPONSMITH,
    PHARAOH_PANEL_CARPENTERS_GUILD,
    PHARAOH_PANEL_BRICKLAYERS_GUILD,
    PHARAOH_PANEL_STONEMASONS_GUILD,
    PHARAOH_PANEL_COUNT
};

enum PharaohIndustryCommandKind {
    PHARAOH_INDUSTRY_SET_HELP,
    PHARAOH_INDUSTRY_TITLE,
    PHARAOH_INDUSTRY_TITLE_ICON,
    PHARAOH_INDUSTRY_PRODUCTION,
    PHARAOH_INDUSTRY_LINE,
    PHARAOH_INDUSTRY_NUMBER,
    PHARAOH_INDUSTRY_STOCK,
    PHARAOH_INDUSTRY_TEXT,
    PHARAOH_INDUSTRY_NO_ROAD,
    PHARAOH_INDUSTRY_WORKERS_PANEL,
    PHARAOH_INDUSTRY_INNER_PANEL,
    PHARAOH_INDUSTRY_EMPLOYMENT,
    PHARAOH_INDUSTRY_FARM_WORKERS,
    PHARAOH_INDUSTRY_DESCRIPTION
};

typedef struct PharaohIndustryBuilding {
    int32_t num_workers;
    int32_t loads_stored;
    int32_t second_input_stock;
    int32_t progress;
    int32_t curse_days_left;
    int32_t worker_count;
    int32_t labor_days_left[2];
} PharaohIndustryBuilding;

typedef struct PharaohIndustryPanelSpec {
    int32_t help;
    int32_t product;
    int32_t group;
    int32_t raw_resource;
    int32_t second_raw_resource;
    int32_t halt_resource;
    int32_t shows_stock;
    int32_t no_raw_text;
} PharaohIndustryPanelSpec;

typedef struct PharaohIndustryCommand {
    enum PharaohIndustryCommandKind kind;
    int32_t group;
    int32_t text;
    int32_t resource;
    int32_t value;
    int32_t x;
    int32_t y;
} PharaohIndustryCommand;

typedef void (*PharaohIndustryEmitFunction)(
    void *user_data, const PharaohIndustryCommand *command);
typedef int32_t (*PharaohIndustryValueFunction)(void *user_data);
typedef int32_t (*PharaohIndustryTextFunction)(void *user_data,
    int32_t base_text);
typedef bool (*PharaohIndustryPredicateFunction)(void *user_data);

typedef struct PharaohIndustryPanelContext {
    const PharaohIndustryBuilding *building;
    const PharaohIndustryPanelSpec *specs;
    const uint8_t *mothballed;
    int32_t no_resource;
    int32_t road_access;
    int32_t panel_y;
    int32_t panel_height_blocks;
    int32_t block_height;
    int32_t farm_group;
    int32_t months_group;
    int32_t resource_names_group;
    int32_t work_camp_help;
    int32_t work_camp_group;
    int32_t reed_help;
    int32_t reed_group;
    int32_t reed_resource;
    int32_t cattle_help;
    int32_t cattle_group;
    int32_t cattle_product;
    int32_t cattle_raw_resource;
    int32_t artisans_help;
    int32_t artisans_group;
    int32_t paint_resource;
    int32_t clay_resource;
    int32_t hunting_help;
    int32_t hunting_group;
    int32_t game_meat_resource;
    PharaohIndustryValueFunction farm_growth_percent;
    PharaohIndustryValueFunction farm_fertility;
    PharaohIndustryValueFunction next_harvest_month;
    PharaohIndustryPredicateFunction farm_on_floodplain;
    PharaohIndustryPredicateFunction building_is_crop_farm;
    PharaohIndustryPredicateFunction farm_has_water;
    PharaohIndustryTextFunction worker_percentage_text;
    PharaohIndustryEmitFunction emit;
    void *user_data;
} PharaohIndustryPanelContext;

void pharaoh_industry_draw_work_camp(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_farm_crop(PharaohIndustryPanelContext *context,
    int32_t group);
void pharaoh_industry_draw_barley_farm(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_grain_farm(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_lettuce_farm(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_flax_farm(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_henna_farm(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_reed_gatherer(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_cattle_ranch(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_stone_quarry(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_gold_mine(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_copper_mine(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_gemstone_mine(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_clay_pit(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_brewery(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_weaver(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_weaponsmith(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_carpenters_guild(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_bricklayers_guild(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_stonemasons_guild(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_artisans_guild(PharaohIndustryPanelContext *context);
void pharaoh_industry_draw_hunting_lodge(PharaohIndustryPanelContext *context);

#endif
