#ifndef PHARAOH_RENDERING_MONUMENT_SPOT_DRAW_H
#define PHARAOH_RENDERING_MONUMENT_SPOT_DRAW_H

#include "pharaoh/rendering/monument_spot_animation.h"
#include "pharaoh/building/type_queries.h"

typedef struct PharaohMonumentSpotDrawOperations {
    void *user_data;
    void (*draw_image)(void *user_data,int32_t image,int32_t x,int32_t y);
} PharaohMonumentSpotDrawOperations;

typedef struct PharaohMonumentSpotGroupDrawContext {
    const PharaohMonumentSpotDrawContext *draw;
    const PharaohMonumentWorkSpotBinding *sphinx;
    const PharaohMonumentWorkSpotBinding *sun_temple;
    const PharaohMonumentWorkSpotBinding *library;
    const PharaohMonumentWorkSpotBinding *abu_simbel;
    const PharaohMonumentWorkSpotBinding *caesareum;
    const PharaohMonumentWorkSpotBinding *lighthouse;
    const PharaohMonumentWorkSpotGroup *royal_tomb;
    const PharaohBuildingTypeRecord *buildings;
    int32_t (*royal_tomb_spot_table)(void *user_data,int32_t chamber_kind);
    const PharaohMonumentSpotDrawOperations *operations;
    int32_t *library_position_variant;
} PharaohMonumentSpotGroupDrawContext;

void pharaoh_sphinx_draw_work_spots(
    PharaohMonumentSpotGroupDrawContext *,int32_t x,int32_t y); /*0041FFF0*/
void pharaoh_sun_temple_draw_work_spots(
    PharaohMonumentSpotGroupDrawContext *,int32_t x,int32_t y); /*00420050*/
void pharaoh_library_draw_work_spots(
    PharaohMonumentSpotGroupDrawContext *,int32_t x,int32_t y); /*004206F0*/
void pharaoh_abu_simbel_draw_work_spots(
    PharaohMonumentSpotGroupDrawContext *,int32_t x,int32_t y); /*00420A30*/
void pharaoh_caesareum_draw_work_spots(
    PharaohMonumentSpotGroupDrawContext *,int32_t x,int32_t y); /*00420D60*/
void pharaoh_lighthouse_draw_work_spots(
    PharaohMonumentSpotGroupDrawContext *,int32_t x,int32_t y); /*00421060*/
void pharaoh_royal_tomb_draw_work_spots(
    PharaohMonumentSpotGroupDrawContext *,int32_t x,int32_t y,
    int32_t building_id); /*00421920*/

#endif
