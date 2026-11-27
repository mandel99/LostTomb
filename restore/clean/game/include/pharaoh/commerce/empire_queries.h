#ifndef PHARAOH_COMMERCE_EMPIRE_QUERIES_H
#define PHARAOH_COMMERCE_EMPIRE_QUERIES_H

#include <stdbool.h>
#include <stdint.h>

enum {
    PHARAOH_EMPIRE_OBJECT_SIZE = 98,
    PHARAOH_EMPIRE_ROUTE_SIZE = 324,
    PHARAOH_EMPIRE_CITY_RESOURCE_SLOTS = 8,
    PHARAOH_EMPIRE_CITY_SOLD_RESOURCE_SLOTS = 14,
    PHARAOH_EMPIRE_OBJECT_COUNT = 200,
    PHARAOH_EMPIRE_CITY_COUNT = 61,
    PHARAOH_EMPIRE_RESOURCE_COUNT = 36,
    PHARAOH_EMPIRE_ROUTE_COUNT = 50,
    PHARAOH_EMPIRE_ROUTE_POINT_COUNT = 50
};

typedef struct PharaohEmpireObjectRecord {
    uint8_t kind;
    uint8_t in_use;
    uint8_t reserved_02[6];
    uint16_t image_width;
    uint16_t image_height;
    uint16_t image_id;
    uint16_t image_offset;
    uint8_t reserved_10[8];
    uint8_t city_type;
    uint8_t name_id;
    uint8_t route_id;
    uint8_t is_open;
    int16_t cost_to_open;
    uint8_t sold_resources[PHARAOH_EMPIRE_CITY_SOLD_RESOURCE_SLOTS];
    uint8_t reserved_2c[7];
    uint8_t city_type_countdown;
    uint8_t bought_resources[PHARAOH_EMPIRE_CITY_RESOURCE_SLOTS];
    uint8_t invasion_path_id;
    uint8_t invasion_years;
    uint8_t trade_amounts[36];
} PharaohEmpireObjectRecord;

typedef struct PharaohEmpireCityRecord {
    uint8_t in_use;
    uint8_t reserved_01;
    uint8_t kind;
    uint8_t name_id;
    uint8_t route_id;
    uint8_t is_open;
    uint8_t buys[PHARAOH_EMPIRE_RESOURCE_COUNT];
    uint8_t sells[PHARAOH_EMPIRE_RESOURCE_COUNT];
    int16_t cost_to_open;
    int16_t unread_default;
    int16_t trader_entry_delay;
    int16_t trader_blocked;
    int16_t map_object;
    uint8_t is_sea_trade;
    uint8_t reserved_59;
    int16_t trader_walkers[8];
} PharaohEmpireCityRecord;

typedef struct PharaohEmpireTradeAmounts {
    int32_t resource[PHARAOH_EMPIRE_RESOURCE_COUNT];
} PharaohEmpireTradeAmounts;

typedef struct PharaohEmpireRouteRecord {
    uint8_t step;
    uint8_t reserved_01[3];
    uint32_t path;
    uint8_t points[PHARAOH_EMPIRE_ROUTE_POINT_COUNT * 6];
    int32_t path_length;
    int32_t path_capacity;
    int16_t first_object;
    int16_t last_object;
    uint8_t kind;
    uint8_t point_count;
    uint8_t in_use;
    uint8_t reserved_143;
} PharaohEmpireRouteRecord;

typedef struct PharaohEmpireCityImageCatalog {
    int32_t default_city_group;
    int32_t rank_ten_city_group;
    int32_t type_two_city_group;
    int32_t type_four_city_group;
    const int32_t (*name_offsets)[2];
} PharaohEmpireCityImageCatalog;

uint32_t pharaoh_empire_object_city_buys_resource(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    uint32_t resource_kind);

uint32_t pharaoh_empire_route_is_sea(
    const PharaohEmpireRouteRecord *routes, int32_t route_kind);

uint8_t pharaoh_empire_object_trade_amount(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    int32_t resource_kind);

bool pharaoh_empire_city_has_trade_limit(
    const PharaohEmpireCityRecord *cities,
    const PharaohEmpireTradeAmounts *route_limits,
    int32_t city_id, int32_t resource_kind,
    int16_t ra_bonus, int16_t ra_penalty, int16_t ra_minor_penalty);

uint32_t pharaoh_empire_can_import_resource(
    const PharaohEmpireCityRecord cities[PHARAOH_EMPIRE_CITY_COUNT],
    const PharaohEmpireTradeAmounts *route_limits, int32_t resource_kind,
    int16_t ra_bonus, int16_t ra_penalty, int16_t ra_minor_penalty);

uint32_t pharaoh_empire_can_import_resource_potentially(
    const PharaohEmpireCityRecord cities[PHARAOH_EMPIRE_CITY_COUNT],
    const PharaohEmpireTradeAmounts *route_limits, int32_t resource_kind,
    int16_t ra_bonus, int16_t ra_penalty, int16_t ra_minor_penalty);

int32_t pharaoh_empire_object_city_image(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    int32_t scenario_rank,
    const PharaohEmpireCityImageCatalog *catalog);

int32_t pharaoh_empire_city_for_trade_route(
    const PharaohEmpireCityRecord *cities, uint32_t route_id);
int32_t pharaoh_empire_route_find_for_city(
    const PharaohEmpireObjectRecord *objects,
    const PharaohEmpireCityRecord *cities,
    PharaohEmpireRouteRecord *routes, int32_t city_id,
    uint8_t *ours_first);
int32_t pharaoh_empire_route_travel_months(
    const PharaohEmpireRouteRecord *route);

uint32_t pharaoh_empire_object_city_sells_resource(
    const PharaohEmpireObjectRecord *objects, int32_t object_index,
    uint32_t resource_kind);
uint32_t pharaoh_empire_our_city_sells_resource(
    const PharaohEmpireObjectRecord *objects, uint32_t resource_kind);

#endif
