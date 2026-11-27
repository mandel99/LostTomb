#include "pharaoh/monuments/pyramid_preview.h"
#include <stddef.h>

_Static_assert(offsetof(PharaohPyramidTilePlacement,terminal_tile)==24,
    "native EAAE9C placement result");

static int32_t signed_bits(uint32_t bits)
{
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

/* Reuse of complete old mixed Unit02086 with genuine clean C children. */
uint8_t pharaoh_pyramid_preview_is_allowed(const PharaohPyramidPreviewContext *c,
    uint32_t center,int32_t tool,uint32_t *failure_flags)
{
    PharaohMapSurfaceContext *const surface=c->surface;
    const int32_t samples=*surface->preview_count;
    const int32_t half=*surface->orientation/2;
    uint8_t valid=1;
    *failure_flags=0;
    uint32_t style=*surface->pyramid_style;
    for(int32_t row=0;;++row){
        const int32_t count=signed_bits(style==5?
            c->style_five_piece_counts[tool]:c->piece_counts[tool]);
        if(row>=count) break;
        uint32_t cell;
        if(style==5) cell=(uint32_t)c->style_five_piece_xy[row][1]*228u+
            (uint32_t)c->style_five_piece_xy[row][0];
        else cell=(uint32_t)c->piece_offsets[row];
        cell+=center;
        for(int32_t n=0;n<samples;++n){
            const int32_t linear=signed_bits(cell);
            if(pharaoh_map_grid_size_reaches_outside(surface->geometry,
                linear%228,linear/228,3)) return 0;
            const uint32_t index=(uint32_t)n+(uint32_t)half*36u;
            const uint32_t tile=cell+(uint32_t)c->sample_offsets[index];
            if(surface->map->terrain[tile]&UINT32_C(0xfeffd76e)){
                valid=0;*failure_flags|=1u;
            }
        }
        if(samples>0) style=*surface->pyramid_style;
    }
    if(pharaoh_pyramid_has_complex_temple(signed_bits(*surface->pyramid_style),
        surface->pyramid_complex_offsets,tool)){
        const int32_t relative=signed_bits(
            (uint32_t)surface->pyramid_complex_offsets[tool]+center-
            (uint32_t)surface->map->origin);
        pharaoh_pyramid_map_segment_update(surface,relative%228,relative/228,0,0);
        if(surface->pyramid_placement->terminal_tile==0){
            valid=0;*failure_flags|=2u;
        }
    }
    style=*surface->pyramid_style;
    const int32_t count=signed_bits(style==5?
        c->style_five_strip_counts[tool]:c->strip_counts[tool]);
    uint32_t tile=center+(uint32_t)(style==5?
        c->style_five_strip_offsets[tool]:c->strip_offsets[tool]);
    for(int32_t n=0;n<count;++n,++tile){
        if(surface->map->terrain[tile]&UINT32_C(0xfeffd72e)){
            valid=0;*failure_flags|=4u;
        }
    }
    return valid;
}

uint8_t pharaoh_pyramid_preview_try_tile(const PharaohPyramidPreviewContext *c,
    uint32_t tile)
{
    uint32_t failure_flags;
    if(pharaoh_pyramid_preview_is_allowed(c,tile,*c->tool,&failure_flags)==0)
        return 1;
    const int32_t relative=signed_bits(tile-(uint32_t)c->surface->map->origin);
    *c->anchor_tile=tile;
    *c->anchor_x=relative%228;
    *c->anchor_y=relative/228;
    return 0;
}

static uint32_t visit(uint32_t tile,int32_t x,int32_t y,void *context)
{
    /* Native callback ignores both coordinates and its physical fourth arg.
       Only AL is consumed by all four ring call sites. */
    (void)x;(void)y;
    return pharaoh_pyramid_preview_try_tile(context,tile);
}

void pharaoh_pyramid_preview_find_place(const PharaohPyramidPreviewContext *c)
{
    const int32_t tool=*c->tool;
    const int32_t radius=c->radius_by_tool[tool];
    pharaoh_map_visit_diamond_rings(c->surface->geometry,
        signed_bits(*c->selected_tile),radius,visit,(void *)c);
}
