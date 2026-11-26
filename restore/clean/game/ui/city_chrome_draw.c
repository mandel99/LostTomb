#include "pharaoh/ui/city_chrome_draw.h"
#include "pharaoh/runtime/text_loading.h"
#include <stddef.h>
#include <string.h>

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static const PharaohBitmapFont *font(const PharaohCityChromeDrawContext *c,int32_t id)
{
    for(uint32_t i=0;i<c->font_count;++i)
        if(c->fonts[i].first_image==(uint32_t)id) return &c->fonts[i];
    return NULL; /* The native caller requires the actual loaded font. */
}

void pharaoh_city_menu_bar_draw(PharaohCityChromeDrawContext *c,
    PharaohMenuBarFocusRecord *record,int32_t count)
{
    PharaohBitmapTextDrawState *const state=c->text->drawing;
    PharaohTextSelection *const selection=c->text->selection;
    *c->menu_flag_de1=1;
    *c->menu_flag_de0=1;
    state->cursor_x=record->left;
    if(count<1) return;
    uint32_t index=1;
    do {
        const int32_t group=record->reserved_06;
        const int32_t y=record->top;
        record->left=(int16_t)state->cursor_x;
        const int32_t x=record->left;
        pharaoh_text_select(selection,group,0);
        /* Native fonts F56F84/F56F8C are tags634/635. The views retain
           their actual first-image IDs, not those tag indices. */
        const PharaohBitmapFont *const small=&c->fonts[0];
        const PharaohBitmapFont *const normal=&c->fonts[1];
        if(index==*c->selected_menu) {
            state->outline=1;
            (void)pharaoh_bitmap_text_width_native(small,
                (const uint8_t *)selection->current,c->text->glyph_map,state->current_glyph);
            pharaoh_bitmap_text_draw_selection_native(normal,state,selection,
                group,0,x,y,(int32_t)normal->first_image,0xfac1);
            state->outline=0;
        } else pharaoh_bitmap_text_draw_selection_native(normal,state,selection,
            group,0,x,y,(int32_t)normal->first_image,0);
        record->right=(int16_t)state->cursor_x;
        state->cursor_x=add(state->cursor_x,10);
        ++record;++index;
    } while((int32_t)index<=count);
}

void pharaoh_city_menu_button_draw(PharaohCityChromeDrawContext *c)
{
    const PharaohImageButtonRecord *const b=c->menu_button;
    *c->draw_x=b->x;
    *c->active_image=(uint32_t)c->sprite_group_bases[b->image_group];
    *c->draw_y=b->y;
    if(b->activation_ticks!=0) {
        uint32_t variant;
        memcpy(&variant,b->reserved_1c,sizeof(variant)); /* native DWORD at+1C */
        if(b->activation_ticks==20) *c->force_refresh=1;
        *c->active_image+=variant+4u;
    } else if(b->pressed!=0) {
        uint32_t variant;
        memcpy(&variant,b->reserved_1c,sizeof(variant));
        *c->active_image+=variant+1u;
    }
    (void)pharaoh_image_draw(c->images,(int32_t)*c->active_image,*c->draw_x,*c->draw_y);
}

void pharaoh_city_header_background_draw(PharaohCityChromeDrawContext *c)
{
    int32_t x;
    if(*c->screen_width==640) x=-360;
    else if(*c->screen_width==800) x=-240;
    else if(*c->screen_width==1024) x=0;
    else return;
    (void)pharaoh_image_draw(c->images,add(*c->header_image_base,8),x,0);
}

static void text(void *p,const char *s,int32_t x,int32_t y,int32_t id,int32_t color)
{
    PharaohCityChromeDrawContext *c=p;
    c->text->drawing->font_id=(uint32_t)id;
    pharaoh_bitmap_text_draw_native(font(c,id),c->text->drawing,
        (const uint8_t *)s,x,y,(uint16_t)color);
}
static void background(void *p,int32_t x,int32_t width)
{(void)x;(void)width;pharaoh_city_header_background_draw(p);}
static void buttons(void *p,int32_t editor,int32_t count)
{
    PharaohCityChromeDrawContext *c=p;
    pharaoh_city_menu_bar_draw(c,editor?c->editor_menu:c->game_menu,count);
}
static void language(void *p,int32_t group,int32_t entry,int32_t x,int32_t y,
    int32_t id,int32_t color,int32_t ox,int32_t oy)
{
    PharaohCityChromeDrawContext *c=p;
    pharaoh_ui_text_draw_language(c->text,font(c,id),group,entry,x,y,id,
        (uint16_t)color,ox,oy);
}
static void number(void *p,int32_t value,int32_t prefix,int32_t x,int32_t y,
    int32_t id,int32_t color,int32_t ox,int32_t oy)
{
    PharaohCityChromeDrawContext *c=p;
    pharaoh_ui_text_draw_number(c->text,font(c,id),value,(char)prefix," ",x,y,id,
        (uint16_t)color,ox,oy);
}
static void year(void *p,int32_t mode,int32_t value,int32_t x,int32_t y,int32_t id,int32_t color)
{
    PharaohCityChromeDrawContext *c=p;
    pharaoh_bitmap_text_draw_year_native(font(c,id),c->text->drawing,c->text->selection,
        c->text->number_text,mode,value,x,y,id,(uint16_t)color);
}
static void menu_button(void *p) {pharaoh_city_menu_button_draw(p);}
void pharaoh_city_topbar_bind_drawing(PharaohCityTopbar *bar,PharaohCityChromeDrawContext *c)
{
    bar->menu_button_x=&c->menu_button->x;
    bar->text_advance=&c->text->drawing->cursor_x;
    bar->screen_width=c->screen_width;
    bar->user_data=c;bar->draw_text=text;bar->draw_background=background;
    bar->draw_buttons=buttons;bar->draw_language=language;bar->draw_number=number;
    bar->draw_year=year;bar->draw_menu_button=menu_button;
}

_Static_assert(sizeof(PharaohMenuBarFocusRecord)==16 &&
    offsetof(PharaohMenuBarFocusRecord,reserved_06)==6 &&
    offsetof(PharaohImageButtonRecord,activation_ticks)==25 &&
    offsetof(PharaohImageButtonRecord,pressed)==26 &&
    offsetof(PharaohImageButtonRecord,reserved_1c)==28,"native top-menu records");
