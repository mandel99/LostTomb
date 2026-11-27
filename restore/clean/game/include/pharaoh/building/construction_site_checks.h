#ifndef PHARAOH_BUILDING_CONSTRUCTION_SITE_CHECKS_H
#define PHARAOH_BUILDING_CONSTRUCTION_SITE_CHECKS_H
#include "pharaoh/building/temple_images.h"
#include "pharaoh/monuments/placement.h"
#include "pharaoh/monuments/mausoleum_projection.h"
#include "pharaoh/monuments/progress_reports.h"
#include "pharaoh/events/event_queries.h"
#include "pharaoh/map/terrain_queries.h"
#include "pharaoh/runtime/city_warning.h"

typedef struct PharaohTempleComplexSiteContext {
    const PharaohMapTerrainView *map;
    const int16_t *walker_ids;
    const PharaohConstructionEntryPoints *entry_points;
    const int32_t *orientation,*construction_orientation;
} PharaohTempleComplexSiteContext;

typedef struct PharaohConstructionMilitaryWarnings {
    const uint32_t *editor_active,*invasion_warning_shown;
    PharaohScenarioEvent *events;
    const int32_t *event_count,*current_player;
    uint8_t *invasion_search_flag;
    int32_t *current_event_index;
    const PharaohEventCalendarView *calendar;
    const PharaohScenarioPlayerRuntime *players;
    PharaohCityWarningContext *warnings;
} PharaohConstructionMilitaryWarnings;

uint32_t pharaoh_gatehouse_site_fits(PharaohMapSurfaceContext *surface,
    const PharaohGatehouseImagePart parts[4],int32_t x,int32_t y,int32_t mode);
int32_t pharaoh_temple_complex_can_place(const PharaohTempleComplexSiteContext *c,
    int32_t x,int32_t y);
uint8_t pharaoh_mausoleum_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMausoleumProjectionContext *projection,
    const int32_t *orientation,uint32_t cell);
uint8_t pharaoh_sun_temple_can_place(const PharaohMonumentPlacementContext *c,
    const PharaohMonumentImagePart parts[100],uint32_t cell);
uint8_t pharaoh_sun_temple_has_completed(PharaohComplexProgressContext *c);
void pharaoh_construction_warning_military(const PharaohConstructionMilitaryWarnings *c,
    int32_t tool);
#endif
