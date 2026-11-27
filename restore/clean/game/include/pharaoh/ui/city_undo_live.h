#ifndef PHARAOH_UI_CITY_UNDO_LIVE_H
#define PHARAOH_UI_CITY_UNDO_LIVE_H

#include "pharaoh/building/type_queries.h"
#include "pharaoh/commerce/storage_queries.h"
#include "pharaoh/commerce/storage_withdraw.h"
#include "pharaoh/building/allocation.h"
#include "pharaoh/building/construction_support.h"
#include "pharaoh/map/farm_tiles.h"
#include "pharaoh/monuments/mausoleum_images.h"
#include "pharaoh/building/construction_commit.h"
#include "pharaoh/editor/terrain_kinds.h"
#include "pharaoh/map/route_tile_placement.h"
#include "pharaoh/map/ferry_routing.h"

/* Complete logical AL result of native45B230. Uses the actual264-byte
   building pool and196-byte storage policy pool, including slot0. Resource
   is a native index0..35. No new bounds/clamps or live-register inputs. */
uint8_t pharaoh_city_undo_storage_limit_exceeded(
    const PharaohBuildingTypeRecord *buildings,
    const PharaohBuildingStorageRecord *storages,
    int32_t building, int32_t amount, int32_t resource);

typedef struct PharaohCityUndoStorageContext {
    PharaohBuildingTypeRecord *buildings;
    const PharaohBuildingStorageRecord *storages;
    const int32_t *highest_building_id; /* 78D790 */
    const uint32_t *current_player; /* EAAC28 */
    PharaohStorageWithdrawContext *stock; /* Same buildings/counters/images. */
} PharaohCityUndoStorageContext;

int32_t pharaoh_city_undo_storage_deliver(const PharaohCityUndoStorageContext *,
    uint32_t player, int32_t building, int32_t resource, int32_t amount); /*45B2A0*/
int32_t pharaoh_city_undo_store_anywhere(const PharaohCityUndoStorageContext *,
    int32_t resource, int32_t requested, uint8_t count_only); /*45AFE0*/

typedef struct PharaohCityUndoMaintenanceContext {
    PharaohConstructionSupportContext *support;
    PharaohBuildingAllocationContext *allocation;
    const int32_t *highest_building_id; /* Actual signed DWORD78D790. */
} PharaohCityUndoMaintenanceContext;

void pharaoh_city_undo_storage_reset_links(
    const PharaohCityUndoMaintenanceContext *); /*46B320*/
void pharaoh_city_undo_ferry_reset_links(
    const PharaohCityUndoMaintenanceContext *); /*46F030*/
void pharaoh_city_undo_repair_reset_links(
    const PharaohCityUndoMaintenanceContext *); /*46A320*/
void pharaoh_city_undo_restore_houses(PharaohBuildingAllocationContext *,
    PharaohMapBuildingTilesContext *); /*485890*/
void pharaoh_city_undo_place_farm(const PharaohFarmTileDrawContext *,
    int32_t building, uint8_t extra); /*488E60, logical extra is BYTE*/
void pharaoh_city_undo_mausoleum_images(PharaohMausoleumImageContext *,
    int32_t building); /*4E0C80*/

typedef struct PharaohCityUndoPlayerExtras {
    int32_t *festival_x, *festival_y; /* EA6010/14 + player*49D8; DWORDs */
    uint32_t *recruiter_placed; /* EA5D20 + player*49D8 */
    const int16_t *sphinx_stage_before_delete; /* WORD EA6244 + player*49D8 */
    uint16_t *temple_complex_upgrades; /* WORD EA5CFA + player*49D8 */
    uint32_t *temple_god_bonus; /* DWORD EA6200 + player*49D8 */
} PharaohCityUndoPlayerExtras;

typedef struct PharaohCityUndoFormation {
    uint8_t state, faction, legion_id, is_at_fort;
    int16_t walker_type, building_id;
    uint8_t reserved_08[0x42 - 8], is_legion;
    uint8_t reserved_43[0x90 - 0x43];
} PharaohCityUndoFormation;

