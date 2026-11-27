#ifndef PHARAOH_BUILDING_CONSTRUCTION_COMMIT_SUPPORT_H
#define PHARAOH_BUILDING_CONSTRUCTION_COMMIT_SUPPORT_H
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/map/road_access.h"
#include "pharaoh/runtime/city_warning.h"

typedef struct PharaohConstructionAccessWarnings {
    const uint32_t *editor_active,*warning_shown;
    const int32_t *problem_messages;
    PharaohCityWarningContext *warnings;
    PharaohRoadAccessContext *roads;
    const PharaohMapTerrainView *map;
    const int32_t (*patterns)[24];
} PharaohConstructionAccessWarnings;
void pharaoh_construction_warning_road_access(const PharaohConstructionAccessWarnings *c,
    int32_t tool,int32_t x,int32_t y,int32_t size);
void pharaoh_construction_warning_tower(const PharaohConstructionAccessWarnings *c,
    int32_t tool,int32_t x,int32_t y,int32_t layout);
void pharaoh_map_restore_free_tile_images(const PharaohMapSurfaceContext *surface,
    const uint32_t *backup_images);
void pharaoh_construction_ferry_landing_placed(uint32_t *ferry_state,
    const int32_t *building,PharaohCityWarningContext *warnings,PharaohMapBuildingUndo *undo);
void pharaoh_map_undo_release_reservations(PharaohMapBuildingUndo *undo,
    PharaohBuildingTypeRecord *live);
#endif
