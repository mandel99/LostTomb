#include "pharaoh/platform/windows_city_ui.h"
#include "pharaoh/scenario/mission_scene.h"
#include "pharaoh/ui/building_info_live.h"
#include "pharaoh/ui/building_info_industry_live.h"
#include "pharaoh/ui/building_info_program_data.h"
#include "pharaoh/ui/building_info_action_bindings.h"
#include "pharaoh/ui/building_info_draw_bindings.h"
#include "pharaoh/ui/building_info_services_live.h"
#include "pharaoh/platform/windows_game_input.h"
#include "pharaoh/platform/windows_startup_host.h"
#include <mmsystem.h>
#include "pharaoh/ui/city_chrome_draw.h"
#include "pharaoh/ui/button_border.h"
#include "pharaoh/ui/city_input_actions.h"
#include "pharaoh/ui/city_input_program_data.h"
#include "pharaoh/ui/city_input_action_bindings.h"
#include "pharaoh/ui/window_state_actions.h"
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
    PharaohBuildingInfoLiveState info_state;
    PharaohBuildingInfoLiveContext info;
    PharaohBuildingInfoLiveInitContext info_initialize;
    PharaohInfoIndustryLiveContext industry;
    PharaohInfoServicesLiveContext services;
    PharaohSquareButtonRecord info_tax_arrows[2];
    PharaohBuildingInfoProgramData info_program;
    PharaohBuildingInfoActionBindings info_actions;
    PharaohBuildingInfoActionsLiveContext info_action_world;
    PharaohUnitCommandUiContext info_units;
    PharaohNativeLegionOrderContext info_orders;
    PharaohBuildingInfoInitBindings info_init_bindings;
    PharaohBuildingInfoDrawBindings info_draw_bindings;
    PharaohInfoPreviewContext info_previews;
    PharaohInfoWalkerContentContext info_walkers;
    PharaohInfoTerrainContentContext info_terrain;
    PharaohInfoProgramBorrowed info_borrowed;
    PharaohStorageInfoDrawContext info_drawing;
    PharaohInfoLayoutDrawContext info_layout;
    PharaohPanelFrameNativeContext info_inner,info_outer;
    PharaohBitmapWrappedNativeContext info_wrapped;
    PharaohBitmapTextCatalogWrappedNativeContext info_catalog;
    PharaohInfoContentDrawContext info_content;
    PharaohInfoSquareDrawContext info_square_drawing;
    PharaohSquareButtonInputState info_square_input;
    PharaohImageButtonInputState info_image_input;
    PharaohButtonHitState info_image_focus;
    PharaohGenericButtonInputState info_generic_input;
    PharaohGenericButtonFocusState info_generic_focus;
    PharaohButtonLayoutState info_button_layout;
    PharaohButtonLayoutOperations info_button_layout_operations;
    int32_t info_focus_primary,info_focus_secondary,info_focus_building;
    PharaohFoodSupplyQueryContext info_food;
    PharaohMapHouseWaterContext info_water;
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
    PharaohCityInputProgramData input_program;
    PharaohCityIngameButtonTables input_tables;
    PharaohCityInputActionBindings input_actions;
    PharaohImageButtonInputState city_image_input;
    PharaohGenericButtonInputState city_generic_input;
    PharaohCitySidebarActionContext sidebar_action;
    PharaohSoundEffectRequestState sidebar_sound;
    PharaohDigitalSoundDeviceContext sidebar_sound_device;
    PharaohCityViewContext sidebar_view;
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
    const PharaohCityViewContext *view;
    int32_t collapsed,caption,previous_caption,minimap_dirty,editor_dirty,editor_view;
    int32_t command_a,command_b,previous_message_count;
    uint8_t ui_redraw,show_all;
    /* These process globals have pinned initial value0; no saved copies. */
    int32_t previous_month,previous_treasury,previous_population;
    int32_t treasury_x,population_x,date_x,redraw_mode;
    int32_t window_seven_key;
    uint32_t selected_menu;
    int32_t navigation_hover,navigation_x,navigation_y;
    int32_t sidebar_slide_step;
    int32_t keyboard_field,image_focus,arrow_focus,menu_focus;
    int32_t ui_mouse_x,ui_mouse_y,mouse_drag_state;
    uint32_t modal_active,modal_return_window;
    uint8_t *current_window;
    uint8_t menu_flag_de0,menu_flag_de1;
};

