#ifndef PHARAOH_COMMERCE_EXPORT_KEEP_H
#define PHARAOH_COMMERCE_EXPORT_KEEP_H

#include "pharaoh/monuments/progress_reports.h"
#include "pharaoh/scenario/saved_game_initialize.h"
#include "pharaoh/events/event_queries.h"

typedef struct PharaohMonumentResourceDemand {
    PharaohComplexProgressContext *complex;
    PharaohPyramidSummaryContext *pyramid;
    PharaohScenarioPlayerRuntime *players;
    const int32_t *current_player;
    const int16_t *planned_kinds; /*three native WORDs*/
} PharaohMonumentResourceDemand;

typedef struct PharaohExportKeepContext {
    PharaohSavedGameInitializeContext *initialize;
    PharaohMonumentResourceDemand *monuments;
    PharaohScenarioEvent *events;
    const int32_t *event_count, *highest_building;
    const uint32_t *produced, *consumed; /*36 DWORDs, indexed by resource*/
    const uint32_t *categories; /*six native categories*/
    const uint32_t (*population_limits)[6]; /*four population bands*/
    int32_t *export_keep; /*36 DWORDs*/
} PharaohExportKeepContext;

int32_t pharaoh_monument_resources_needed(PharaohMonumentResourceDemand *c,
    int32_t resource);
void pharaoh_empire_update_export_keep(PharaohExportKeepContext *c);
void pharaoh_saved_game_bind_export_keep(
    PharaohSavedGameInitializeOperations *ops, PharaohExportKeepContext *c);

#endif
