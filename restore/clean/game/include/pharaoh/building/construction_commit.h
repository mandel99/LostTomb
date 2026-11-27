#ifndef PHARAOH_BUILDING_CONSTRUCTION_COMMIT_H
#define PHARAOH_BUILDING_CONSTRUCTION_COMMIT_H

#include "pharaoh/building/construction_load.h"
#include "pharaoh/building/construction_basic_warnings.h"
#include "pharaoh/building/construction_resource_warnings.h"
#include "pharaoh/building/construction_commit_support.h"
#include "pharaoh/building/construction_site_checks.h"
#include "pharaoh/building/demolition.h"
#include "pharaoh/building/dock_access.h"
#include "pharaoh/building/entertainment_construction.h"
#include "pharaoh/building/housing_construction.h"
#include "pharaoh/commerce/storage_withdraw.h"
#include "pharaoh/map/bridge_placement.h"
#include "pharaoh/map/construction_footprint.h"
#include "pharaoh/map/construction_route_actions.h"
#include "pharaoh/map/surface_paint.h"
#include "pharaoh/map/wall_construction.h"
#include "pharaoh/military/construction_alert.h"
#include "pharaoh/monuments/pyramid_preview.h"
#include "pharaoh/monuments/scenario_slots.h"
#include "pharaoh/simulation/finance_calendar.h"

/* Defined below from actual Native fields; the city bank is shared by all
 * construction, storage, population and finance views. */
typedef struct PharaohConstructionCommitCity {
    uint8_t native_0000[0x2];
    uint8_t player_active;
    uint8_t native_0003[0x9];
    int32_t treasury;
    uint8_t native_0010[0x2742];
    int16_t storage_cursor;
    uint8_t native_2754[0x538];
    uint32_t construction_current_year;
    uint8_t native_2c90[0x16aa];
    uint16_t sphinx_stage;
    uint8_t native_433c[0x13c];
    uint16_t temple_placed;
    uint8_t native_447a[0xc];
    uint16_t palace_placed;
    uint8_t native_4488[0x2];
    int16_t temple_count;
    uint8_t native_448c[0x2a0];
    int32_t temple_building;
    uint8_t native_4730[0xe4];
    uint16_t festival_placed;
    uint8_t native_4816[0x4a];
    uint32_t withdrawn[36];
    uint32_t stored[36];
    uint8_t native_4980[0x1c];
    uint32_t mansion_placed;
    uint8_t native_49a0[0x30];
    uint32_t academy_placed;
    uint8_t native_49d4[0x4];
} PharaohConstructionCommitCity;
_Static_assert(sizeof(PharaohConstructionCommitCity)==0x49d8,
    "native commit city stride");
_Static_assert(offsetof(PharaohConstructionCommitCity,player_active)==0x2,"native city player_active");
_Static_assert(offsetof(PharaohConstructionCommitCity,treasury)==0xc,"native city treasury");
_Static_assert(offsetof(PharaohConstructionCommitCity,storage_cursor)==0x2752,"native city storage_cursor");
_Static_assert(offsetof(PharaohConstructionCommitCity,construction_current_year)==0x2c8c,"native city construction_current_year");
_Static_assert(offsetof(PharaohConstructionCommitCity,sphinx_stage)==0x433a,"native city sphinx_stage");
_Static_assert(offsetof(PharaohConstructionCommitCity,temple_placed)==0x4478,"native city temple_placed");
_Static_assert(offsetof(PharaohConstructionCommitCity,palace_placed)==0x4486,"native city palace_placed");
_Static_assert(offsetof(PharaohConstructionCommitCity,temple_count)==0x448a,"native city temple_count");
_Static_assert(offsetof(PharaohConstructionCommitCity,temple_building)==0x472c,"native city temple_building");
_Static_assert(offsetof(PharaohConstructionCommitCity,festival_placed)==0x4814,"native city festival_placed");
_Static_assert(offsetof(PharaohConstructionCommitCity,withdrawn)==0x4860,"native city withdrawn[36]");
_Static_assert(offsetof(PharaohConstructionCommitCity,stored)==0x48f0,"native city stored[36]");
_Static_assert(offsetof(PharaohConstructionCommitCity,mansion_placed)==0x499c,"native city mansion_placed");
_Static_assert(offsetof(PharaohConstructionCommitCity,academy_placed)==0x49d0,"native city academy_placed");

