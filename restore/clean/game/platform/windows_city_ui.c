#include "pharaoh/platform/windows_city_ui.h"
#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/ui/city_chrome_draw.h"
#include "pharaoh/ui/button_border.h"
#include <stdlib.h>
#include <string.h>

#include "windows_city_ui_defaults.inc"

struct PharaohWindowsCityUi {
    PharaohUiLayoutState layout;
    PharaohUiLayoutIo io;
    PharaohUiLayoutTextContext layout_text;
    PharaohUiLayoutContext layout_context;
    PharaohGenericButton label_buttons[30]; /* initialize writes right only */
    PharaohLayoutHighlightState highlight;
    PharaohLayoutHighlightDrawing highlight_drawing;
    PharaohUiTextLayoutContext text;
    PharaohBitmapFont fonts[8];
    PharaohImageGlyphDrawBinding glyph;
    PharaohCityChromeDrawContext chrome;
    PharaohCityTopbar topbar;
    PharaohMenuBarFocusRecord game_menu[4],editor_menu[4];
    PharaohImageButtonRecord menu_button;
    /* These process globals have pinned initial value0; no saved copies. */
    int32_t previous_month,previous_treasury,previous_population;
    int32_t treasury_x,population_x,date_x,redraw_mode;
    int32_t window_seven_key;
    uint32_t selected_menu;
    uint8_t menu_flag_de0,menu_flag_de1;
};

static struct PharaohWindowsCityUi *open_ui(PharaohWindowsStartupHost *h)
{
    if(h->city_ui!=NULL) return h->city_ui;
    struct PharaohWindowsCityUi *const u=calloc(1,sizeof(*u));
    if(u==NULL) return NULL;
    memcpy(u->game_menu,native_game_menu,sizeof(u->game_menu));
    memcpy(u->editor_menu,native_editor_menu,sizeof(u->editor_menu));
    u->menu_button=native_menu_button;
    pharaoh_ui_layout_bind_stdio(&u->io);
    h->city_ui=u;
    return u;
}

int32_t pharaoh_windows_city_ui_draw_topbar(PharaohWindowsStartupHost *h,
    PharaohImageDrawContext *images,const PharaohSavedSceneStorage *saved,
    const uint8_t *window,const int32_t *suppress_flood,const int32_t *groups)
{
    if(h->game_frame_locked==0) return 0;
    struct PharaohWindowsCityUi *const u=open_ui(h);
    if(u==NULL) return 0;
    for(int32_t i=0;i<8;++i) {
        const PharaohBitmapFont *const f=pharaoh_windows_startup_host_native_font(h,634+i);
        if(f==NULL || f->first_image==0) return 0;
        u->fonts[i]=*f;
        u->glyph=(PharaohImageGlyphDrawBinding){images,&h->text_input.drawing.color};
        pharaoh_image_bind_bitmap_font(&u->fonts[i],&u->glyph);
    }
    u->layout_text=(PharaohUiLayoutTextContext){&h->text_selection,&u->fonts[1],
        pharaoh_bitmap_native_glyph_map(),&h->bitmap_current_glyph,&h->text_selection.current};
    u->layout_context=(PharaohUiLayoutContext){&u->layout,&u->layout_text,
        u->label_buttons,&u->io};
    pharaoh_ui_layout_bind_highlight(&u->highlight,&u->highlight_drawing,images,
        &groups[401],&groups[443],&h->bitmap_current_glyph,
        &images->clip->draw_x,&images->clip->draw_y);
    u->text=(PharaohUiTextLayoutContext){&u->layout_context,&u->highlight,
        window,&u->window_seven_key,&h->text_selection,&h->text_input.drawing,
        pharaoh_bitmap_native_glyph_map(),&h->text_selection.current,
        &h->bitmap_center_offset,&h->text_input.drawing.reset_flag,h->bitmap_text_buffers};
    u->chrome=(PharaohCityChromeDrawContext){
        .images=images,.text=&u->text,.fonts=u->fonts,.font_count=8,
        .screen_width=&h->graphics.render_width,.header_image_base=&groups[390],
        .sprite_group_bases=groups,.game_menu=u->game_menu,.editor_menu=u->editor_menu,
        .selected_menu=&u->selected_menu,.menu_flag_de0=&u->menu_flag_de0,
        .menu_flag_de1=&u->menu_flag_de1,.force_refresh=&h->graphics.redraw_requested,
        .menu_button=&u->menu_button,.draw_x=&images->clip->draw_x,
        .draw_y=&images->clip->draw_y,.active_image=&h->bitmap_current_glyph};
    u->topbar=(PharaohCityTopbar){
        .window_id=window,.editor_active=suppress_flood,.screen_width=&h->graphics.render_width,
        .current_player=saved->view.current_player,.players=saved->view.players,
        .month=saved->destinations[36],.year=saved->destinations[37],
        .body_font=(const int32_t *)(const void *)&h->sprite_tags[635].image_index,
        .negative_font=(const int32_t *)(const void *)&h->sprite_tags[638].image_index,
        .heading_font=(const int32_t *)(const void *)&h->sprite_tags[637].image_index,
        .origin_x=&h->graphics.ui_origin_x,.origin_y=&h->graphics.ui_origin_y,.scenario_name=saved->destinations[129],
        .previous_population=&u->previous_population,.previous_treasury=&u->previous_treasury,
        .previous_month=&u->previous_month,.redraw_mode=&u->redraw_mode,
        .treasury_x=&u->treasury_x,.population_x=&u->population_x,.date_x=&u->date_x};
    pharaoh_city_topbar_bind_drawing(&u->topbar,&u->chrome);
    /* Native004D6B60 invokes0051F0A0(0) after foreground and input. */
    pharaoh_city_draw_topbar(&u->topbar,0);
    return 1;
}

void pharaoh_windows_city_ui_release(PharaohWindowsStartupHost *h)
{
    struct PharaohWindowsCityUi *const u=h->city_ui;
    if(u==NULL) return;
    for(int32_t i=0;i<120;++i) for(int32_t list=0;list<3;++list) {
        PharaohUiLayoutNode *node=u->layout.entries[i].lists[list];
        while(node!=NULL) {
            PharaohUiLayoutNode *const next=node->next;
            u->io.release(u->io.user_data,node);
            node=next;
        }
    }
    free(u);h->city_ui=NULL;
}
