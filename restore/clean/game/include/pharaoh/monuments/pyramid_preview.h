#ifndef PHARAOH_MONUMENTS_PYRAMID_PREVIEW_H
#define PHARAOH_MONUMENTS_PYRAMID_PREVIEW_H
#include "pharaoh/map/surface_tiles.h"
#include "pharaoh/map/terrain_queries.h"

typedef struct PharaohPyramidPreviewContext {
    PharaohMapSurfaceContext *surface;
    const int32_t *tool; /*5E6970*/
    const uint32_t *piece_counts,*style_five_piece_counts; /*56F7D8/56F7EC*/
    const int32_t *piece_offsets; /*56FE30,144 entries*/
    const int32_t (*style_five_piece_xy)[2]; /*56FD10,36 pairs*/
    const int32_t *sample_offsets; /*5CFE10, four orientations x36*/
    const uint32_t *strip_counts,*style_five_strip_counts; /*570094/5700A8*/
    const int32_t *strip_offsets,*style_five_strip_offsets; /*5700B4/5700C8*/
    const int32_t *radius_by_tool; /*5F5D1C*/
    uint32_t *selected_tile,*anchor_tile; /*BCA2C4/BCA300*/
    int32_t *anchor_x,*anchor_y; /*BCA304/308*/
} PharaohPyramidPreviewContext;

/* Complete004ECEC0, logical AL result. Every known native consumer tests AL
   or NEG AL; incidental EAX high bits are not exposed as a C result.
   Surface fields alias actual style5E4EF4, orientationBCA2E0, count5CFCDC,
   map originBCA2D8, complex offsets5F5D08 and placementEAAE84. */
uint8_t pharaoh_pyramid_preview_is_allowed(const PharaohPyramidPreviewContext *,
    uint32_t center,int32_t tool,uint32_t *failure_flags);
/* Complete callback004ED2D0 and parent004ED330. The ring bridge calls this
   actual C predicate, then publishes the chosen anchor; no placeholder. */
uint8_t pharaoh_pyramid_preview_try_tile(const PharaohPyramidPreviewContext *,
    uint32_t tile);
void pharaoh_pyramid_preview_find_place(const PharaohPyramidPreviewContext *);
#endif
