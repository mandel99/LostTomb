#ifndef PHARAOH_POPULATION_FOOD_SUPPLY_QUERY_H
#define PHARAOH_POPULATION_FOOD_SUPPLY_QUERY_H

#include "pharaoh/building/model_types.h"
#include "pharaoh/building/occupancy_query.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/city/food_rate.h"
#include "pharaoh/commerce/empire_queries.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/map/tile_adjacency.h"

/* A view of the actual player bank, not a copied food-query result. */
typedef struct PharaohFoodSupplyPlayer {
    uint8_t reserved_0000[0x2826];
    uint16_t trade_permissions[36];
    uint8_t reserved_286e[0x4478 - 0x286e];
    uint16_t temple_count;
    uint8_t reserved_447a[0x45dc - 0x447a];
    int32_t warning_population_limit;
    uint8_t reserved_45e0[0x472c - 0x45e0];
    int32_t temple_building;
    uint8_t reserved_4730[0x476c - 0x4730];
    int32_t ratio_first_divisor, ratio_first_dividend;
    uint32_t reserved_4774;
    int32_t ratio_second_divisor, ratio_second_dividend;
    uint8_t reserved_4780[0x49d8 - 0x4780];
} PharaohFoodSupplyPlayer;

_Static_assert(sizeof(PharaohFoodSupplyPlayer) == 0x49d8 &&
    offsetof(PharaohFoodSupplyPlayer, trade_permissions) == 0x2826 &&
    offsetof(PharaohFoodSupplyPlayer, temple_count) == 0x4478 &&
    offsetof(PharaohFoodSupplyPlayer, warning_population_limit) == 0x45dc &&
    offsetof(PharaohFoodSupplyPlayer, temple_building) == 0x472c &&
    offsetof(PharaohFoodSupplyPlayer, ratio_first_divisor) == 0x476c &&
    offsetof(PharaohFoodSupplyPlayer, ratio_second_divisor) == 0x4778,
    "native food-supply player fields");

typedef struct PharaohFoodSupplyQueryContext {
    const PharaohBuildingTypeRecord *buildings; /* 4000 native264-byte rows. */
    const PharaohBuildingModelRecord *models; /* 705EA4, laborers DWORD+20. */
    const PharaohFoodSupplyPlayer *players; /* EA1870, actual49D8 stride. */
    const uint32_t *current_player; /* EAAC28, whole DWORD. */
    const uint32_t *first_ratio_price, *second_ratio_price; /* 78D66C/78D668. */
    const PharaohEmpireCityRecord *cities; /* 61 rows at77DE00, stride106. */
    const PharaohEmpireTradeAmounts *trade_amounts; /* 77646C,36 DWORDs/route. */
    const int32_t *map_origin; /* BCA2D8. */
    uint32_t *current_tile; /* BC9768. */
    const uint8_t *soil_values; /* A5D4CC, BYTE values. */
    const PharaohMapTileAreaOffset (*footprints)[6]; /* 5CC8C0,8-byte rows. */
    PharaohOccupancyState *occupancy; /* Real terrain/object grids and offsets. */
} PharaohFoodSupplyQueryContext;

typedef struct PharaohFoodSupplyQueryResult {
    PharaohPercentageScaleStatus status;
    int32_t value;
} PharaohFoodSupplyQueryResult;

/* Native00447530. Defined return is full signed EAX. Initial signed IDIV
 * overflow is reported before later calls/stores; no native EAX is claimed on
 * that fault. All pointers bind the same live city/building/empire/map owners. */
PharaohFoodSupplyQueryResult pharaoh_city_food_supply_query(
    const PharaohFoodSupplyQueryContext *context);

/* Whole native00489050/00489110/00488140 C support, with actual x87 PC/RC
 * and FTOL conversion. configured consumes the native low BYTE only. Native
 * ECX was forwarded to helpers whose completed algorithms do not read it. */
int32_t pharaoh_food_supply_area_quality(const PharaohFoodSupplyQueryContext *,
    int32_t x, int32_t y, int32_t columns, int32_t rows);
int32_t pharaoh_food_supply_farm_quality(const PharaohFoodSupplyQueryContext *,
    int32_t building);
int32_t pharaoh_food_supply_farm_capacity(const PharaohFoodSupplyQueryContext *,
    int32_t building, uint8_t configured);

#endif
