#ifndef PHARAOH_BUILDING_DOCK_ACCESS_H
#define PHARAOH_BUILDING_DOCK_ACCESS_H

#include "pharaoh/map/water_routing.h"
#include "pharaoh/map/building_access_queries.h"

typedef struct PharaohDockAccessContext {
    const PharaohWaterRoutingContext *water;
    const PharaohRoadAccessMap *access; /* Shares water->distance. */
    const int16_t *river_entry_x, *river_entry_y; /* 784C8C/8E signed WORDs. */
    PharaohBuildingTypeRecord *buildings; /* 7999D8, 264-byte records. */
    const int32_t *highest_building_id; /* 78D790, reloaded each iteration. */
} PharaohDockAccessContext;

/* Complete native00457AC0, full EAX0/1; child status does not short-circuit. */
uint32_t pharaoh_dock_is_connected_to_open_water(
    const PharaohDockAccessContext *, int32_t x, int32_t y);
/* Complete native00457A40. Updates only live kinds75/192 with byte05==0. */
void pharaoh_docks_refresh_water_access(const PharaohDockAccessContext *);

#endif
