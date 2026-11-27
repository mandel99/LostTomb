#ifndef PHARAOH_POPULATION_HOUSE_ECONOMY_H
#define PHARAOH_POPULATION_HOUSE_ECONOMY_H

#include <stdint.h>
#include "pharaoh/building/model_types.h"

/* Actual7999D8 pool. The market and house inventories overlap. */
typedef struct PharaohHouseEconomyRecord {
    uint8_t state,reserved_001[2],size,merged,house_size;
    uint8_t reserved_006[0x10-6];
    int16_t type,level;
    uint8_t reserved_014[0x1c-0x14];
    int16_t population;
    uint8_t reserved_01e[0x35-0x1e],water;
    uint8_t reserved_036[6];
    int16_t primary_input;
    uint8_t reserved_03e,well;
    uint8_t reserved_040[6],physician,reserved_047;
    uint8_t courthouse,reserved_049,school,library,reserved_04c,dentist;
    uint8_t reserved_04e[4];
    uint16_t unsupported_food;
    union {
        struct { int16_t food[8],beer,pottery,linen,jewelry;
            uint8_t juggler,musician; };
        int16_t market_stock[13]; /* [0] is native market index-1. */
    };
    uint8_t reserved_06e[0x7e-0x6e],merge_state,reserved_07f;
    int8_t entertainment,education,health,religion;
    uint8_t reserved_084,evolve_text,reserved_086[0x9e-0x86];
    int16_t pottery_demand,jewelry_demand,linen_demand,beer_demand;
    uint8_t reserved_0a6[0xbe-0xa6];
    int8_t desirability;
    uint8_t reserved_0bf[0xcc-0xbf];
    int16_t secondary_input;
    uint8_t reserved_0ce[0x104-0xce];
    int16_t primary_orders,secondary_orders;
} PharaohHouseEconomyRecord;

/* OriginEA5B74, player stride49D8. Only proven DWORD counters below are used;
 * the other words retain opaque native storage. */
typedef struct PharaohHouseDemandBank { uint32_t words[0x49d8/4]; }
    PharaohHouseDemandBank;

typedef struct PharaohHouseEconomyContext {
    volatile PharaohHouseEconomyRecord *buildings;
    const volatile PharaohHouseModel *models; /*70CDCC*/
    volatile PharaohHouseDemandBank *players;
    const volatile uint32_t *current_player; /*EAAC28*/
    const volatile int32_t *food_count,*foods; /*F53DD8/DC*/
    const volatile uint32_t *negative_influence; /*EADCD4*/
} PharaohHouseEconomyContext;

uint32_t pharaoh_house_check_requirements(const PharaohHouseEconomyContext *,
    int32_t building,uint32_t next_level);
void pharaoh_house_update_evolve_text(const PharaohHouseEconomyContext *,int32_t building);

/* Complete fixed child of483910; no additional native-entry credit. */
uint32_t pharaoh_house_market_supply(const PharaohHouseEconomyContext *,int32_t house,uint32_t market);

/* OriginEA60AC; actual DWORD resource totals, one49D8 bank per player. */
typedef struct PharaohGranaryResourceBank { uint32_t words[0x49d8/4]; }
    PharaohGranaryResourceBank;
int32_t pharaoh_granary_remove_resource(volatile PharaohHouseEconomyRecord *,
    volatile PharaohGranaryResourceBank *,int32_t player,int32_t building,
    int32_t resource,int32_t amount);
int32_t pharaoh_building_recruiter_add_input(volatile PharaohHouseEconomyRecord *,
    int32_t building,int32_t amount,int32_t resource);

#endif
