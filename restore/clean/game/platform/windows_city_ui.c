#include "pharaoh/platform/windows_city_ui.h"
#include "pharaoh/platform/windows_startup_host.h"
#include "pharaoh/ui/city_chrome_draw.h"
#include "pharaoh/ui/button_border.h"
#include "pharaoh/ui/sidebar_content.h"
#include "pharaoh/ui/sidebar_layout.h"
#include "pharaoh/rendering/minimap_image.h"
#include "pharaoh/rendering/minimap_building.h"
#include "pharaoh/rendering/walker_overlay.h"
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
    PharaohImageButtonRecord controls[2],buildings[16],extra_buttons[2];
    PharaohImageButtonRecord collapsed_control[1],collapsed_buildings[12];
    PharaohSidebarClipPositions sidebar_positions;
    PharaohSidebarContent content;
    PharaohSidebarBackgroundState sidebar;
    PharaohSidebarBackgroundOperations sidebar_ops;
    PharaohImageButtonDrawState button_draw;
    PharaohGraphicsPrimitives graphics;
    PharaohMinimap minimap;
    PharaohMinimapImage minimap_images;
    PharaohMinimapBuilding minimap_buildings;
    PharaohMinimapWalkers minimap_walkers;
    PharaohWalkerOverlayContext walker_overlay;
    PharaohWindowsStartupHost *host;
    PharaohImageDrawContext *images;
    int32_t collapsed,caption,previous_caption,minimap_dirty,editor_dirty,editor_view;
    int32_t command_a,command_b,previous_message_count;
    uint8_t ui_redraw,show_all;
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
    memcpy(u->controls,native_sidebar_controls,sizeof(u->controls));
    memcpy(u->buildings,native_sidebar_buildings,sizeof(u->buildings));
    memcpy(u->extra_buttons,native_sidebar_extra,sizeof(u->extra_buttons));
    memcpy(u->collapsed_control,native_sidebar_collapsed_control,sizeof(u->collapsed_control));
    memcpy(u->collapsed_buildings,native_sidebar_collapsed_buildings,sizeof(u->collapsed_buildings));
    pharaoh_ui_layout_bind_stdio(&u->io);
    h->city_ui=u;
    return u;
}