typedef struct PharaohCityUndoState {
    const uint32_t *ready; /* BC968C */
    uint32_t *available, *cost, *count; /* BC9694/BC9684/BC95F0 */
    const int32_t *saved_tool; /* BC9690 */
    const uint32_t *resources; /* BC95F4[36] */
    const int16_t *building_ids; /* BA746C[525] */
    const PharaohBuildingTypeRecord *building_backup; /* BA7888[525] */
} PharaohCityUndoState;

enum { PHARAOH_CITY_UNDO_GRID_CELLS = 51984 };
typedef struct PharaohCityUndoMaps {
    uint32_t *terrain, *images; /* AF598C/B7482C */
    uint8_t *canals, *bitfields, *sprites, *edges; /* AE8E7C/B285CC/ADC36C/B67D1C */
    const uint32_t *saved_terrain, *saved_images; /* 9DE62C/A1126C */
    const uint8_t *saved_canals, *saved_bitfields, *saved_sprites, *saved_edges;
    /* Saved BYTE layers:9D1B1C/9B84FC/9C500C/9AB9EC. All layers51984 cells. */
} PharaohCityUndoMaps;

typedef struct PharaohCityUndoLiveContext {
    const PharaohCityUndoState *state;
    const PharaohCityUndoMaps *maps;
    const PharaohCityUndoStorageContext *storage;
    const PharaohCityUndoMaintenanceContext *maintenance;
    PharaohConstructionLoadContext *load;
    const PharaohConstructionCommitContext *commit;
    PharaohConstructionSupportContext *support;
    const PharaohFarmTileDrawContext *farms;
    PharaohMausoleumImageContext *mausoleum;
    const PharaohCityUndoPlayerExtras *player_extras; /* one real view per player */
    const PharaohCityUndoFormation *formations; /* BCB9C8[50] */
    const PharaohConstructionSavedLegion *finished_legions; /* 6FDCBC[3] */
    PharaohConstructionSavedLegion *started_legions; /* 6FDCE4[3] */
    const PharaohRouteTilePlacementContext *roads;
    PharaohEditorTerrainKindState *terrain_kind;
    PharaohFerryContext *ferry;
    const PharaohPyramidDemolitionContext *pyramid;
    PharaohMapBuildingUndo *capture; /* actual485100 undo snapshot/IDs */
    uint32_t *paved_refresh_pending; /* 77567C */
    uint8_t *redraw; /* E38DCA */
} PharaohCityUndoLiveContext;

void pharaoh_city_undo_venue_remove(const PharaohCityUndoLiveContext *,
    int32_t building, int32_t kind); /*43FD00*/
void pharaoh_city_undo_gatehouse_remove(const PharaohCityUndoLiveContext *,
    int32_t building); /*4BFEE0*/
void pharaoh_city_undo_restore_building(const PharaohCityUndoLiveContext *,
    int32_t building); /*4858E0*/
void pharaoh_city_undo_temple_complex_create(const PharaohCityUndoLiveContext *,
    int32_t building,int32_t x,int32_t y,uint32_t owner,int32_t kind,
    uint32_t initialize); /*4C3420; actual construction provider*/
void pharaoh_city_undo_perform(const PharaohCityUndoLiveContext *); /*485230*/
void pharaoh_city_sidebar_undo(const PharaohCityUndoLiveContext *); /*406010*/

/* All contexts borrow the same live scene, pools, existing Native undo
   snapshots and immutable part descriptors. No world mirror is created.
   Normalized monument methods are reused through a single selected building
   and one-cell views; adapters call actual map/terrain C and write back only
   the exact Native BYTE/WORD/DWORD fields. Existing Native grid restoration
   copies remain part of the undo algorithm. The65..69 restore branch installs
   its actual4C3420 provider in a local470990 context view; no shared callback
   is replaced. capture is used by the complete4C3420 initialize!=0 branch.
   Maps, load/support/stock/maintenance/farm/monument/ferry/routing views and
   player_extras must alias the same original fields. All real providers are
   required; this API supplies no missing-child callbacks or zero defaults. */

#endif
