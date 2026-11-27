#ifndef PHARAOH_UI_CITY_MAP_INPUT_H
#define PHARAOH_UI_CITY_MAP_INPUT_H
#include "pharaoh/monuments/pyramid_preview.h"
#include "pharaoh/runtime/city_view.h"

typedef struct PharaohCityMapInputContext {
    const PharaohCityViewState *view;
    const PharaohCityProjectionLookup *projection;
    PharaohCityPixelHit *hit;
    const int32_t *mouse_x,*mouse_y;
    int32_t *map_x,*map_y; /*BCA2C8/BCA2CC*/
    const uint8_t *selection_locked; /*605A5C*/
    const int32_t *construction_tool; /*6059E8*/
    const PharaohPyramidPreviewContext *preview;
} PharaohCityMapInputContext;

/* Complete00405F60/00405F80. Shared map origin, selection and anchor fields
   are borrowed directly. Actual view/projection and preview pools required. */
void pharaoh_pyramid_preview_update(const PharaohCityMapInputContext *);
void pharaoh_city_view_mouse_select(const PharaohCityMapInputContext *);
#endif
