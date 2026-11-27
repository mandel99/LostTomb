#ifndef PHARAOH_UI_CONSTRUCTION_FINISH_H
#define PHARAOH_UI_CONSTRUCTION_FINISH_H

#include "pharaoh/ui/construction_input.h"
#include "pharaoh/building/construction_commit.h"
#include "pharaoh/editor/terrain_refresh.h"

typedef struct PharaohConstructionFinishContext {
    const PharaohConstructionInputContext *input;
    const PharaohConstructionCommitContext *commit;
    PharaohEditorTerrainContext *editor;
    uint32_t *selection_tile;       /* BCA2C4; distinct from BCA300. */
    const uint32_t *fallback_tile;   /* BCA2E4. */
    const int32_t *orientation;      /* BCA2E0. */
} PharaohConstructionFinishContext;

/* Complete00405E90. All borrowed contexts refer to the same live scene.
 * Native result is void; status only propagates a real child IDIV fault. */
PharaohPercentageScaleStatus pharaoh_construction_finish(
    const PharaohConstructionFinishContext *);

#endif