/* Pinned DWORD table at5F8260, including its final zero sentinel. */
static const int32_t sidebar_slide_offsets[48]={
    1,2,3,4,5,6,7,8,10,12,14,16,18,21,24,27,30,33,37,41,45,49,
    54,59,64,70,76,83,91,99,106,113,119,125,130,135,139,143,146,
    149,152,154,156,158,160,162,165,0};

static struct PharaohWindowsCityUi *open_ui(PharaohWindowsStartupHost *h)
{
    if(h->city_ui!=NULL) return h->city_ui;
    struct PharaohWindowsCityUi *const u=calloc(1,sizeof(*u));
    if(u==NULL) return NULL;
    memcpy(u->game_menu,native_game_menu,sizeof(u->game_menu));
    memcpy(u->editor_menu,native_editor_menu,sizeof(u->editor_menu));
    /* Pinned5C8D38 two mutable tax-arrow rows; tokens are never executed
     * as process addresses.40A260 is an explicit pending input binding. */
    const PharaohSquareButtonRecord tax[2]={{180,70,18,24,0x0040a260,0,0,0,4,1,0},
        {204,70,16,24,0x0040a260,0,0,0,4,0,0}};
    memcpy(u->info_tax_arrows,tax,sizeof(tax));
    if(pharaoh_city_input_program_data_initialize_once(&u->input_program,
        pharaoh_city_input_action_bindings_resolve_compiled,&u->input_actions)!=
        PHARAOH_CITY_INPUT_PROGRAM_READY){free(u);return NULL;}
    pharaoh_city_input_program_data_bind_tables(&u->input_program,&u->input_tables);
    pharaoh_ui_layout_bind_stdio(&u->io);
    h->city_ui=u;
    return u;
}