static struct PharaohWindowsCityUi *bind_frame(PharaohWindowsStartupHost *h,
    PharaohImageDrawContext *images,const PharaohWindowsCityUiScene *scene,
    const uint8_t *window,const int32_t *groups)
{
    const PharaohSavedSceneStorage *const saved=scene->saved;
    const int32_t *const suppress_flood=&scene->view->event_54_suppress_flood;
    if(h->game_frame_locked==0 || h->native_force_refresh==NULL || *scene->overlay!=0 || *suppress_flood!=0) return NULL;
    struct PharaohWindowsCityUi *const u=open_ui(h);
    if(u==NULL) return NULL;
    for(int32_t i=0;i<8;++i) {
        const PharaohBitmapFont *const f=pharaoh_windows_startup_host_native_font(h,634+i);
        if(f==NULL || f->first_image==0) return NULL;
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
        .menu_flag_de1=&u->menu_flag_de1,.force_refresh=h->native_force_refresh,
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
    u->host=h;u->images=images;
    return u;
}


static const PharaohBitmapFont *font(struct PharaohWindowsCityUi *u,int32_t id)
{
    for(int32_t i=0;i<8;++i) if(u->fonts[i].first_image==(uint32_t)id) return &u->fonts[i];
    return NULL; /* Native callers require a valid loaded font ID. */
}
static void sprite(void *data,int32_t image,int32_t x,int32_t y)
{
    struct PharaohWindowsCityUi *u=data;
    u->host->bitmap_current_glyph=(uint32_t)image;
    u->images->clip->draw_x=x;u->images->clip->draw_y=y;
    (void)pharaoh_image_draw(u->images,image,x,y);
}
static PharaohGraphicsPrimitives *target(struct PharaohWindowsCityUi *u)
{
    const PharaohSpriteClipState *c=u->images->clip;
    u->graphics.clip_x0=c->clip_x_min;u->graphics.clip_x1=c->clip_x_max;
    u->graphics.clip_y0=c->clip_y_min;u->graphics.clip_y1=c->clip_y_max;
    return &u->graphics;
}
static void fill(void *p,int32_t x,int32_t y,int32_t w,int32_t h,int32_t color)
{pharaoh_graphics_fill_rect(target(p),x,y,w,h,color);}
static void mask(void *p,int32_t x,int32_t y,int32_t w,int32_t h,int32_t color)
{pharaoh_graphics_and_rect(target(p),x,y,w,h,(uint16_t)color);}
static void line(void *p,int32_t x,int32_t y,int32_t x1,int32_t y1,int32_t color)
{pharaoh_graphics_draw_line(target(p),x,y,x1,y1,color);}
static void view_rect(void *p,int32_t x,int32_t y,int32_t w,int32_t h,uint32_t color)
{pharaoh_graphics_draw_rect(target(p),x,y,w,h,(int32_t)color);}
static void image_buttons(void *p,int32_t x,int32_t y,PharaohImageButtonRecord *b,int32_t n)
{struct PharaohWindowsCityUi *u=p;pharaoh_image_buttons_draw(x,y,b,n,&u->button_draw);}
static void centered(void *p,int32_t group,int32_t entry,int32_t x,int32_t y,
    int32_t width,int32_t id,int32_t color,int32_t ox,int32_t oy)
{
    struct PharaohWindowsCityUi *u=p;
    pharaoh_ui_text_draw_centered_language(&u->text,font(u,id),group,entry,x,y,width,id,
        (uint16_t)color,ox,oy);
}
static void number_centered(void *p,int32_t value,int32_t prefix,int32_t x,
    int32_t y,int32_t width,int32_t id,int32_t color)
{
    struct PharaohWindowsCityUi *u=p;
    const PharaohBitmapTextCenteredNativeContext c={font(u,id),u->text.drawing,
        u->text.glyph_map,u->text.center_offset};
    pharaoh_bitmap_text_draw_number_centered_native(&c,u->text.number_text,value,
        (char)prefix," ",x,y,width,id,(uint16_t)color);
}

int32_t pharaoh_windows_city_ui_begin_frame(PharaohWindowsStartupHost *h,
    PharaohImageDrawContext *images,const PharaohWindowsCityUiScene *scene,
    const uint8_t *window,const int32_t *groups)
{
    struct PharaohWindowsCityUi *const u=bind_frame(h,images,scene,window,groups);
    if(u==NULL) return 0;
    const PharaohSavedSceneStorage *const saved=scene->saved;
    const int32_t *const editor=&scene->view->event_54_suppress_flood;
    pharaoh_sidebar_init_positions((uint32_t)h->graphics.render_width,&u->sidebar_positions);
    /* Build-menu views start at enabled(+18h), not at the image record.
       Read each live value; retain the renderer's own other mutable fields. */
    for(int32_t i=0;i<12;++i) {
        u->buildings[i].enabled=scene->build_menu->sidebar_buttons[0][i].enabled;
        u->collapsed_buildings[i].enabled=scene->build_menu->sidebar_buttons[1][i].enabled;
    }
    u->graphics=(PharaohGraphicsPrimitives){.display_width=h->graphics.render_width,
        .screen_height=h->graphics.render_height,.rgb565_pixels=(uint16_t *)(void *)images->surface_pixels,
        .rgb565_pitch_bytes=images->pitch_bytes};
    _Static_assert(sizeof(PharaohSpriteTagRecord)==8,"native tag stride");
    u->button_draw=(PharaohImageButtonDrawState){
        .sprite_group_bases=(const int32_t *)(const void *)&h->sprite_tags[0].image_index,
        .draw_x=&images->clip->draw_x,.draw_y=&images->clip->draw_y,
        .active_sprite_image=(int32_t *)(void *)&h->bitmap_current_glyph,
        .ui_redraw=&u->ui_redraw,.window_redraw_requested=&h->graphics.redraw_requested,
        .force_refresh=h->native_force_refresh,.draw_sprite=sprite,.user_data=u};
    u->editor_dirty=scene->view->state->redraw_requested;
    u->content=(PharaohSidebarContent){
        .collapsed=&u->collapsed,.sidebar_x=(const int32_t *)(const void *)&u->sidebar_positions.selector_other_right_edge,
        .collapsed_x=(const int32_t *)(const void *)&u->sidebar_positions.selector_one_right_edge,
        .category=scene->build_menu->selected_category,.category_image_base=&groups[386],
        .caption=&u->caption,.body_font=u->topbar.body_font,.origin_x=u->topbar.origin_x,.origin_y=u->topbar.origin_y,
        .previous_caption=&u->previous_caption,.ui_redraw=&u->ui_redraw,
        .command_a=&u->command_a,.command_b=&u->command_b,.command_button=&u->buildings[12],
        .expanded_controls=u->controls,.expanded_buildings=u->buildings,.expanded_extra=u->extra_buttons,
        .collapsed_control=u->collapsed_control,.collapsed_buildings=u->collapsed_buildings,
        .editor_active=editor,.editor_dirty=&u->editor_dirty,.map_dirty=&h->city_scroll_requested,
        .minimap_dirty=&u->minimap_dirty,.user_data=u,.draw_buttons=image_buttons,.draw_sprite=sprite,
        .draw_centered_language=centered,.draw_line=line,.window_id=window,.message_count=scene->message_count,
        .problem_count=scene->problem_count,.scenario_mode=&scene->build_menu->settings->is_custom_scenario,
        .small_font=(const int32_t *)(const void *)&h->sprite_tags[634].image_index,
        .previous_message_count=&u->previous_message_count,.message_button=&u->buildings[13],
        .problem_button=&u->buildings[14],.scenario_button=&u->buildings[15],.draw_number_centered=number_centered};
    u->minimap=(PharaohMinimap){.view=scene->view->state,.projection=scene->view->projection,.clip=images->clip,
        .terrain=saved->view.terrain,.buildings=saved->view.building_ids,.walkers=saved->view.walker_ids,
        .edges=saved->view.edges,.random=saved->view.random,.extra=saved->destinations[14],
        .pixel_mode=images->pixel_mode,.overlay=scene->overlay,.editor_active=editor,.editor_view=&u->editor_view,
        .color_variant=saved->view.metadata+0x3d8,.show_all=&u->show_all,
        .screen_width=images->screen_width,.screen_height=images->screen_height,
        .land=&groups[410],.rock=&groups[411],.trees=&groups[412],.water=&groups[414],.road=&groups[415],
        .building=&groups[416],.overlay_image=&groups[418],.garden=&groups[420],.black=&groups[421],
        .special=&groups[492],.terrain_02000000_image=&groups[480],.user_data=u,.draw_view_rect=view_rect};
    u->minimap_images=(PharaohMinimapImage){images->image_pixels,images->image_count,
        &images->state->source.pixels,&images->clip->draw_x,&images->clip->draw_y,&u->graphics};
    u->minimap_buildings=(PharaohMinimapBuilding){.buildings=saved->view.buildings,
        .footprints=saved->view.properties,.type_image_ids=native_minimap_type_images,
        .house_image=&groups[417],.overlay_image=&groups[418],.garden_image=&groups[420],
        .special_image=&groups[492],.building_image=&groups[416],.tint=scene->tint,
        .user_data=u,.draw_sprite=sprite};
    u->minimap_walkers=(PharaohMinimapWalkers){.walkers=saved->view.walkers,
        .formation_status=(const PharaohMinimapFormationStatus *)(const void *)
            ((const uint8_t *)saved->destinations[24]+130),.overlay=scene->overlay};
    u->walker_overlay=(PharaohWalkerOverlayContext){scene->overlay,saved->view.current_player,
        saved->view.walkers,saved->view.buildings,saved->view.players};
    pharaoh_minimap_bind_walker_overlay(&u->minimap_walkers,&u->walker_overlay);
    pharaoh_minimap_bind_walkers(&u->minimap,&u->minimap_walkers);
    pharaoh_minimap_bind_images(&u->minimap,&u->minimap_images);
    pharaoh_minimap_bind_buildings(&u->minimap,&u->minimap_buildings);
    pharaoh_minimap_bind_pixel_pair(&u->minimap,&u->graphics);
    pharaoh_sidebar_bind_minimap(&u->content,&u->minimap);
    u->sidebar=(PharaohSidebarBackgroundState){.screen_width=images->screen_width,.screen_height=images->screen_height,
        .hit_height=&h->graphics.hit_height,.sidebar_x=u->content.sidebar_x,.collapsed_x=u->content.collapsed_x,
        .collapsed=&u->collapsed,.editor_active=editor,.game_image_base=&groups[390],.editor_image_base=&groups[22],
        .body_font=u->topbar.body_font,.origin_x=u->topbar.origin_x,.origin_y=u->topbar.origin_y,.ui_redraw=&u->ui_redraw};
    /* The unsupported editor arm has no fabricated draw-city/editor callbacks. */
    u->sidebar_ops=(PharaohSidebarBackgroundOperations){.user_data=u,.draw_sprite=sprite,.fill_bottom=fill,.fill_editor=mask};
    pharaoh_sidebar_bind_content(&u->sidebar_ops,&u->content);
    pharaoh_sidebar_bind_topbar(&u->sidebar_ops,&u->topbar);
    if(h->graphics.redraw_requested!=0) {
        pharaoh_sidebar_draw_background(&u->sidebar,&u->sidebar_ops);
        h->graphics.redraw_requested=0; /* native004D71C0 state1 */
    }
    return 1;
}

void pharaoh_windows_city_ui_draw_foreground(PharaohWindowsStartupHost *h)
{
    struct PharaohWindowsCityUi *const u=h->city_ui;
    pharaoh_sidebar_draw_buttons(&u->content);
    pharaoh_sidebar_draw_caption(&u->content,0,*u->content.sidebar_x+11);
    pharaoh_sidebar_draw_minimap(&u->content,0);
    u->ui_redraw=0; /* native004D75D0 finish, after the selected foreground children */
    pharaoh_city_draw_topbar(&u->topbar,0);
    pharaoh_sidebar_draw_message_count(&u->content,1);
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
