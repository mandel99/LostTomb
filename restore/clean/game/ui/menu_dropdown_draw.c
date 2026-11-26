#include "pharaoh/ui/menu_dropdown_draw.h"
#include "pharaoh/core/static_objects.h"

static int16_t word(uint32_t bits)
{
    const uint16_t low=(uint16_t)bits;
    return low<=INT16_MAX?(int16_t)low:(int16_t)((int32_t)low-65536);
}
static int32_t add(int32_t a,int32_t b)
{
    const uint32_t bits=(uint32_t)a+(uint32_t)b;
    return bits<=INT32_MAX?(int32_t)bits:
        (int32_t)((int64_t)bits-INT64_C(0x100000000));
}

/* Complete old mixed Unit00397 algorithm with explicit observed cells. */
void pharaoh_menu_dropdown_draw(const PharaohMenuDropdownDrawContext *c)
{
    if(*c->menu->current_window!=3 || *c->menu->selected_header==0) return;
    const PharaohMenuBarFocusRecord *const header=*c->menu->active_header;
    *c->current_item=*c->header_items;
    const int16_t count=*c->header_count;
    const int32_t x=header->left,y=header->top,group=header->reserved_06;
    *c->scratch_height=word((uint32_t)(int32_t)count*20u+20u);
    *c->scratch_width=10;
    const int16_t width=word((uint32_t)pharaoh_native_metric_select_004d5ef0(
        group,c->menu_widths));
    *c->scratch_width=width;
    pharaoh_panel_pattern_fill_native(c->panel,x,add(y,18),width,
        (int32_t)*c->scratch_height/16);
    for(int32_t index=1;index<=count;++index) {
        const int32_t row_y=add(add((*c->current_item)->relative_y,y),30);
        const int32_t entry=(*c->current_item)->text_entry;
        if((uint32_t)index==*c->menu->hovered_item) {
            c->drawing->outline=1;
            *c->scratch_height=16;
            *c->scratch_width=width;
            pharaoh_bitmap_text_draw_selection_native(c->highlight_font,c->drawing,
                c->selection,group,entry,add(x,8),row_y,
                (int32_t)c->highlight_font->first_image,0xfac1);
            c->drawing->outline=0;
        } else pharaoh_bitmap_text_draw_selection_native(c->normal_font,c->drawing,
            c->selection,group,entry,add(x,8),row_y,
            (int32_t)c->normal_font->first_image,0);
        ++*c->current_item;
    }
}
