#include "pharaoh/ui/sidebar_content.h"

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}

/* Native0051EC90, existing Unit02345 algorithm. This is button drawing;
 * the minimap is the distinct0051EFB0 child. */
void pharaoh_sidebar_draw_buttons(PharaohSidebarContent *c)
{
    c->command_button->enabled=(uint8_t)(*c->command_a!=0 && *c->command_b!=0);
    if(*c->collapsed==0) {
        c->draw_buttons(c->user_data,*c->sidebar_x,30,c->expanded_controls,2);
        c->draw_buttons(c->user_data,*c->sidebar_x,30,c->expanded_buildings,16);
        c->draw_buttons(c->user_data,*c->sidebar_x,30,c->expanded_extra,2);
    } else if(*c->collapsed==1) {
        c->draw_buttons(c->user_data,*c->collapsed_x,30,c->collapsed_control,1);
        c->draw_buttons(c->user_data,*c->collapsed_x,30,c->collapsed_buildings,12);
    }
}

/* Native0051ED30; caption cache changes only when a visible draw is issued. */
void pharaoh_sidebar_draw_caption(PharaohSidebarContent *c,int32_t force,int32_t x)
{
    int32_t redraw=*c->caption==*c->previous_caption?force:1;
    if(*c->ui_redraw!=0) redraw=1;
    if(*c->collapsed==0 && redraw!=0) {
        *c->previous_caption=*c->caption;
        c->draw_centered_language(c->user_data,*c->caption==0?6:14,
            *c->caption==0?4:*c->caption,x,34,117,*c->body_font,65535,*c->origin_x,*c->origin_y);
    }
}

/* Native0051EDD0, exact category permutation from the original branches. */
void pharaoh_sidebar_draw_category(PharaohSidebarContent *c,int32_t force,int32_t x)
{
    static const int32_t offsets[13]={0,1,9,5,10,4,7,11,3,8,6,12,2};
    if(force!=0 || *c->collapsed==0) {
        const int32_t category=*c->category;
        const int32_t offset=(uint32_t)category<13u?offsets[category]:0;
        c->draw_sprite(c->user_data,add(*c->category_image_base,offset),x,211);
    }
}

/* Native0051EFB0; consume dirty only while expanded, editor tests ==1
 * for invalidation and ==0 for normal placement are deliberately distinct. */
void pharaoh_sidebar_draw_minimap(PharaohSidebarContent *c,int32_t force)
{
    if(*c->collapsed!=0) return;
    int32_t redraw=*c->minimap_dirty!=0;
    if(redraw) *c->minimap_dirty=0;
    if(*c->editor_active==1 && *c->editor_dirty!=0) redraw=1;
    if(*c->map_dirty!=0) redraw=1;
    if(force==0 && redraw==0) return;
    if(*c->editor_active==0) {
        c->draw_minimap(c->user_data,add(*c->sidebar_x,13),59,73,111);
        return;
    }
    c->draw_minimap(c->user_data,add(*c->sidebar_x,8),36,73,111);
    c->draw_line(c->user_data,add(*c->sidebar_x,7),35,add(*c->sidebar_x,153),35,0x4208);
    c->draw_line(c->user_data,add(*c->sidebar_x,7),36,add(*c->sidebar_x,7),147,0x4208);
    c->draw_line(c->user_data,add(*c->sidebar_x,153),36,add(*c->sidebar_x,153),147,0xc618);
}
