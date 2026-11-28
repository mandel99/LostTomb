#ifndef PHARAOH_MILITARY_SHIPWRIGHT_DEMAND_H
#define PHARAOH_MILITARY_SHIPWRIGHT_DEMAND_H

#include "pharaoh/walker/record_queries.h"

#include <stddef.h>
#include <stdint.h>

typedef struct PharaohShipwrightDemandBuilding {
    uint8_t state;
    int16_t type;
    int16_t workers;
    int16_t boat_id;
    int16_t boat_order;
} PharaohShipwrightDemandBuilding;

int32_t pharaoh_shipwright_boat_needed(
    PharaohShipwrightDemandBuilding *buildings, size_t building_count,
    const PharaohWalkerQueryRecord *walkers, int32_t *walker_type);

/* The same complete460FB0 algorithm borrows either normalized or actual
 * building fields. Widths are BYTE state and signed WORD type/workers/IDs.
 * The mutable boat ID cell remains in the supplied pool. No pool copy. */
typedef struct PharaohShipwrightDemandBorrowedFields {
    uint8_t *base;
    size_t stride,state,type,workers,boat_id,boat_order;
} PharaohShipwrightDemandBorrowedFields;
int32_t pharaoh_shipwright_boat_needed_borrowed(
    const PharaohShipwrightDemandBorrowedFields *,size_t building_count,
    const PharaohWalkerQueryRecord *,int32_t *walker_type);

#endif