typedef uint8_t (*PharaohConstructionTerrainRule)(uint32_t terrain);
typedef struct PharaohConstructionRequirements {
    uint32_t floodplain_count,road,wall,water_40000,water_10004,forest,terrain_10800;
} PharaohConstructionRequirements; /* Native6059B4..6059CF. */

typedef struct PharaohConstructionMonument {
    const PharaohMonumentPlacementObject *placement;
    void *progress_object;
    PharaohMonumentExistingProgress progress; /* Actual completed C method. */
} PharaohConstructionMonument;

typedef struct PharaohConstructionCommitContext {
    PharaohBuildingAllocationContext *allocation;
    PharaohConstructionLoadContext *creation;
    PharaohConstructionCommitCity *cities;
    const PharaohBuildingModelRecord *models;
    const PharaohConstructionUndoContext *backup;
    const PharaohConstructionEntryPoints *entry_points;
    const PharaohConstructionRequirements *requirements;
    const PharaohConstructionTerrainRule *terrain_rule;
    uint32_t *cost,*credit,*count,*size,*native_bc9720;
    const uint32_t *editor_mode,*selected_cell; /*605A0C/BCA300. */
    uint32_t *spent_23,*spent_26,*spent_30; /*BC9650/5C/6C. */
    const int32_t *native_705c1c,*native_bcb9b0,*native_bcb9b4;
    uint32_t *native_705c6c;
    const uint8_t *native_bca314,*native_bca315;
    uint8_t *placement_byte; /*605A5C. */
    uint32_t *undo_cursor;
    int32_t *undo_ticks,*undo_ready;
    uint8_t *redraw;
    const PharaohConstructionBasicWarnings *basic;
    const PharaohConstructionResourceWarnings *resources;
    const PharaohConstructionAccessWarnings *access;
    const PharaohConstructionMilitaryWarnings *military;
    const PharaohDemolitionContext *demolition;
    const PharaohHousingConstructionContext *housing;
    const PharaohConstructionRouteActionsContext *routes;
    const PharaohWallConstructionContext *walls;
    const PharaohSurfacePaintContext *paint;
    const PharaohBridgePlacementContext *bridge;
    const PharaohEntertainmentConstructionContext *entertainment;
    PharaohConstructionFootprintContext *footprint;
    PharaohWaterOrientationMap *water_orientation;
    const PharaohDockAccessContext *dock;
    const PharaohGatehouseImagePart *gatehouse_parts;
    const PharaohTempleComplexSiteContext *temple_site;
    const PharaohMonumentPlacementContext *monument_site;
    const PharaohMausoleumProjectionContext *mausoleum;
    const PharaohPyramidPreviewContext *pyramid_preview;
    const int32_t *pyramid_tool;
    const PharaohMonumentImagePart *sun_parts;
    PharaohComplexProgressContext *sun_progress;
    PharaohConstructionMonument abu_simbel,library,caesareum,lighthouse;
    PharaohConstructionMonument small_tomb,medium_tomb,large_tomb,grand_tomb;
    const PharaohMonumentSlotContext *slots;
    const PharaohConstructionAlertContext *alert;
    PharaohSceneRouting *routing;
    const PharaohStorageWithdrawContext *storage;
    const PharaohWalkerQueryRecord *walkers;
} PharaohConstructionCommitContext;

/* Whole4728A0. Every nested context/provider borrows the actual live scene;
 * creation operations and monument progress callbacks must be genuine C.
 * Definitions are the pinned237 immutable Native rows (first DWORD0..6,
 * identical to the existing normalized BYTE size). No integer call tokens,
 * original-image callbacks or missing-provider substitutes are supported.
 * Native normal result is void; C status only propagates a genuine food
 * query's checked IDIV fault before later effects. */
PharaohPercentageScaleStatus pharaoh_construction_commit(
    const PharaohConstructionCommitContext *,uint32_t player,int32_t orientation,
    int32_t first_x,int32_t first_y,int32_t last_x,int32_t last_y,int32_t tool);

#endif
