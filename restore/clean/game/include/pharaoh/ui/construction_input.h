#ifndef PHARAOH_UI_CONSTRUCTION_INPUT_H
#define PHARAOH_UI_CONSTRUCTION_INPUT_H
#include "pharaoh/building/construction_undo.h"
#include "pharaoh/map/construction_routes.h"

typedef struct PharaohConstructionInputContext {
    const uint32_t *anchor_tile;
    const int32_t *anchor_x,*anchor_y,*map_origin;
    const uint8_t *input_locked,*bytes_ab623c;
    int32_t *start_y,*end_y,*start_x,*end_x; /*6059D4/D8/DC/E0*/
    uint8_t *byte_605798,*current_window,*redraw;
    uint32_t *first_tile,*initial_cell_byte,*drag_started; /*6059D0/BC96CC/6059E4*/
    int32_t *tool,*ferry_state; /*6059E8/BCA310*/
    const int32_t *ferry_first_building; /*BC9800*/
    PharaohBuildingTypeRecord *buildings;
    const PharaohConstructionUndoContext *undo;
    const PharaohConstructionRouteContext *routes;
} PharaohConstructionInputContext;

/* Complete00405C20/00405BA0, reusing real undo/routes/ferry C. Every shared
   pointer aliases its actual native owner; no default routing/cancel action. */
void pharaoh_construction_begin(const PharaohConstructionInputContext *);
uint32_t pharaoh_construction_cancel_live(const PharaohConstructionInputContext *);
#endif
