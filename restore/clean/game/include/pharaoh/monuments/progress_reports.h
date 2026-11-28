#ifndef PHARAOH_MONUMENTS_PROGRESS_REPORTS_H
#define PHARAOH_MONUMENTS_PROGRESS_REPORTS_H

#include "pharaoh/monuments/pyramid_work_queries.h"

_Static_assert(sizeof(PharaohBuildingTypeRecord)==0x108 &&
    offsetof(PharaohBuildingTypeRecord,monument_worker_id)==0x5c &&
    offsetof(PharaohBuildingTypeRecord,pyramid_worker_id_64)==0x64 &&
    sizeof(PharaohWalkerQueryRecord)==0x184 &&
    offsetof(PharaohWalkerQueryRecord,type)==0x0a &&
    offsetof(PharaohWalkerQueryRecord,state)==0x0e &&
    offsetof(PharaohWalkerQueryRecord,action_state_before_6_7)==0x10 &&
    offsetof(PharaohWalkerQueryRecord,destination_grid_offset)==0x28 &&
    offsetof(PharaohWalkerQueryRecord,action_state)==0x38 &&
    offsetof(PharaohWalkerQueryRecord,leading_walker_id)==0x68,
    "native monument worker record ABI");

typedef struct PharaohComplexPartDefinition {
    uint32_t kind;
    uint8_t reserved_04[48];
} PharaohComplexPartDefinition;

typedef struct PharaohComplexProgressContext {
    PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    const PharaohComplexPartDefinition *mausoleum_parts, *sun_temple_parts;
    const int32_t *reference_ids;
    const int16_t *const *reference_targets; /*legacy twelve-WORD records, walker at[9]*/
    const int16_t *const *reference_walker_ids; /*optional actual walker_id WORD fields,13 pointers*/
} PharaohComplexProgressContext;

typedef struct PharaohPyramidPartLayoutEntry {
    int32_t kind, layers;
} PharaohPyramidPartLayoutEntry;

/* Native112-byte report. The UI token's8-field normalized view is separate. */
typedef struct PharaohPyramidProgressReport {
    int32_t phase, height, heightNext, layerCount;
    int32_t mainPlaced, casingPlaced, frontierMain, frontierCasing;
    int32_t mainRemaining, casingRemaining, mainTotal, CasingTotal;
    int32_t phasePercent, percent, prepMinStage, prepMaxStage, prepDone, prepTotal;
    int32_t stonemasons, bricklayers, waitingMain, waitingCasing, carpenters;
    int32_t waiting, blockWorkers, laborers, materialMain, materialCasing;
} PharaohPyramidProgressReport;

typedef struct PharaohPyramidSummaryContext {
    PharaohBuildingTypeRecord *buildings;
    const PharaohWalkerQueryRecord *walkers;
    const int16_t *tile_building_ids;
    const int32_t *style;
    int32_t *frontier_count, *frontier_parts; /*144 entries*/
    const int32_t *part_counts, *mastaba_part_counts;
    const int32_t *layer_counts, *bent_layer_counts, *mastaba_layer_counts;
    const int32_t *bent_casing_totals;
    const int32_t *block_totals, *stepped_block_totals;
    const int32_t *bent_block_totals, *mastaba_block_totals;
    const int32_t *prep_totals, *mastaba_prep_totals;
    const PharaohPyramidPartLayoutEntry (*pyramid_layout)[144];
    const PharaohPyramidPartLayoutEntry (*mastaba_layout)[36];
    /* Native BYTE layer index may reach following layout bytes through255. */
    const uint8_t *bent_half_layer_bytes;
    const int32_t *descriptor_counts;
    const PharaohPyramidWorkDescriptor *descriptors;
} PharaohPyramidSummaryContext;

uint8_t pharaoh_pyramid_worker_claim_active(PharaohPyramidSummaryContext *c,
    int32_t building);
void pharaoh_pyramid_build_progress_report(PharaohPyramidSummaryContext *c,
    int32_t building, PharaohPyramidProgressReport *out);
int32_t pharaoh_mausoleum_progress(PharaohComplexProgressContext *c,
    int32_t building, int32_t out[12]);
int32_t pharaoh_sun_temple_progress(PharaohComplexProgressContext *c,
    int32_t building, int32_t out[14]);

#endif
