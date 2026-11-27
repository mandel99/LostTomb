#ifndef PHARAOH_RUNTIME_NATIVE_LEAF_QUERIES_H
#define PHARAOH_RUNTIME_NATIVE_LEAF_QUERIES_H

#include "pharaoh/map/building_access_queries.h"
#include "pharaoh/events/event_queries.h"
#include "pharaoh/military/legion_orders.h"

typedef struct PharaohIndexedChainNode {
    struct PharaohIndexedChainNode *next;
    uint32_t reserved_04;
    int32_t key;
} PharaohIndexedChainNode;

/* Native540570 returns the actual node in EAX. Its unused stack slot is omitted. */
PharaohIndexedChainNode *pharaoh_indexed_chain_find(PharaohIndexedChainNode *node,int32_t minimum);

uint8_t pharaoh_walker_group_status_nonnegative(const PharaohWalkerQueryRecord *walkers,
    const PharaohNativeLegionRecord *formations,int32_t id);
uint32_t pharaoh_trade_ship_assignment_invalid(const PharaohWalkerQueryRecord *walkers,
    const PharaohBuildingTypeRecord *buildings,int32_t id);
uint8_t pharaoh_building_overlay_allows(const PharaohBuildingTypeRecord *buildings,
    const volatile uint32_t *mode,const int32_t *const *type_lists,int32_t id);

typedef struct PharaohDeferredEventFilter {
    const volatile int32_t *minimum_index,*event_count;
    const volatile PharaohScenarioEvent *events;
    const volatile uint8_t *phase;
} PharaohDeferredEventFilter;

uint8_t pharaoh_event_deferred_allows(const PharaohDeferredEventFilter *context,int32_t index);

/* View starts EA3E30; actual player stride49D8, signed age WORDs start+4. */
typedef struct PharaohPopulationLeafPlayer {
    int32_t recorded_months;
    int16_t people_at_age[100];
    uint8_t reserved_0cc[0x49d8-0xcc];
} PharaohPopulationLeafPlayer;

typedef struct PharaohPopulationGraphDates {
    int32_t end_year,end_month,start_year,start_month; /* EA1800..0C */
} PharaohPopulationGraphDates;

typedef struct PharaohPopulationGraphContext {
    const volatile PharaohPopulationLeafPlayer *players;
    const volatile uint32_t *current_player;
    const volatile int16_t *scenario_start_year;
    const volatile int32_t *current_year,*current_month;
    volatile PharaohPopulationGraphDates *dates;
} PharaohPopulationGraphContext;

void pharaoh_population_remove_from_decade(volatile PharaohPopulationLeafPlayer *players,
    int32_t player,int32_t decade,int32_t count);
void pharaoh_population_graph_dates(const PharaohPopulationGraphContext *context,int32_t distance);
void pharaoh_pyramid_entry_road_coordinates(const PharaohSpecialEntryMap *map,
    int32_t id,volatile int32_t *x,volatile int32_t *y);

#endif