static struct PharaohWindowsCityUi *bind_frame(PharaohWindowsStartupHost *h,
    PharaohImageDrawContext *images,const PharaohWindowsCityUiScene *scene,
    uint8_t *window,const int32_t *groups)
{
    const PharaohSavedSceneStorage *const saved=scene->saved;
    const int32_t *const editor_active=&scene->view->event_54_suppress_flood;
    if(h->game_frame_locked==0 || h->native_force_refresh==NULL || *scene->overlay!=0 || *editor_active!=0) return NULL;
    struct PharaohWindowsCityUi *const u=open_ui(h);
    if(u==NULL || scene->text_metrics==NULL) return NULL;
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
        .menu_button=u->input_program.compass,.draw_x=&images->clip->draw_x,
        .draw_y=&images->clip->draw_y,.active_image=&h->bitmap_current_glyph};
    u->topbar=(PharaohCityTopbar){
        .window_id=window,.editor_active=editor_active,.screen_width=&h->graphics.render_width,
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
    u->host=h;u->images=images;u->current_window=window;
    u->sidebar_view=*scene->view;u->view=&u->sidebar_view;
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

static uint32_t info_clock(void *data)
{(void)data;return timeGetTime();}
/* Byte window actually indexed at5C9268+repeat8..47, not a guessed DWORD
 * global type. Leading8 bytes are retained from pinned Ghidra, not used. */
static const uint8_t info_repeat_schedule[48]={144,145,92,0,6,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,1,0,0,1,1,1,1,1,1,1,1,0};

static void info_layout_panel(void *data,int32_t x,int32_t y,int32_t width,int32_t height)
{
    struct PharaohWindowsCityUi *const u=data;
    pharaoh_panel_frame_draw_native(&u->info_outer,x,y,width,height);
}
static void info_layout_border(void *data,int32_t style,int32_t x,int32_t y,
    int32_t width,int32_t height,uint32_t color)
{
    struct PharaohWindowsCityUi *const u=data;
    pharaoh_button_border_draw(&u->highlight_drawing.border,style,x,y,width,height,color);
}

/* Genuine shared layout/text/image owners for4F8E60/4F94A0 children.
 * Program callbacks and full window dispatch are installed separately only
 * after every reached provider has its real world/platform context. */
static void bind_info_drawing(struct PharaohWindowsCityUi *u,
    const PharaohWindowsCityUiScene *scene,const int32_t *groups)
{
    PharaohWindowsStartupHost *const h=u->host;
    PharaohImageDrawContext *const im=u->images;
    PharaohBuildingInfoLiveContext *const info=&u->info;
    info->display_mode=&h->settings.display_mode;
    info->text_width=&h->text_input.drawing.cursor_x;
    info->button_parameter=&h->button_parameter;
    info->focus_primary=&u->info_focus_primary;
    info->focus_secondary=&u->info_focus_secondary;
    info->focus_building=&u->info_focus_building;
    /* Native tag baseF55BB4,stride8. These are loaded SG3 values. */
    info->resource_images=&groups[713];info->panel_button_images=&groups[284];
    info->font_black=&groups[635];info->font_green=&groups[636];
    info->font_white=&groups[637];info->font_red=&groups[638];info->font_title=&groups[640];
    if(h->game_input!=NULL) {
        info->right_released=&h->game_input->mouse.right_button_released;
        if(h->game_input->files_initialized)
            info->sound_channels=(const int32_t *)(const void *)&h->game_input->building_sounds[0].roam;
    }
    info->sound_enabled=&h->settings.sound_effects_enabled;
    info->sound_volume=&h->settings.sound_effects_volume;
    u->info_inner=(PharaohPanelFrameNativeContext){im,&groups[402],&h->bitmap_current_glyph,&im->clip->draw_x,&im->clip->draw_y};
    u->info_outer=(PharaohPanelFrameNativeContext){im,&groups[401],&h->bitmap_current_glyph,&im->clip->draw_x,&im->clip->draw_y};
    u->info_layout=(PharaohInfoLayoutDrawContext){&u->layout_context,u->text.window_id,
        &u->window_seven_key,&u->highlight,&u->highlight_drawing.border,&u->info_inner,&u->info_outer};
    /* These are actual SG3 globals, also required when a diagnostic or a
     * previous zero-line briefing did not invoke rich-text drawing. */
    scene->text_metrics->images=h->sprite_images;
    scene->text_metrics->image_count=h->loaded_libraries->next_image;
    scene->text_metrics->large_font=(uint32_t)groups[634]; /* nativeF56F84 */
    scene->text_metrics->borrowed_line_buffer=(uint8_t *)(void *)(h->bitmap_text_buffers+356);
    u->info_wrapped=(PharaohBitmapWrappedNativeContext){&u->fonts[1],&h->text_input.drawing,
        scene->text_metrics,(uint8_t *)(void *)(h->bitmap_text_buffers+356),
        &scene->text_metrics->maximum_line_width,(uint32_t)*info->font_black};
    u->info_catalog=(PharaohBitmapTextCatalogWrappedNativeContext){&h->text_selection,
        &u->info_wrapped,&h->text_selection.current};
    const uint8_t *const player=(const uint8_t *)scene->saved->view.players+
        (uint32_t)*scene->saved->view.current_player*0x49d8u;
    u->info_drawing=(PharaohStorageInfoDrawContext){.ui=info,.layout=&u->info_layout,
        .text=&u->text,.wrapped=&u->info_catalog,.font_black=&u->fonts[1],
        .font_green=&u->fonts[2],.font_white=&u->fonts[3],.font_red=&u->fonts[4],.font_title=&u->fonts[6],
        .models=u->info_initialize.models,.labor=(const PharaohLaborCategory *)(const void *)(player+0x2b2c),
        .city_population=(const int32_t *)(const void *)(player+0x24),
        .kingdom_supplies_grain=u->info_initialize.editor_active,.context_icons=&groups[403],
        .number_suffix="",.employee_suffix=" "}; /* pinned7027D4/57C048 */
    u->info_previews.ui=info;
    u->info_content=(PharaohInfoContentDrawContext){&u->info_drawing,
        pharaoh_mission_scene_occupancy(scene->scene),&u->info_previews};
    u->info_walkers=(PharaohInfoWalkerContentContext){.content=&u->info_content,.image_buttons=&u->button_draw};
    u->info_terrain=(PharaohInfoTerrainContentContext){&u->info_walkers,NULL};
    u->industry.content=&u->info_content;u->industry.percent_suffix="%";
    u->info_square_drawing=(PharaohInfoSquareDrawContext){im,&h->bitmap_current_glyph,
        &im->clip->draw_x,&im->clip->draw_y,&u->ui_redraw};
    u->services=(PharaohInfoServicesLiveContext){.drawing=&u->info_drawing,
        .players=(const PharaohInfoServicesPlayerRecord *)(const void *)scene->saved->view.players,
        .current_player=(const uint32_t *)(const void *)scene->saved->view.current_player,
        .month=scene->saved->destinations[36],.square_buttons=&u->info_square_drawing,
        .tax_arrows=u->info_tax_arrows,.space_suffix=" ",.percent_suffix="%"};
    u->info_button_layout.actual_entries_by_key=u->layout.entries;
    u->info_button_layout.window_key=*info->current_window;
    u->info_button_layout.window_seven_layout_base=u->window_seven_key;
    u->info_button_layout.highlight_mode=u->highlight.edit_mode;
    u->info_button_layout_operations=(PharaohButtonLayoutOperations){u,info_layout_panel,info_layout_border};
    u->info_borrowed=(PharaohInfoProgramBorrowed){.display_mode=info->display_mode,
        .focus_primary=info->focus_primary,.focus_secondary=info->focus_secondary,.focus_building=info->focus_building,
        .button_parameter=info->button_parameter,.button_secondary_parameter=&h->button_secondary_parameter,
        .text_width=info->text_width,.resource_images=info->resource_images,.panel_button_images=info->panel_button_images,
        .font_black=info->font_black,.font_green=info->font_green,.font_white=info->font_white,
        .font_red=info->font_red,.font_title=info->font_title,.tutorial_flags=info->tutorial_flags,
        .phrase_counts=info->phrase_counts,.sound_channels=info->sound_channels,
        .sound_volume=info->sound_volume,.sound_enabled=info->sound_enabled,.square_input=&u->info_square_input,
        .image_input=&u->info_image_input,.image_focus=&u->info_image_focus,.generic_input=&u->info_generic_input,
        .generic_focus=&u->info_generic_focus,.button_layout=&u->info_button_layout,
        .button_layout_operations=&u->info_button_layout_operations};
}

static int32_t bind_info_inputs(struct PharaohWindowsCityUi *u,const PharaohWindowsCityUiScene *scene)
{
    PharaohBuildingInfoLiveContext *const i=&u->info;
    PharaohWindowsStartupHost *const h=u->host;
    if(h->game_input==NULL)return 0;
    if(u->info_square_input.clock==NULL)u->info_square_input.clock=info_clock;
    u->info_square_input.repeat_schedule=info_repeat_schedule;
    u->info_orders.redraw_units=i->current_window;u->info_orders.redraw_city=i->redraw_requested;
    u->info_units.orders=&u->info_orders;u->info_units.orders_open=(uint32_t *)(void *)&i->window->orders_shown;
    u->info_units.panel_mode=(const uint32_t *)(const void *)&i->window->mode;
    u->info_action_world=(PharaohBuildingInfoActionsLiveContext){.window=i->window,.walkers=i->walkers,
        .players=(const PharaohBuildingInfoActionPlayer *)(const void *)scene->saved->view.players,
        .current_player=(const uint32_t *)(const void *)scene->saved->view.current_player,
        .button_parameter=(const uint32_t *)(const void *)i->button_parameter,
        .current_window=i->current_window,.redraw=i->redraw_requested,
        .phrase_bank=pharaoh_mission_scene_info_phrase_bank(scene->scene)};
    const PharaohBuildingInfoActionProviders p={.ui=i,.live=&u->info_borrowed,
        .actions=&u->info_action_world,.units=&u->info_units,
        .current_player=u->info_action_world.current_player,
        .selected_buildings=(PharaohInfoSelectedBuildingPlayer *)(void *)
            ((uint8_t *)scene->saved->view.players+0x4674)};
    if(pharaoh_building_info_action_bindings_bind(&u->info_actions,&p).support!=PHARAOH_INFO_ACTION_READY)return 0;
    if(u->info_program.initialized==0&&pharaoh_building_info_program_data_initialize_once(&u->info_program,
        pharaoh_building_info_action_bindings_resolve_compiled,NULL).support!=PHARAOH_INFO_PROGRAM_READY)return 0;
    return pharaoh_building_info_program_data_bind(&u->info_program,i,&u->info_borrowed).support==PHARAOH_INFO_PROGRAM_READY;
}

static void primitive_pixel(void *data,int32_t x,int32_t y,uint16_t color)
{pharaoh_graphics_set_pixel_raw(data,x,y,color);}

int32_t pharaoh_windows_city_ui_begin_frame(PharaohWindowsStartupHost *h,
    PharaohImageDrawContext *images,const PharaohWindowsCityUiScene *scene,
    uint8_t *window,const int32_t *groups)
{
    struct PharaohWindowsCityUi *const u=bind_frame(h,images,scene,window,groups);
    if(u!=NULL)u->view=scene->view;
    if(u==NULL) return 0;
    const PharaohSavedSceneStorage *const saved=scene->saved;
    const int32_t *const editor=&scene->view->event_54_suppress_flood;
    u->info.window=&u->info_state;
    pharaoh_mission_scene_bind_building_info_world(scene->scene,&u->info,&u->info_initialize);
    pharaoh_mission_scene_bind_info_industry_world(scene->scene,&u->industry,&u->info_food,&u->info_water);
    bind_info_drawing(u,scene,groups);
    pharaoh_sidebar_init_positions((uint32_t)h->graphics.render_width,&u->sidebar_positions);
    /* Build-menu views start at enabled(+18h), not at the image record.
       Read each live value; retain the renderer's own other mutable fields. */
    for(int32_t i=0;i<12;++i) {
        u->input_program.expanded_build[i].enabled=scene->build_menu->sidebar_buttons[0][i].enabled;
        u->input_program.collapsed_build[i].enabled=scene->build_menu->sidebar_buttons[1][i].enabled;
    }
    u->graphics=(PharaohGraphicsPrimitives){.display_width=h->graphics.render_width,
        .screen_height=h->graphics.render_height,.rgb565_pixels=(uint16_t *)(void *)images->surface_pixels,
        .rgb565_pitch_bytes=images->pitch_bytes,.write_pixel=primitive_pixel,.user_data=&u->graphics};
    _Static_assert(sizeof(PharaohSpriteTagRecord)==8,"native tag stride");
    u->button_draw=(PharaohImageButtonDrawState){
        .sprite_group_bases=(const int32_t *)(const void *)&h->sprite_tags[0].image_index,
        .draw_x=&images->clip->draw_x,.draw_y=&images->clip->draw_y,
        .active_sprite_image=(int32_t *)(void *)&h->bitmap_current_glyph,
        .ui_redraw=&u->ui_redraw,.window_redraw_requested=&h->graphics.redraw_requested,
        .force_refresh=h->native_force_refresh,.draw_sprite=sprite,.user_data=u};
    if(!bind_info_inputs(u,scene))return 0;
    u->content=(PharaohSidebarContent){
        .collapsed=&u->collapsed,.sidebar_x=(const int32_t *)(const void *)&u->sidebar_positions.selector_other_right_edge,
        .collapsed_x=(const int32_t *)(const void *)&u->sidebar_positions.selector_one_right_edge,
        .category=scene->build_menu->selected_category,.category_image_base=&groups[386],
        .caption=&u->caption,.body_font=u->topbar.body_font,.origin_x=u->topbar.origin_x,.origin_y=u->topbar.origin_y,
        .previous_caption=&u->previous_caption,.ui_redraw=&u->ui_redraw,
        .command_a=&u->command_a,.command_b=&u->command_b,.command_button=&u->input_program.expanded_build[12],
        .expanded_controls=u->input_program.expanded_extra,.expanded_buildings=u->input_program.expanded_build,
        .expanded_extra=u->input_program.expanded_top,.collapsed_control=u->input_program.collapsed_expand,
        .collapsed_buildings=u->input_program.collapsed_build,
        .editor_active=editor,.editor_dirty=&u->editor_dirty,.map_dirty=&h->city_scroll_requested,
        .minimap_dirty=&u->minimap_dirty,.user_data=u,.draw_buttons=image_buttons,.draw_sprite=sprite,
        .draw_centered_language=centered,.draw_line=line,.window_id=window,.message_count=scene->message_count,
        .problem_count=scene->problem_count,.scenario_mode=&scene->build_menu->settings->is_custom_scenario,
        .small_font=(const int32_t *)(const void *)&h->sprite_tags[634].image_index,
        .previous_message_count=&u->previous_message_count,.message_button=&u->input_program.expanded_build[13],
        .problem_button=&u->input_program.expanded_build[14],.scenario_button=&u->input_program.expanded_build[15],
        .draw_number_centered=number_centered};
    const PharaohMinimap previous_minimap=u->minimap;
    u->minimap=(PharaohMinimap){.view=scene->view->state,.projection=scene->view->projection,.clip=images->clip,
        .terrain=saved->view.terrain,.buildings=saved->view.building_ids,.walkers=saved->view.walker_ids,
        .edges=saved->view.edges,.random=saved->view.random,.extra=saved->destinations[14],
        .pixel_mode=images->pixel_mode,.overlay=scene->overlay,.editor_active=editor,.editor_view=&u->editor_view,
        .color_variant=saved->view.metadata+0x3d8,.show_all=&u->show_all,
        .screen_width=images->screen_width,.screen_height=images->screen_height,
        .land=&groups[410],.rock=&groups[411],.trees=&groups[412],.water=&groups[414],.road=&groups[415],
        .building=&groups[416],.overlay_image=&groups[418],.garden=&groups[420],.black=&groups[421],
        .special=&groups[492],.terrain_02000000_image=&groups[480],.user_data=u,.draw_view_rect=view_rect};
    /* 43EF10 and406190 borrow the persistent draw scratch, just as native. */
    u->minimap.origin_x=previous_minimap.origin_x;u->minimap.origin_y=previous_minimap.origin_y;
    u->minimap.scan_x=previous_minimap.scan_x;u->minimap.scan_y=previous_minimap.scan_y;
    u->minimap.left=previous_minimap.left;u->minimap.right=previous_minimap.right;
    u->minimap.top=previous_minimap.top;u->minimap.bottom=previous_minimap.bottom;
    u->minimap.tile=previous_minimap.tile;u->minimap.pixel_x=previous_minimap.pixel_x;u->minimap.pixel_y=previous_minimap.pixel_y;
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
    pharaoh_windows_game_input_bind_sound(h->game_input,h,&u->sidebar_sound,
        &u->sidebar_sound_device);
    u->sidebar_action=(PharaohCitySidebarActionContext){.current_window=window,
        .redraw=&h->graphics.redraw_requested,.slide_step=&u->sidebar_slide_step,
        .view=&u->sidebar_view,.sound=&u->sidebar_sound,
        .sound_device=&u->sidebar_sound_device};
    {
        const PharaohCityInputActionProviders providers={.sidebar=&u->sidebar_action,
            .button_parameter=(uint32_t *)(void *)&h->button_parameter,
            .button_secondary_parameter=(uint32_t *)(void *)&h->button_secondary_parameter,
            .image_input=&u->city_image_input,.generic_input=&u->city_generic_input};
        if(pharaoh_city_input_action_bindings_bind(&u->input_actions,&providers)!=
            PHARAOH_CITY_INPUT_ACTION_READY)return 0;
    }
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

uint8_t pharaoh_windows_city_ui_sidebar_slide(PharaohWindowsStartupHost *h,
    PharaohCityRenderContext *city)
{
    struct PharaohWindowsCityUi *const u=h->city_ui;
    if(u==NULL||u->current_window==NULL||h->platform_combined_elapsed_ms==NULL)
        return 0;
    const PharaohCityPendingBackgroundContext background={.sidebar=&u->sidebar,
        .sidebar_operations=&u->sidebar_ops,.city=city,.popup=NULL,
        .current_window=u->current_window,.redraw=&h->graphics.redraw_requested,
        .modal_active=&u->modal_active,.modal_return_window=&u->modal_return_window};
    const PharaohCitySidebarSlideContext slide={
        .frame_duration=(const int32_t *)(const void *)h->platform_combined_elapsed_ms,
        .screen_width=&h->graphics.render_width,
        .step=(uint32_t *)(void *)&u->sidebar_slide_step,.collapsed=&u->collapsed,
        .offsets=sidebar_slide_offsets,.collapsed_x=u->content.collapsed_x,
        .expanded_x=u->content.sidebar_x,.game_image_base=u->sidebar.game_image_base,
        .current_window=u->current_window,.ui_redraw=&u->ui_redraw,
        .view=&u->sidebar_view,.city=city,.images=u->images,
        .button_draw=&u->button_draw,.content=&u->content,.graphics=&u->graphics,
        .buttons=&u->input_tables,.background=&background};
    return pharaoh_city_sidebar_slide(&slide);
}

void pharaoh_windows_city_ui_sidebar_slide_input(PharaohWindowsStartupHost *h,
    int32_t mouse_x,int32_t mouse_y,const PharaohCameraScrollInput *scroll_input,
    const PharaohCameraScrollContext *scroll)
{
    struct PharaohWindowsCityUi *const u=h->city_ui;
    if(u==NULL||u->current_window==NULL||h->game_input==NULL)return;
    const PharaohMouseUpdateState *const mouse=&h->game_input->mouse;
    const PharaohCityIngameFrame frame={&mouse_x,&mouse_y,
        &mouse->left_button_pressed,&mouse->left_button_down,
        &mouse->left_button_released,&mouse->right_button_pressed,
        &mouse->right_button_released};
    const PharaohCityIngameDispatchContext context={.frame=&frame,
        .current_window=u->current_window,.redraw=&h->graphics.redraw_requested,
        .ui_redraw=&u->ui_redraw,.keyboard_field=&u->keyboard_field,
        .image_focus=&u->image_focus,.arrow_focus=&u->arrow_focus,
        .menu_focus=&u->menu_focus,.ui_mouse_x=&u->ui_mouse_x,
        .ui_mouse_y=&u->ui_mouse_y,.mouse_drag_state=&u->mouse_drag_state,
        .scroll_input=scroll_input,.scroll=scroll};
    pharaoh_city_sidebar_slide_input(&context);
}

int32_t pharaoh_windows_city_ui_open_empty_terrain(PharaohWindowsStartupHost *h,
    PharaohMissionScene *scene,const int32_t *tile_x,const int32_t *tile_y)
{
    struct PharaohWindowsCityUi *const u=h->city_ui;
    if(!u||!h->game_input)return -1;
    u->info_initialize.cursor_tile_x=tile_x;u->info_initialize.cursor_tile_y=tile_y;
    u->info_initialize.display_height=&h->graphics.screen_height; /*actualE38DAC, distinct from E38D98*/
    u->info_initialize.mouse_y=&h->game_input->mouse.logical_y;
    u->info_initialize.panel_position_mode=&u->collapsed;
    PharaohBuildingInfoInitProviders p={.initializer=&u->info_initialize};
    pharaoh_mission_scene_bind_info_init_resources(scene,&p);
    pharaoh_building_info_init_bindings_bind(&u->info_init_bindings,&p);
    PharaohMapSurfaceContext *const surface=pharaoh_mission_scene_surface(scene);
    const PharaohBuildingInfoEmptyTerrainInitDomain d={surface->map,surface->geometry};
    const PharaohBuildingInfoInitResult r=pharaoh_building_info_init_bindings_initialize_empty_terrain(&u->info_init_bindings,&d);
    return r.support==PHARAOH_INFO_INIT_READY?r.opened:-1;
}

int32_t pharaoh_windows_city_ui_empty_info_input(PharaohWindowsStartupHost *h)
{
    struct PharaohWindowsCityUi *const u=h->city_ui;
    if(!u||!h->game_input||u->info_state.mode!=1||u->info_state.walker_count!=0)return 0;
    const PharaohMouseUpdateState *const m=&h->game_input->mouse;
    PharaohGenericButtonInputState *const g=&u->info_generic_input;
    g->cursor_x=m->logical_x;g->cursor_y=m->logical_y;
    g->left_down=m->left_button_down;g->left_pressed=m->left_button_pressed;g->left_released=m->left_button_released;
    g->right_down=m->right_button_down;g->right_pressed=m->right_button_pressed;g->right_released=m->right_button_released;
    PharaohImageButtonInputState *const i=&u->info_image_input;
    i->cursor_x=m->logical_x;i->cursor_y=m->logical_y;i->left_pressed=m->left_button_pressed;
    i->right_pressed=m->right_button_pressed;i->left_down=m->left_button_down;
    PharaohSquareButtonInputState *const s=&u->info_square_input;
    s->cursor_x=m->logical_x;s->cursor_y=m->logical_y;s->current_window_id=*u->info.current_window;
    s->left_went_down=m->left_button_pressed;s->left_clicked=m->left_button_released;s->left_down=m->left_button_down;
    i->redraw_requested=0;
    u->info_image_focus.focused_id=u->info_focus_building;
    u->info_button_layout.window_key=*u->info.current_window;
    pharaoh_building_info_live_handle_input(&u->info);
    if(i->redraw_requested)*u->info.redraw_requested=1;
    u->info_focus_building=u->info_image_focus.focused_id;
    if(u->info_actions.last_result.support!=PHARAOH_INFO_ACTION_READY) {
        OutputDebugStringA("LostTomb: information action has an unbound real provider\n");
        return 0;
    }
    return 1;
}

int32_t pharaoh_windows_city_ui_draw_empty_info(PharaohWindowsStartupHost *h,
    PharaohCityRenderContext *city)
{
    struct PharaohWindowsCityUi *const u=h->city_ui;
    if(!u||u->info_state.mode!=1||u->info_state.walker_count!=0)return 0;
    const PharaohBuildingInfoDrawProviders p={.ui=&u->info,.storage=&u->info_drawing,
        .content=&u->info_content,.walkers=&u->info_walkers,.terrain=&u->info_terrain,
        .square_buttons=&u->info_square_drawing,.image_buttons=&u->button_draw,.city=city,
        .industry=&u->industry,.services=&u->services};
    pharaoh_building_info_draw_bindings_initialize(&u->info_draw_bindings,&p);
    if(pharaoh_building_info_draw_bindings_background(&u->info_draw_bindings).support!=PHARAOH_INFO_DRAW_READY)return 0;
    return pharaoh_building_info_draw_bindings_foreground(&u->info_draw_bindings).support==PHARAOH_INFO_DRAW_READY;
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

int32_t pharaoh_windows_city_ui_navigation(PharaohWindowsStartupHost *h,
    int32_t mouse_x,int32_t mouse_y,uint8_t left_down,uint8_t right_went_down)
{
    struct PharaohWindowsCityUi *u=h->city_ui;
    if(u==NULL)return 0;
    if(u->collapsed==0) {
        pharaoh_build_sidebar_hover_update(mouse_x,mouse_y,u->minimap.left,u->minimap.top,
            u->minimap.right,u->minimap.bottom,&u->navigation_hover);
        PharaohCityViewContext view=*u->view;
        const PharaohMinimapClickContext click={&u->minimap,&view,&mouse_x,&mouse_y,
            &u->navigation_hover,u->content.sidebar_x,&left_down,&right_went_down,
            (const uint32_t *)(const void *)u->minimap.editor_active,&u->navigation_x,&u->navigation_y};
        if(pharaoh_minimap_sidebar_click(&click)!=0)return 1;
    }
    PharaohImageButtonInputState *const input=&u->city_image_input;
    const PharaohMouseUpdateState *const mouse=&h->game_input->mouse;
    input->cursor_x=mouse_x;input->cursor_y=mouse_y;
    input->left_pressed=mouse->left_button_pressed;
    input->right_pressed=mouse->right_button_released;
    input->left_down=mouse->left_button_down;input->redraw_requested=0;
    const PharaohCityInputImageTable *const table=u->collapsed==0?
        &u->input_tables.expanded_extra:&u->input_tables.collapsed_expand;
    const int32_t x=u->collapsed==0?*u->content.sidebar_x:*u->content.collapsed_x;
    const int32_t y=u->collapsed==0?30:24;
    /* Only the complete409D10 row is activated here. The adjacent overlay
       row keeps its exact data but remains outside this partial host route. */
    const int32_t handled=pharaoh_image_buttons_handle_mouse(x,y,table->records,
        1,*table->trailing_trigger,input);
    if(input->redraw_requested!=0)u->ui_redraw=1;
    return handled;
}
