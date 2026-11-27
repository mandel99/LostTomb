#include "pharaoh/ui/city_map_input.h"

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

void pharaoh_pyramid_preview_update(const PharaohCityMapInputContext *c)
{
    if(*c->construction_tool==0xb7)
        pharaoh_pyramid_preview_find_place(c->preview);
}

/* Complete old mixed Unit00048 algorithm; all children now actual typed C. */
void pharaoh_city_view_mouse_select(const PharaohCityMapInputContext *c)
{
    const PharaohPyramidPreviewContext *const preview=c->preview;
    *c->map_y=0;*c->map_x=0;
    const uint32_t cell=(uint32_t)pharaoh_city_view_mouse_to_grid_offset(c->view,
        c->projection,c->hit,*c->mouse_x,*c->mouse_y);
    *preview->selected_tile=cell;
    if(cell==0) return;
    const int32_t relative=signed_bits(cell-(uint32_t)preview->surface->map->origin);
    const int32_t x=relative%228,y=relative/228;
    *c->map_x=x;
    const uint8_t locked=*c->selection_locked;
    *c->map_y=y;
    if(locked!=0) return;
    *preview->anchor_tile=cell;*preview->anchor_x=x;*preview->anchor_y=y;
    pharaoh_pyramid_preview_update(c);
}
