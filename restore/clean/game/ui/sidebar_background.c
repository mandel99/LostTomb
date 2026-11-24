#include "pharaoh/ui/sidebar_background.h"

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}

void pharaoh_sidebar_draw_right_edge800(const PharaohSidebarBackgroundState *s,
    const PharaohSidebarBackgroundOperations *o)
{o->draw_sprite(o->user_data,add(*s->game_image_base,3),760,0);}

void pharaoh_sidebar_draw_right_edge1024(const PharaohSidebarBackgroundState *s,
    const PharaohSidebarBackgroundOperations *o)
{o->draw_sprite(o->user_data,add(*s->game_image_base,2),1000,0);}

/* Typed reuse of Unit02344.c; live Ghidra0051E990 confirms branch order,
 * exact force arguments and editor's city redraw between fill and sprites.
 * Keep state pointer-backed: child calls share the original live globals. */
void pharaoh_sidebar_draw_background(const PharaohSidebarBackgroundState *s,
    const PharaohSidebarBackgroundOperations *o)
{
    void *u=o->user_data;
    void *content=o->content_user_data!=0?o->content_user_data:u;
    if(*s->screen_width==1024) pharaoh_sidebar_draw_right_edge1024(s,o);
    else if(*s->screen_width==800) pharaoh_sidebar_draw_right_edge800(s,o);
    const int32_t height=*s->screen_width==1024?9:6;
    o->fill_bottom(u,0,add(*s->screen_height,-height),*s->screen_width,height,0);
    *s->ui_redraw=1;
    if(*s->editor_active==0) {
        int32_t image=*s->game_image_base,x=*s->sidebar_x;
        if(*s->collapsed!=0) {image=add(image,1);x=*s->collapsed_x;}
        o->draw_sprite(u,image,x,30);
        o->draw_buttons(content);
        o->draw_0051ed30(content,1,add(*s->sidebar_x,11));
        o->draw_0051edd0(content,0,add(*s->sidebar_x,11));
        o->draw_0051efb0(content,1);
        if(*s->screen_width==1024) {
            if(*s->collapsed==0) o->draw_sprite(u,add(*s->game_image_base,4),*s->sidebar_x,480);
            else if(*s->collapsed==1) o->draw_sprite(u,add(*s->game_image_base,5),*s->collapsed_x,480);
        } else if(*s->screen_width==800) {
            if(*s->collapsed==0) o->draw_sprite(u,add(*s->game_image_base,6),*s->sidebar_x,480);
            else if(*s->collapsed==1) o->draw_sprite(u,add(*s->game_image_base,7),*s->collapsed_x,480);
        }
    } else {
        o->fill_editor(u,add(*s->sidebar_x,6),24,
            add(add(*s->screen_width,-*s->sidebar_x),-6),add(*s->hit_height,-24),0);
        o->draw_city(u);
        o->draw_sprite(u,*s->editor_image_base,*s->sidebar_x,24);
        if(*s->screen_width==1024) o->draw_sprite(u,add(*s->editor_image_base,2),*s->sidebar_x,480);
        else if(*s->screen_width==800) o->draw_sprite(u,add(*s->editor_image_base,1),*s->sidebar_x,480);
        o->draw_editor(u);
        o->draw_editor_buttons(u,*s->sidebar_x,24,15);
        o->draw_0051efb0(content,1);
        o->draw_language(u,44,132,add(*s->sidebar_x,11),152,*s->body_font,0,*s->origin_x,*s->origin_y);
        o->draw_language(u,2,0,add(*s->sidebar_x,94),152,*s->body_font,0,*s->origin_x,*s->origin_y);
    }
    o->draw_0051f0a0(u,1);
    o->draw_0051f680(content,1);
}
