#ifndef PHARAOH_UI_BUILDING_INFO_INIT_BINDINGS_H
#define PHARAOH_UI_BUILDING_INFO_INIT_BINDINGS_H

#include "pharaoh/ui/building_info_content_live.h"
#include "pharaoh/ui/walker_phrase_live.h"
#include "pharaoh/map/road_access.h"
#include "pharaoh/map/grid_geometry.h"
#include "pharaoh/runtime/city_warning_clear.h"

/* Actual EADB38..EADCB8 storage immediately before the 224-byte info state.
 * Borrow this block; do not clear it. Several native reports leave slots alone. */
typedef struct PharaohBuildingInfoInitReports {
    int32_t royal_tomb[7];                 /* EADB38, shared by four tombs */
    int32_t reserved_1c;
    int32_t mausoleum[12];                /* EADB58 */
    int32_t lighthouse[8];                /* EADB88 */
    int32_t caesareum[8];                 /* EADBA8 */
    int32_t abu_simbel[4];                /* EADBC8 */
    int32_t library[14];                  /* EADBD8 */
    int32_t sun_temple[14];               /* EADC10 */
    PharaohPyramidProgressReport pyramid;/* EADC48 */
} PharaohBuildingInfoInitReports;

enum PharaohBuildingInfoInitDivideFault {
    PHARAOH_INFO_INIT_DIVIDE_BY_ZERO,
    PHARAOH_INFO_INIT_SIGNED_DIVIDE_OVERFLOW
};

/* REQUIRED synchronous, non-returning native #DE endpoint. native_entry is
 * 004CDE60 or 0053C100; kind identifies the reported IDIV failure, not a
 * fabricated return value. Raise the platform's native divide exception and
 * leave this call permanently, even if a platform exception handler resumes.
 * Do not queue an exception, return a status, or return to the initializer.
 * C11 cannot put _Noreturn on a function-pointer typedef; the adapter enforces
 * this contract with abort() only if the supplied endpoint incorrectly returns. */
typedef void (*PharaohBuildingInfoInitDivideFaultEndpoint)(void *user_data,
    uint32_t native_entry,enum PharaohBuildingInfoInitDivideFault kind);

/* All pointers are borrowed, including the initializer and its actual UI.
 * The existing providers must be fully initialized for their native domains.
 * Scalar fields held by value in those contexts (warning width, road origin,
 * receiver parts, occupancy origin) must reflect the live state at invocation.
 * No pools, report blocks, RNG, event state, or saved profile banks are copied.
 * road_result_x/y are the genuine D3B4E4/E0 DWORDs, distinct from the shared
 * D3B4FC/F8 distance scratch used by house/phrase providers. */
typedef struct PharaohBuildingInfoInitProviders {
    PharaohBuildingInfoLiveInitContext *initializer;
    const PharaohCityWarningClearContext *warnings;
    const PharaohResourceAvailabilityContext *resources;
    const PharaohInfoPyramidSelectionContext *pyramid_selection;
    PharaohRoadAccessContext *road;
    int32_t *road_result_x,*road_result_y;
    const PharaohInfoWorstDesirabilityContext *house_worst;
    const PharaohHouseEconomyContext *house_economy;
    const PharaohWalkerPhraseContext *phrase;
    const PharaohInfoAdvisorButtonsContext *advisors;
    PharaohPyramidSummaryContext *pyramid_progress;
    PharaohComplexProgressContext *complex_progress;
    PharaohBaseMonumentProgress *library,*caesareum,*lighthouse;
    /* Native receivers 6F9320,6F5648,6DE9E0,6E9188, respectively. */
    PharaohBaseMonumentProgress *royal_tombs[4];
    PharaohBuildingInfoInitReports *reports;
    void *divide_fault_user_data;
    PharaohBuildingInfoInitDivideFaultEndpoint divide_fault;
} PharaohBuildingInfoInitProviders;

enum PharaohBuildingInfoInitSupport {
    PHARAOH_INFO_INIT_READY,
    PHARAOH_INFO_INIT_MISSING_CONTEXT,
    PHARAOH_INFO_INIT_MISSING_DATA,
    PHARAOH_INFO_INIT_STATE_MISMATCH,
    PHARAOH_INFO_INIT_MISSING_FAULT_ENDPOINT,
    PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN
};

typedef struct PharaohBuildingInfoInitResult {
    enum PharaohBuildingInfoInitSupport support;
    uint32_t native_entry;
    const char *field; /* static diagnostic label; NULL when ready */
    int32_t opened;   /* actual 4F7E50 logical result, only when ready */
} PharaohBuildingInfoInitResult;

typedef struct PharaohBuildingInfoInitBindings {
    PharaohBuildingInfoInitProviders providers;
    PharaohBuildingInfoLiveInitOperations operations;
} PharaohBuildingInfoInitBindings;

/* Installs 25 genuine adapters in initializer->operations. Keep bindings at
 * a stable address, and keep all borrowed data alive. Call the checked entry
 * below; availability/alias refusal happens before ANY initializer stores.
 * Readiness requires the complete provider set for this all-branches factory,
 * including contexts for presently unselected monument types. It does not
 * establish array bounds or replace the existing providers' native contracts. */
void pharaoh_building_info_init_bindings_bind(PharaohBuildingInfoInitBindings *,
    const PharaohBuildingInfoInitProviders *);
PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_support(
    const PharaohBuildingInfoInitBindings *);
PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_initialize(
    const PharaohBuildingInfoInitBindings *);

/* Checked subset of the SAME complete4F7E50 initializer. These are actual
 * scene map/geometry owners, not caller-invented bounds or a copied UI state.
 * Cursor coordinates come from initializer.cursor_tile_x/y. Their Native
 * grid_start must match map.origin, and map.terrain must be the same grid.
 *
 * Exact supported domain: editor_active!=1; pending_warning_count<=0;
 * cursor inside the actual map rectangle/diamond and the228x228 allocation;
 * selected building grid WORD==0; real record0.building_type==0; all nine
 * actual signedWORD neighbour heads<=0, with every index in that allocation.
 * This proves no special building/progress/details/road/house/phrase/advisor
 * child or military walker selection is reached. No sentinel fields are
 * changed. The native terrain classifier, including its retain-old-kind arm,
 * and positioning still execute in the whole initializer, not in a dry-run.
 *
 * Only genuine5596D0 resources and the existing pure4F8960 mode1 positioning
 * path are reached. Missing unrelated provider contexts and #DE endpoint do
 * not block this subset. Resources must be the actual scene producer: output
 * counts/lists alias ui; profile flags/outputs do not alias cursor/grid heads,
 * record0, geometry, warning/editor fields or info control state. Supplied
 * arrays have their original allocated/sentinel backing. Game-thread inputs
 * stay stable during the synchronous support+initialize call; resource child
 * writes only its documented counts/lists and profile known flags.
 *
 * Refusal is read-only. initialize_empty_terrain repeats the pure guard and
 * invokes4F7E50 once. Editor/warning reset arms are explicitly refused rather
 * than replaced. The original all-branches APIs above remain unchanged. */
typedef struct PharaohBuildingInfoEmptyTerrainInitDomain {
    const PharaohMapTerrainView *map;
    const PharaohMapGridGeometry *geometry;
} PharaohBuildingInfoEmptyTerrainInitDomain;

PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_empty_terrain_support(
    const PharaohBuildingInfoInitBindings *,const PharaohBuildingInfoEmptyTerrainInitDomain *);
PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_initialize_empty_terrain(
    const PharaohBuildingInfoInitBindings *,const PharaohBuildingInfoEmptyTerrainInitDomain *);

#endif
