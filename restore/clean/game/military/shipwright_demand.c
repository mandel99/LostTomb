#include "pharaoh/military/shipwright_demand.h"
#include <stddef.h>

enum {
    FISHING_WHARF = 0x4c,
    SHIPWRIGHT = 0x4a,
    TRANSPORT_WHARF = 0xb5,
    WARSHIP_WHARF = 0xb6,
    FISHING_BOAT = 0x19,
    TRANSPORT_SHIP = 0x4d,
    WARSHIP = 0x4e,
    BOAT_ORDER_FOR_CITY = 0x1000
};

static int16_t *word(const PharaohShipwrightDemandBorrowedFields *f,size_t id,size_t offset)
{ return (int16_t *)(f->base+id*f->stride+offset); }
static int32_t valid_wharf_boat(const PharaohShipwrightDemandBorrowedFields *f,
    int32_t building_id, const PharaohWalkerQueryRecord *walkers,
    int32_t expected_type)
{
    const int32_t walker_id = *word(f,(size_t)building_id,f->boat_id);
    if (walker_id <= 0) return 0;
    if (walkers[walker_id].state == 0u ||
        (walkers[walker_id].type != expected_type &&
         walkers[walker_id].type != 0u) ||
        walkers[walker_id].building_id != building_id) {
        *word(f,(size_t)building_id,f->boat_id) = 0;
        return 0;
    }
    return 1;
}

/* Complete C translation of Pharaoh.exe 0x00460FB0. */
int32_t pharaoh_shipwright_boat_needed_borrowed(
    const PharaohShipwrightDemandBorrowedFields *f, size_t building_count,
    const PharaohWalkerQueryRecord *walkers, int32_t *walker_type)
{
    int32_t wharves[3] = {0, 0, 0};
    int32_t boats[3] = {0, 0, 0};
    int32_t orders[3] = {0, 0, 0};
    size_t building_id;
    for (building_id = 1; building_id < building_count; ++building_id) {
        if (f->base[building_id*f->stride+f->state] != 1u ||
            *word(f,building_id,f->workers) == 0) continue;
        const int16_t type=*word(f,building_id,f->type);
        if (type == FISHING_WHARF) {
            ++wharves[0];
            boats[0] += valid_wharf_boat(f, (int32_t)building_id,
                walkers, FISHING_BOAT);
        } else if (type == TRANSPORT_WHARF) {
            ++wharves[1];
            boats[1] += valid_wharf_boat(f, (int32_t)building_id,
                walkers, TRANSPORT_SHIP);
        } else if (type == WARSHIP_WHARF) {
            ++wharves[2];
            boats[2] += valid_wharf_boat(f, (int32_t)building_id,
                walkers, WARSHIP);
        } else if (type == SHIPWRIGHT && *word(f,building_id,f->boat_order) != 0) {
            orders[0] += *word(f,building_id,f->boat_order) == (BOAT_ORDER_FOR_CITY | FISHING_BOAT);
            orders[1] += *word(f,building_id,f->boat_order) == (BOAT_ORDER_FOR_CITY | TRANSPORT_SHIP);
            orders[2] += *word(f,building_id,f->boat_order) == (BOAT_ORDER_FOR_CITY | WARSHIP);
        }
    }
    if (orders[0] + boats[0] < wharves[0]) *walker_type = FISHING_BOAT;
    else if (orders[1] + boats[1] < wharves[1]) *walker_type = TRANSPORT_SHIP;
    else if (orders[2] + boats[2] < wharves[2]) *walker_type = WARSHIP;
    else return 0;
    return 1;
}

/* Existing normalized API delegates to the same complete460FB0 algorithm. */
int32_t pharaoh_shipwright_boat_needed(
    PharaohShipwrightDemandBuilding *buildings,size_t building_count,
    const PharaohWalkerQueryRecord *walkers,int32_t *walker_type)
{
    const PharaohShipwrightDemandBorrowedFields fields={(uint8_t *)buildings,
        sizeof(*buildings),offsetof(PharaohShipwrightDemandBuilding,state),
        offsetof(PharaohShipwrightDemandBuilding,type),offsetof(PharaohShipwrightDemandBuilding,workers),
        offsetof(PharaohShipwrightDemandBuilding,boat_id),offsetof(PharaohShipwrightDemandBuilding,boat_order)};
    return pharaoh_shipwright_boat_needed_borrowed(&fields,building_count,walkers,walker_type);
}
