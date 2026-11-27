#include "pharaoh/platform/windows_mission_runtime.h"
#include "pharaoh/runtime/camera_scroll.h"
#include <mmsystem.h>
#include "pharaoh/platform/windows_city_ui.h"
#include "pharaoh/scenario/mission_scene_program.h"
#include "pharaoh/scenario/campaign_continue.h"
#include "pharaoh/core/pk_explode_data.h"
#include "pharaoh/core/percentage_adjust.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/ui/image_button_input.h"
#include "pharaoh/runtime/sound_stream_fade.h"
#include "pharaoh/runtime/animation_frame_state.h"
#include "pharaoh/rendering/city_ground.h"
#include "pharaoh/rendering/city_top.h"
#include "pharaoh/rendering/city_object_layers.h"
#include "pharaoh/rendering/building_stock_draw.h"
#include "pharaoh/rendering/building_animation_offset.h"
#include "pharaoh/rendering/monument_spot_draw.h"
#include "pharaoh/rendering/sun_temple_draw.h"
#include "pharaoh/rendering/obelisk_draw.h"
#include "pharaoh/rendering/abu_simbel_draw.h"
#include "pharaoh/rendering/granary_draw.h"
#include "pharaoh/rendering/temple_annex_draw.h"
#include "pharaoh/rendering/pyramid_draw.h"
#include "pharaoh/rendering/pyramid_walker_draw.h"
#include "pharaoh/rendering/image_draw.h"
#include "pharaoh/building/temple_images.h"
#include "pharaoh/building/occupancy_query.h"
#include "pharaoh/building/type_queries.h"
#include "pharaoh/religion/building_types.h"
#include "pharaoh/map/house_water.h"
#include "pharaoh/map/farm_tiles.h"
#include <stdlib.h>
#include <string.h>

struct PharaohWindowsMissionRuntime {
    PharaohWindowsStartupHost *host;
    PharaohBriefingState *briefing;
    uint8_t *window;
    PharaohMissionSceneProgramData *program;
    PharaohMissionScene *scene;
    PharaohRouteImage *route_images;
    int32_t image_count;
    PharaohFileReadPartOperations file_operations;
    PharaohFileReadPartContext files;
    PharaohFilePathState path;
    PharaohFileMediaRootContext media;
    PharaohCrtErrorState crt;
    PharaohFileDirectoryResetContext directory;
    char startup_directory[MAX_PATH],directory_detail[MAX_PATH],media_path[263];
    PharaohBriefingOperations draw;
    PharaohWindowsBriefingRenderer renderer;
    PharaohBriefingHintContext hint;
    PharaohImageButtonRecord accept[1],difficulty[2],back[1],up[1],down[1];
    PharaohImageButtonInputState input;
    uint32_t selected_choice,city_word;
    int32_t focus[4],special_music,music_fade,speech_fade;
    uint8_t transition_mode,io_failed;
    int32_t scroll_drag_position;
    const uint8_t **image_pixels;
    int32_t *image_group_first;
    int16_t *sprite_offset_x,*sprite_offset_y;
    int32_t image_pixel_count;
    PharaohImageDrawState image_draw_state;
    PharaohSpriteClipState image_clip;
    PharaohImageDrawContext image_draw;
    uint8_t *external_pixels;
    char bitmap_name[PHARAOH_SPRITE_LIBRARY_BITMAP_NAME_BYTES];
    PharaohCityGroundScan ground_scan;
    PharaohCityGroundOperations ground_operations;
    PharaohCityGroundContext ground;
    PharaohBuildingAnimationContext building_animations;
    PharaohBuildingAnimationOffsetContext animation_offset;
    PharaohBuildingStockDrawContext building_stock;
    PharaohCityTopOperations top_operations;
    PharaohCityTopContext top;
    PharaohCityObjectLayerOperations high_walker_operations;
    PharaohCityObjectLayerContext high_walkers;
    PharaohFarmTileDrawContext farm_tiles;
    PharaohMonumentSpotDrawContext monument_spot_draw;
    PharaohMonumentSpotDrawOperations monument_spot_operations;
    PharaohMonumentSpotGroupDrawContext monument_spots;
    PharaohSunTempleDrawContext sun_temple;
    PharaohObeliskDrawContext obelisk;
    PharaohAbuSimbelDrawContext abu_simbel;
    PharaohGranaryDrawContext granary_draw;
    PharaohTempleAnnexDrawContext temple_annex;
    PharaohPyramidCoordinateTableStorage pyramid_coordinate_storage;
    PharaohPyramidWalkerDrawContext pyramid_walker;
    PharaohWalkerOverlayContext walker_overlay;
    PharaohWalkerDrawOperations walker_operations;
    PharaohWalkerDrawContext walker_draw;
    PharaohPyramidDrawContext pyramid_draw;
    uint16_t top_pass_sequence;
    PharaohAnimationFrameState animation_frames;
    uint32_t ambient_scores[12],ambient_total;
    uint32_t farm_game_day;
    int32_t farm_game_tick;
    int32_t diagnostic_frame_status;
    uint32_t navigation_tick,navigation_west,navigation_east,navigation_north,navigation_south;
    uint32_t marked_tile,marked_x,marked_y,advance_animation,last_animation_tick,debug_invalid;
    uint32_t water_image,water_variant_a,water_variant_b,cleared_image,royal_tomb_image;
    uint16_t tint;
    int32_t sound_suppressed,building_sound_ids[256];
    int32_t selected_walker_x,selected_walker_y;
    uint8_t skip_walker_draw;
    /* Native6DE515,605B3D and78D5EC have pinned zero initial values. */
    uint8_t alternate_overlay,walker_layer_cycle_enabled;
    uint32_t walker_layer_cycle_counter;
    PharaohCitySoundViewRecord sound_views[70];
    int32_t special_image;
};

static uint8_t top_is_farm(void *data,int32_t building)
{
    const PharaohWindowsMissionRuntime *const r=data;
    return (uint8_t)pharaoh_building_is_farm(
        pharaoh_mission_scene_storage(r->scene)->view.buildings,building);
}
static int32_t animation_is_floodplain_farm(void *data,int32_t building)
{
    const PharaohWindowsMissionRuntime *const r=data;
    const PharaohSavedSceneStorage *const s=pharaoh_mission_scene_storage(r->scene);
    if(!pharaoh_building_is_farm(s->view.buildings,building)) return 0;
    return (int32_t)(uint8_t)pharaoh_building_has_active_occupant(
        pharaoh_mission_scene_occupancy(r->scene),building);
}
static uint8_t top_is_floodplain_farm(void *data,int32_t building)
{return (uint8_t)animation_is_floodplain_farm(data,building);}
static int32_t top_farm_near_water(void *data,int32_t building)
{
    PharaohWindowsMissionRuntime *const r=data;
    return (int32_t)pharaoh_building_region_has_terrain_4(
        pharaoh_mission_scene_occupancy(r->scene),building);
}
static int32_t top_farm_has_water_access(void *data,int32_t building)
{
    PharaohWindowsMissionRuntime *const r=data;
    return pharaoh_farm_has_water_access(
        pharaoh_mission_scene_storage(r->scene)->view.buildings,building);
}
static void top_draw_farm(void *data,int32_t building,int32_t x,int32_t y,
    int32_t extra)
{
    PharaohWindowsMissionRuntime *const r=data;
    pharaoh_map_draw_farm(&r->farm_tiles,building,x,y,(uint8_t)extra);
}
static int32_t top_is_palace(void *data,int32_t building)
{
    const PharaohWindowsMissionRuntime *const r=data;
    return pharaoh_building_stores_treasury_funds(
        pharaoh_mission_scene_storage(r->scene)->view.buildings,building);
}
static int32_t top_main_building(void *data,int32_t building)
{
    const PharaohWindowsMissionRuntime *const r=data;
    const PharaohBuildingTypeRecord *const buildings=
        pharaoh_mission_scene_storage(r->scene)->view.buildings;
    return pharaoh_building_main_part_id(buildings,building);
}
static int32_t top_houses_water(void *data,int32_t building,int32_t radius)
{
    PharaohWindowsMissionRuntime *const r=data;
    int32_t x=0,y=0;
    const PharaohSavedSceneStorage *const s=pharaoh_mission_scene_storage(r->scene);
    const PharaohSavedGameGeometry *const g=pharaoh_mission_scene_geometry(r->scene);
    const PharaohMapHouseWaterContext context={s->view.buildings,s->view.building_ids,
        g->width,g->height,g->grid_start,&x,&y};
    return pharaoh_map_houses_water_in_radius(&context,building,radius);
}
static int32_t top_animation_offset(void *data,int32_t image,int32_t tile)
{return pharaoh_building_animation_offset(&((PharaohWindowsMissionRuntime *)data)->animation_offset,image,tile);}
static void top_building_stock(void *data,int32_t building,int32_t x,int32_t y)
{pharaoh_building_draw_stock(&((PharaohWindowsMissionRuntime *)data)->building_stock,building,x,y);}
static void top_draw_image(void *data,int32_t image,int32_t x,int32_t y)
{pharaoh_image_draw(&((PharaohWindowsMissionRuntime *)data)->image_draw,(uint32_t)image,(uint32_t)x,(uint32_t)y);}
static int32_t top_royal_tomb_table(void *data,int32_t chamber_kind)
{(void)data;return pharaoh_royal_tomb_spot_table(chamber_kind);}
static void top_sphinx_spots(void *data,int32_t x,int32_t y)
{pharaoh_sphinx_draw_work_spots(&((PharaohWindowsMissionRuntime *)data)->monument_spots,x,y);}
static void top_sun_temple_spots(void *data,int32_t x,int32_t y)
{pharaoh_sun_temple_draw_work_spots(&((PharaohWindowsMissionRuntime *)data)->monument_spots,x,y);}
static void top_library_spots(void *data,int32_t x,int32_t y)
{pharaoh_library_draw_work_spots(&((PharaohWindowsMissionRuntime *)data)->monument_spots,x,y);}
static void top_abu_simbel_spots(void *data,int32_t x,int32_t y)
{pharaoh_abu_simbel_draw_work_spots(&((PharaohWindowsMissionRuntime *)data)->monument_spots,x,y);}
static void top_caesareum_spots(void *data,int32_t x,int32_t y)
{pharaoh_caesareum_draw_work_spots(&((PharaohWindowsMissionRuntime *)data)->monument_spots,x,y);}
static void top_lighthouse_spots(void *data,int32_t x,int32_t y)
{pharaoh_lighthouse_draw_work_spots(&((PharaohWindowsMissionRuntime *)data)->monument_spots,x,y);}
static void top_royal_tomb_spots(void *data,int32_t x,int32_t y,int32_t building)
{pharaoh_royal_tomb_draw_work_spots(&((PharaohWindowsMissionRuntime *)data)->monument_spots,x,y,building);}
static void top_sun_temple_blocks(void *data,int32_t building,int32_t x,int32_t y)
{pharaoh_sun_temple_draw_stage_blocks(&((PharaohWindowsMissionRuntime *)data)->sun_temple,building,x,y);}
static uint8_t top_obelisk_progress(void *data,int32_t building)
{
    const PharaohWindowsMissionRuntime *const r=data;
    return pharaoh_mission_scene_storage(r->scene)->view.buildings[building].pyramid_size_variant;
}
static void top_obelisk(void *data,int32_t building,int32_t x,int32_t y)
{pharaoh_obelisk_draw_overlay(&((PharaohWindowsMissionRuntime *)data)->obelisk,building,x,y);}
static const uint8_t *top_abu_simbel_built_parts(void *data,int32_t main_building)
{
    const PharaohWindowsMissionRuntime *const r=data;
    return (const uint8_t *)(const void *)&pharaoh_mission_scene_storage(r->scene)->view.buildings[main_building]+0x61;
}
static void top_abu_simbel_built(void *data,int32_t building,int32_t x,int32_t y)
{pharaoh_abu_simbel_draw_built_parts(&((PharaohWindowsMissionRuntime *)data)->abu_simbel,building,x,y);}
static void top_determine_foods(void *data)
{pharaoh_mission_scene_determine_available_foods(((PharaohWindowsMissionRuntime *)data)->scene);}
static void top_granary_stores(void *data,int32_t building,int32_t tile,int32_t x,int32_t y)
{pharaoh_granary_draw_stores(&((PharaohWindowsMissionRuntime *)data)->granary_draw,building,(uint32_t)tile,x,y);}
static int32_t top_god_with_status(void *data,int32_t status)
{
    PharaohWindowsMissionRuntime *const r=data;
    const PharaohSavedSceneStorage *const saved=pharaoh_mission_scene_storage(r->scene);
    return pharaoh_city_god_with_status(
        (const int16_t *)(const void *)(saved->view.metadata+0x0a),status);
}
static void top_project_tile(void *data,int32_t x,int32_t y,
    int32_t *pixel_x,int32_t *pixel_y)
{
    PharaohWindowsMissionRuntime *const r=data;
    const PharaohCityViewContext *const view=pharaoh_mission_scene_view(r->scene);
    pharaoh_city_view_tile_to_pixel(view->state,view->projection,
        view->map_bounds,x,y,pixel_x,pixel_y);
}
static void top_temple_annex(void *data,int32_t building,int32_t x,int32_t y)
{
    (void)x;(void)y;
    pharaoh_temple_complex_draw_annex(
        &((PharaohWindowsMissionRuntime *)data)->temple_annex,building);
}
static void top_pyramid_walker_offset(void *data,int32_t walker,
    int32_t *x,int32_t *y)
{pharaoh_pyramid_walker_pixel_offset(&((PharaohWindowsMissionRuntime *)data)->pyramid_walker,walker,x,y);}
static void top_walker_number(void *data,int32_t value,int32_t prefix,
    const char *suffix,int32_t x,int32_t y,int32_t font,int32_t color)
{
    PharaohWindowsMissionRuntime *const r=data;(void)suffix;
    (void)r->draw.draw_number(r->draw.user_data,value,prefix,x,y,font,color);
}
static void top_draw_walker(void *data,int32_t walker,int32_t x,int32_t y,
    uint32_t selection)
{pharaoh_city_draw_walker(&((PharaohWindowsMissionRuntime *)data)->walker_draw,walker,x,y,selection);}
static void top_draw_pyramid(void *data,int32_t image,int32_t x,int32_t y,
    int32_t building)
{pharaoh_pyramid_draw_part(&((PharaohWindowsMissionRuntime *)data)->pyramid_draw,image,x,y,building);}

static int32_t rebuild_image_pixels(PharaohWindowsMissionRuntime *r)
{
    PharaohWindowsStartupHost *const h=r->host;
    const int32_t count=(int32_t)h->loaded_libraries->next_image;
    const uint8_t **pixels;
    if(count==r->image_pixel_count && r->image_pixels!=NULL &&
        r->image_group_first!=NULL && r->sprite_offset_x!=NULL &&
        r->sprite_offset_y!=NULL) return 1;
    pixels=calloc((size_t)count,sizeof(*pixels));
    int16_t *const offset_x=calloc((size_t)count,sizeof(*offset_x));
    int16_t *const offset_y=calloc((size_t)count,sizeof(*offset_y));
    int32_t *const group_first=calloc(PHARAOH_WINDOWS_STARTUP_TAG_CAPACITY,
        sizeof(*group_first));
    if(pixels==NULL || offset_x==NULL || offset_y==NULL || group_first==NULL) {
        free(pixels);free(offset_x);free(offset_y);free(group_first);return 0;
    }
    for(int32_t i=0;i<count;++i) {
        const PharaohSpriteImageRecord *const image=&h->sprite_images[i];
        if(image->is_external==0 && image->library_index<PHARAOH_SPRITE_LIBRARY_COUNT) {
            const PharaohSpriteLibrary *const library=
                &h->loaded_libraries->items[image->library_index];
            if(library->pixels!=NULL && image->pixel_offset<=library->pixel_bytes &&
                image->pixel_bytes<=library->pixel_bytes-image->pixel_offset)
                pixels[i]=library->pixels+image->pixel_offset;
        }
        offset_x[i]=image->sprite_offset_x;offset_y[i]=image->sprite_offset_y;
    }
    for(int32_t i=0;i<PHARAOH_WINDOWS_STARTUP_TAG_CAPACITY;++i)
        group_first[i]=(int32_t)h->sprite_tags[i].image_index;
    free(r->image_pixels);free(r->image_group_first);
    free(r->sprite_offset_x);free(r->sprite_offset_y);
    r->image_pixels=pixels;r->image_group_first=group_first;
    r->sprite_offset_x=offset_x;r->sprite_offset_y=offset_y;r->image_pixel_count=count;
    return 1;
}

static void bind_ground_scan(PharaohWindowsMissionRuntime *r,
    const PharaohCityViewState *view)
{
    const PharaohCityViewportGeometry *const v=&view->viewport;
    PharaohCityGroundScan *const s=&r->ground_scan;
    s->tile_y[0]=(uint32_t)v->tile_double_height;
    s->tile_y[1]=(uint32_t)v->tile_height_and_half;
    s->tile_y[2]=(uint32_t)v->tile_height;
    s->tile_y[3]=(uint32_t)v->tile_half_height;
    s->tile_x[0]=(uint32_t)v->tile_double_width;
    s->tile_x[1]=(uint32_t)v->tile_width_and_half;
    s->tile_x[2]=(uint32_t)v->tile_width;
    s->tile_x[3]=(uint32_t)v->tile_half_width;
    s->visible_rows=(uint32_t)v->height_tiles;
    s->visible_columns=(uint32_t)v->width_tiles;
    s->camera_row=(uint32_t)view->camera_y;
    s->camera_column=(uint32_t)view->camera_x;
    s->start_y=(uint32_t)v->screen_y;
    s->start_x=(uint32_t)v->screen_x;
}

static int32_t read_file(void *data,int32_t file,void *out,uint32_t bytes)
{
    PharaohWindowsMissionRuntime *r=data;
    const int32_t result=pharaoh_windows_startup_handle_read(file,out,bytes);
    if(result!=(int32_t)bytes) r->io_failed=1;
    return result;
}
static int32_t seek_file(void *data,int32_t file,int32_t offset,int32_t origin)
{(void)data;return pharaoh_windows_startup_handle_seek(file,offset,origin);}
static void *allocate(void *data,uint32_t size)
{(void)data;return malloc(size);}
static void release(void *data,void *allocation)
{(void)data;free(allocation);}
static void move(void *out,const void *in,uint32_t bytes)
{memmove(out,in,bytes);}
static void select_message(void *data,int32_t group,int32_t id)
{
    PharaohWindowsMissionRuntime *r=data;
    pharaoh_text_select(&r->host->text_selection,group,id);
}
static void read_error(void *data,uint32_t x,uint32_t y,int32_t argument)
{
    PharaohWindowsMissionRuntime *r=data;
    (void)x;(void)y;(void)argument;r->io_failed=1;
    OutputDebugStringA("LostTomb: saved scene read failed\n");
}
static void codec_error(void *data,const char *message)
{
    PharaohWindowsMissionRuntime *r=data;r->io_failed=1;
    OutputDebugStringA(message);
}
static uint32_t clock_tick(void *data)
{(void)data;return GetTickCount();}
static int32_t clock_counter(void *data,int64_t *out)
{LARGE_INTEGER value;(void)data;const int result=QueryPerformanceCounter(&value);if(result)*out=value.QuadPart;return result;}
static int32_t clock_frequency(void *data,int64_t *out)
{LARGE_INTEGER value;(void)data;const int result=QueryPerformanceFrequency(&value);if(result)*out=value.QuadPart;return result;}

/* Exact logical fields from the pinned tables579788,5C81B0,5797B0,
 *5C7DF8,5C7E20. The gaps between native arrays are deliberately not packed:
 *00425990's post-table trigger is supplied separately from its real address. */
static void button(PharaohImageButtonRecord *b,int16_t x,int16_t width,int16_t height,
    int16_t trigger,int16_t group,int16_t offset,uint32_t action,uint32_t parameter)
{
    memset(b,0,sizeof(*b));b->x=x;b->width=width;b->height=height;
    b->trigger=trigger;b->image_group=group;b->image_offset=offset;b->enabled=1;
    const uint32_t right=UINT32_C(0x0051f780),zero=0;
    memcpy((uint8_t *)b+16,&action,4);memcpy((uint8_t *)b+20,&right,4);
    memcpy((uint8_t *)b+28,&parameter,4);memcpy((uint8_t *)b+32,&zero,4);
}
static void fade_speech(void *data,int32_t channel)
{
    PharaohWindowsMissionRuntime *r=data;
    PharaohSoundStreamFadeContext fade={&r->host->sound.state,
        &r->host->sound.stream_operations,&r->host->sound.ready,
        &r->music_fade,&r->speech_fade};
    pharaoh_sound_stream_fade_to_stop(&fade,(uint32_t)channel);
}
static PharaohCampaignContinueState continue_state(PharaohWindowsMissionRuntime *r)
{
    const PharaohSavedSceneStorage *s=pharaoh_mission_scene_storage(r->scene);
    PharaohScenarioPlayerRuntime *p=&s->view.players[*s->view.current_player];
    return (PharaohCampaignContinueState){
        .index=&r->host->campaign_index,.settings=&r->host->settings,
        .current_window_id=r->window,.transition_mode=&r->transition_mode,
        .redraw_requested=&r->host->graphics.redraw_requested,.selected_choice=&r->selected_choice,
        .focus_ids={&r->focus[0],&r->focus[1],&r->focus[2],&r->focus[3]},
        .previous_city_overlay=pharaoh_mission_scene_previous_overlay(r->scene),
        .city_state_word=(uint32_t *)(void *)((uint8_t *)p+0x471c),
        .difficulty=&p->difficulty,.maximum_difficulty=r->host->settings.difficulty,
        .special_music_playing=&r->special_music};
}
static void difficulty_buildings(void *data)
{
    PharaohWindowsMissionRuntime *r=data;
    r->host->building_model_context.difficulty=(PharaohModelDifficulty)r->host->settings.difficulty;
    (void)pharaoh_building_model_load(&r->host->building_model_context);
}
static void difficulty_figures(void *data)
{
    PharaohWindowsMissionRuntime *r=data;
    r->host->figure_model_context.difficulty=(PharaohModelDifficulty)r->host->settings.difficulty;
    (void)pharaoh_figure_model_load(&r->host->figure_model_context);
}
static void difficulty_taxes(void *data)
{
    PharaohWindowsMissionRuntime *r=data;
    r->host->tax_model_context.difficulty=(PharaohModelDifficulty)r->host->settings.difficulty;
    (void)pharaoh_tax_sentiment_model_load(&r->host->tax_model_context);
}
static void difficulty_images(void *data)
{
    PharaohWindowsMissionRuntime *r=data;
    const PharaohSavedSceneStorage *s=pharaoh_mission_scene_storage(r->scene);
    PharaohSceneEnemyAssets assets={&r->host->sprite_libraries,
        (int16_t *)(void *)(s->view.metadata+0x2bc),&r->host->enemy_models,&r->host->figure_models};
    pharaoh_scene_initialize_enemy_properties(&assets);
}
static int32_t adjust(void *data,int32_t value,int32_t percent)
{(void)data;return pharaoh_adjust_by_percentage(value,percent);}
static void dispatch_button(void *data,uint32_t action)
{
    PharaohWindowsMissionRuntime *r=data;
    const uint32_t parameter=r->input.primary_parameter;
    if(action==UINT32_C(0x00418220)) {
        PharaohCampaignContinueState state=continue_state(r);
        const PharaohCampaignContinueOperations ops={.user_data=r,.fade_music=fade_speech};
        pharaoh_campaign_continue(&state,&ops);
    } else if(action==UINT32_C(0x00418350)) {
        PharaohCampaignContinueState state=continue_state(r);
        pharaoh_campaign_briefing_back(&state);
    } else if(action==UINT32_C(0x00402e50)) {
        const PharaohSavedSceneStorage *s=pharaoh_mission_scene_storage(r->scene);
        PharaohScenarioPlayerRuntime *p=&s->view.players[*s->view.current_player];
        int32_t value;memcpy(&value,s->view.metadata+0x20,4);
        const PharaohMapSelectionOperations ops={.user_data=r,
            .refresh_difficulty_a=difficulty_buildings,.refresh_difficulty_b=difficulty_figures,
            .refresh_difficulty_c=difficulty_taxes,.reload_map_assets=difficulty_images,.adjust_value=adjust};
        pharaoh_map_selection_adjust_difficulty((int32_t)parameter,&r->host->settings.difficulty,
            *r->window,value,pharaoh_mission_scene_program_data(r->program)->difficulty_percentages,
            &p->treasury,&r->host->graphics.redraw_requested,&ops);
    } else if(action==UINT32_C(0x004c7e90)) {
        if(parameter==0 && r->briefing->scroll_position>0) --r->briefing->scroll_position;
        else if(parameter==1 && r->briefing->scroll_position<r->briefing->scroll_maximum) ++r->briefing->scroll_position;
        pharaoh_rich_text_reset_links(&r->renderer.text);r->briefing->scroll_drag=0;
        r->host->graphics.redraw_requested=2;
    }
    /* Every table's right callback0051F780 is the proven empty native RET. */
}
static int32_t handle_buttons(void *data,int32_t x,int32_t y,
    enum PharaohBriefingButtonTable table,int32_t count)
{
    PharaohWindowsMissionRuntime *r=data;
    PharaohImageButtonRecord *buttons=table==PHARAOH_BRIEFING_ACCEPT_BUTTON?r->accept:
        table==PHARAOH_BRIEFING_DIFFICULTY_BUTTONS?r->difficulty:r->back;
    const int16_t after=table==PHARAOH_BRIEFING_ACCEPT_BUTTON?31:
        table==PHARAOH_BRIEFING_DIFFICULTY_BUTTONS?4:288;
    return pharaoh_image_buttons_handle_mouse(x,y,buttons,count,after,&r->input);
}
static uint8_t has_choices(void *data)
{
    PharaohWindowsMissionRuntime *r=data;
    return r->draw.campaign_has_choices(r->draw.user_data);
}
static void handle_scroll(void *data)
{
    PharaohWindowsMissionRuntime *r=data;PharaohBriefingState *b=r->briefing;
    r->up[0].enabled=b->scroll_up_enabled;r->down[0].enabled=b->scroll_down_enabled;
    if(pharaoh_image_buttons_handle_mouse(b->panel_x+560,b->panel_y+168,r->up,1,39,&r->input)!=0) return;
    if(pharaoh_image_buttons_handle_mouse(b->panel_x+560,b->panel_y+382,r->down,1,18,&r->input)!=0) return;
    PharaohRichTextScrollbarState scroll={r->input.cursor_x,r->input.cursor_y,r->input.left_down,
        b->text_x,b->text_y,b->text_width_blocks,b->text_height_blocks,b->scroll_maximum,
        b->scroll_position,0,0,0};
    if(pharaoh_rich_text_handle_scrollbar(&scroll,&r->renderer.text)!=0) {
        b->scroll_position=scroll.scroll_position;b->scroll_drag=scroll.dragging;
        r->scroll_drag_position=scroll.drag_position;
        r->host->graphics.redraw_requested=scroll.redraw_requested;
    }
}

PharaohWindowsMissionRuntime *pharaoh_windows_mission_runtime_create(
    PharaohWindowsStartupHost *host,PharaohBriefingState *briefing,uint8_t *window)
{
    PharaohWindowsMissionRuntime *r=calloc(1,sizeof(*r));if(r==NULL) return NULL;
    r->host=host;r->briefing=briefing;r->window=window;
    r->program=pharaoh_mission_scene_program_create(&host->campaign_index,
        (const uint32_t (*)[6])(const void *)host->trade_model_context.recommendations);
    r->image_count=(int32_t)host->loaded_libraries->next_image;
    r->route_images=calloc((size_t)r->image_count,sizeof(*r->route_images));
    if(r->program==NULL || r->route_images==NULL) {pharaoh_windows_mission_runtime_destroy(r);return NULL;}
    for(int32_t i=0;i<r->image_count;++i) {
        r->route_images[i].width=host->sprite_images[i].width;
        r->route_images[i].height=host->sprite_images[i].height;
    }
    (void)GetCurrentDirectoryA(MAX_PATH,r->startup_directory);
    memcpy(r->media_path,"C:\\",4);
    r->media=(PharaohFileMediaRootContext){&r->path,host->detected_drive_letter,&r->crt};
    r->directory=(PharaohFileDirectoryResetContext){r->startup_directory,&r->crt,r->directory_detail,
        &host->log_format_state,&host->log_format_operations};
    r->file_operations=(PharaohFileReadPartOperations){seek_file,read_file};
    r->files=(PharaohFileReadPartContext){&host->file_size,&r->file_operations,r,&r->media,
        &r->directory,&host->text_selection,r->media_path};
    PharaohMissionSceneInputs inputs={.program=pharaoh_mission_scene_program_data(r->program),
        .settings=&host->settings,.briefing=briefing,.window_id=window,
        .redraw_requested=&host->graphics.redraw_requested,.text=&host->text_catalog,
        .package_files=&r->files,.building_models=&host->building_model_context,
        .figure_models=&host->figure_model_context,.tax_model=&host->tax_model_context,
        .libraries=&host->sprite_libraries,
        .groups=(const PharaohSpriteGroupRecord *)(const void *)host->sprite_tags,
        .images=host->sprite_images,
        .image_count=(const int32_t *)(const void *)&host->loaded_libraries->next_image,.clock_user_data=r,
        .get_time=clock_tick,.get_tick=clock_tick,.query_counter=clock_counter,.query_frequency=clock_frequency,
        .random_seed=GetTickCount(),.screen_width=host->graphics.render_width};
    inputs.reader.io=(PharaohChunkIoContext){.read_raw=read_file,.allocate=allocate,.free=release,
        .move=move,.set_message=select_message,.show_error=read_error,.user_data=r};
    inputs.reader.compressed=(PharaohCompressedChunkState){.explode_tables=&pharaoh_pk_explode_tables,
        .crc_table=pharaoh_compression_crc_table,.report_error=codec_error,.error_user_data=r};
    inputs.reader.route_images=r->route_images;inputs.reader.route_image_count=r->image_count;
    r->scene=pharaoh_mission_scene_create(&inputs);
    if(r->scene==NULL) {pharaoh_windows_mission_runtime_destroy(r);return NULL;}
    pharaoh_mission_scene_bind_briefing_hint(r->scene,&r->hint,&host->text_selection);
    pharaoh_campaign_mission_bind_entry(pharaoh_mission_scene_entry(r->scene),&r->draw);
    pharaoh_windows_startup_host_bind_briefing_renderer(&r->renderer,host,briefing,
        (const uint32_t *)(const void *)&host->settings.is_custom_scenario,&r->hint,
        (const uint8_t *)r->media_path,&r->draw);
    button(r->accept,0,27,27,4,461,0,UINT32_C(0x00418220),1);
    button(&r->difficulty[0],0,17,17,4,481,0,UINT32_C(0x00402e50),0);
    button(&r->difficulty[1],17,17,17,4,285,0,UINT32_C(0x00402e50),1);
    button(r->back,0,31,20,4,359,8,UINT32_C(0x00418350),0);
    button(r->up,0,39,26,6,365,8,UINT32_C(0x004c7e90),0);
    button(r->down,0,39,26,6,365,12,UINT32_C(0x004c7e90),1);
    r->input.user_data=r;r->input.dispatch=dispatch_button;
    return r;
}
void pharaoh_windows_mission_runtime_destroy(PharaohWindowsMissionRuntime *r)
{
    if(r==NULL) return;
    pharaoh_mission_scene_destroy(r->scene);pharaoh_mission_scene_program_destroy(r->program);
    free(r->image_pixels);free(r->image_group_first);
    free(r->sprite_offset_x);free(r->sprite_offset_y);
    free(r->route_images);free(r);
}
const PharaohMissionScene *pharaoh_windows_mission_runtime_scene(const PharaohWindowsMissionRuntime *r)
{return r->scene;}

void pharaoh_windows_mission_runtime_briefing(PharaohWindowsMissionRuntime *r,
    int32_t mouse_x,int32_t mouse_y,uint8_t left_pressed,uint8_t right_pressed,uint8_t left_down)
{
    PharaohBriefingState *b=r->briefing;PharaohWindowsStartupHost *h=r->host;
    b->origin_x=(h->graphics.render_width-640)/2;b->origin_y=(h->graphics.render_height-480)/2;
    b->window_width=h->graphics.render_width;b->window_height=h->graphics.render_height;
    b->current_window_id=*r->window;b->redraw_requested=h->graphics.redraw_requested;
    b->difficulty=h->settings.difficulty;
    /* 0041A180 loads the full mission before reading any scenario goal. The
     * draw controller owns the started test, so populate its goal snapshot
     * after invoking exactly that same bound load on the first frame. */
    if(b->started==0) {
        b->started=1;r->draw.load_selected_mission(r->draw.load_user_data);
        r->image_count=(int32_t)h->loaded_libraries->next_image;
        b->current_window_id=*r->window;
        b->redraw_requested=h->graphics.redraw_requested;
        if(b->current_window_id==1) return;
    }
    if(r->io_failed || pharaoh_mission_scene_status(r->scene)<0) return;
    const uint8_t *m=pharaoh_mission_scene_storage(r->scene)->view.metadata;
    PharaohBriefingScenario goals;
    memcpy(&goals.population_enabled,m+0x380,4);memcpy(&goals.population,m+0x384,4);
    memcpy(&goals.houses,m+0x35c,4);memcpy(&goals.houses_level,m+0x360,4);
    memcpy(goals.rating_values,m+0x34c,16);memcpy(goals.rating_enabled,m+0x364,4);
    pharaoh_briefing_draw(b,&goals,&r->draw);
    r->input.cursor_x=mouse_x;r->input.cursor_y=mouse_y;r->input.left_pressed=left_pressed;
    r->input.right_pressed=right_pressed;r->input.left_down=left_down;
    const PharaohBriefingWindowInputOperations input={r,handle_buttons,has_choices,handle_scroll};
    pharaoh_briefing_window_input(*r->window,b->panel_x,b->panel_y,&input);
    if(*r->window!=0x35) return;
    pharaoh_windows_startup_host_draw_image_buttons(h,b->panel_x+564,b->panel_y+410,r->accept,1);
    pharaoh_windows_startup_host_draw_image_buttons(h,b->panel_x+110,b->panel_y+416,r->difficulty,2);
    if(has_choices(r)) pharaoh_windows_startup_host_draw_image_buttons(h,b->panel_x+10,b->panel_y+412,r->back,1);
    if(b->scroll_maximum>0) {
        pharaoh_windows_startup_host_draw_image_buttons(h,b->panel_x+554,b->panel_y+168,r->up,1);
        pharaoh_windows_startup_host_draw_image_buttons(h,b->panel_x+554,b->panel_y+382,r->down,1);
        int32_t percent=b->scroll_position<1?0:b->scroll_position>=b->scroll_maximum?100:
            pharaoh_scale_value_to_percent(b->scroll_position,b->scroll_maximum).value;
        int32_t offset=pharaoh_adjust_by_percentage(b->text_height_blocks*16-77,percent);
        if(b->scroll_drag!=0) offset=r->scroll_drag_position;
        r->draw.draw_background(r->draw.user_data,284,39,
            b->text_x+b->text_width_blocks*16+6,b->text_y+26+offset);
    }
}

static uint32_t navigation_clock(void *data)
{(void)data;return timeGetTime();}

int32_t pharaoh_windows_mission_runtime_game_frame(
    PharaohWindowsMissionRuntime *r,int32_t mouse_x,int32_t mouse_y,
    uint8_t left_down,uint8_t right_went_down)
{
    PharaohWindowsStartupHost *const h=r->host;
    const PharaohSavedSceneStorage *const saved=pharaoh_mission_scene_storage(r->scene);
    const PharaohCityViewContext *const view=pharaoh_mission_scene_view(r->scene);
    uint8_t *surface;
    int32_t pitch;
    r->diagnostic_frame_status=-1;
    if(r->io_failed || pharaoh_mission_scene_status(r->scene)<0) return 0;
    r->diagnostic_frame_status=-2;
    if(rebuild_image_pixels(r)==0) return 0;
    r->diagnostic_frame_status=-3;
    if(pharaoh_windows_startup_host_begin_game_frame(h,&surface,&pitch)==0) return 0;
    r->diagnostic_frame_status=-4;
    r->image_clip.clip_x_min=0;r->image_clip.clip_y_min=0;
    r->image_clip.clip_x_max=h->graphics.render_width;
    r->image_clip.clip_y_max=h->graphics.render_height;
    r->image_clip.window_width=h->graphics.render_width;
    r->image_draw=(PharaohImageDrawContext){
        .images=h->sprite_images,.image_pixels=r->image_pixels,
        .image_count=&r->image_pixel_count,.libraries=h->loaded_libraries->items,
        .state=&r->image_draw_state,.clip=&r->image_clip,
        .surface_pixels=surface,.pitch_bytes=pitch,.tint=&r->tint,
        .solid_color=&h->text_input.drawing.color,.screen_width=&h->graphics.render_width,
        .screen_height=&h->graphics.render_height,.special_image=&r->special_image,
        .pixel_mode=&h->sprite_pixels.pixel_mode,.window_id=r->window,
        .files=&r->files,.pixel_memory=&h->sprite_pixels,
        .external_pixels=&r->external_pixels,.bitmap_name_scratch=r->bitmap_name};
    h->game_image_draw=&r->image_draw;
    const PharaohCityMessageScenarioReset *const messages=pharaoh_mission_scene_messages(r->scene);
    const PharaohWindowsCityUiScene ui_scene={saved,view,
        pharaoh_mission_scene_build_menu(r->scene),pharaoh_mission_scene_current_overlay(r->scene),
        messages->messages_used,messages->count_problem,&r->tint};
    if(pharaoh_windows_city_ui_begin_frame(h,&r->image_draw,&ui_scene,
        r->window,r->image_group_first)==0) {
        pharaoh_windows_startup_host_end_game_frame(h);
        return 0;
    }
    /* Native city branch orders camera scrolling before sidebar navigation.
       Full menu/construction dispatch is a separate pending owner. */
    {
        const PharaohGameTickGateOperations clock={r,navigation_clock};
        const PharaohCameraScrollInput scroll={&mouse_x,&mouse_y,&h->graphics.render_width,
            &h->graphics.render_height,&h->city_scroll_requested,&r->navigation_west,
            &r->navigation_east,&r->navigation_north,&r->navigation_south,
            &r->navigation_tick,&h->settings.scroll_speed,&clock};
        PharaohCameraScrollContext camera;
        pharaoh_mission_scene_bind_camera(r->scene,&camera);
        pharaoh_camera_scroll_apply(&camera,pharaoh_camera_scroll_direction(&scroll));
        (void)pharaoh_windows_city_ui_navigation(h,mouse_x,mouse_y,left_down,right_went_down);
    }
    bind_ground_scan(r,view->state);
    {
        const uint32_t now=GetTickCount();
        const uint32_t delta=now-r->last_animation_tick;
        r->advance_animation=0;
        if((delta&UINT32_C(0x80000000))!=0 || delta>60) {
            r->last_animation_tick=now;r->advance_animation=1;
        }
        pharaoh_animation_frame_state_update(&r->animation_frames,now);
    }
    memset(&r->ground_operations,0,sizeof(r->ground_operations));
    pharaoh_city_ground_bind_sprite_draw(&r->ground_operations,&r->image_draw);
    {
        const PharaohSpriteGroupRecord *const groups=
            (const PharaohSpriteGroupRecord *)(const void *)h->sprite_tags;
        /* Native globals F56BD4/F56BDC/F56D24/F57224/F572EC are the
         * first-image members of groups 516/517/558/718/743. The cached
         * ImageCollectionObject audit fixes the image-field base F55BB4. */
        r->water_image=groups[516].first_image;
        r->cleared_image=groups[517].first_image;
        r->water_variant_a=groups[558].first_image;
        r->water_variant_b=groups[718].first_image;
        r->royal_tomb_image=groups[743].first_image;
    }
    r->marked_tile=UINT32_C(0xffffffff);
    r->ground=(PharaohCityGroundContext){
        .scan=&r->ground_scan,.cells=(const uint32_t *)(const void *)view->projection->cells,
        .neighbor_cells=(const uint32_t *)(const void *)view->projection->cells,
        .building_ids=saved->view.building_ids,.walker_ids=saved->view.walker_ids,
        .images=saved->view.images,.terrain=saved->view.terrain,
        .footprints=saved->view.properties,.property_flags=saved->view.edges,
        .buildings=saved->view.buildings,.walkers=saved->view.walkers,
        .current_image=&h->bitmap_current_glyph,.ambient_scores=r->ambient_scores,
        .ambient_total=&r->ambient_total,.tint=&r->tint,.marked_tile=&r->marked_tile,
        .advance_animation=&r->advance_animation,.debug_invalid=&r->debug_invalid,
        .overlay=(const uint32_t *)(const void *)pharaoh_mission_scene_current_overlay(r->scene),
        .marked_x=&r->marked_x,.marked_y=&r->marked_y,
        .alternate_overlay=&r->alternate_overlay,
        .water_image=&r->water_image,.water_variant_a=&r->water_variant_a,
        .water_variant_b=&r->water_variant_b,.cleared_image=&r->cleared_image,
        .royal_tomb_image=&r->royal_tomb_image,
        .sweep_countdown=saved->destinations[131],.sweep_start=saved->destinations[107],
        .building_sound_ids=r->building_sound_ids,.sound_suppressed=&r->sound_suppressed,
        .sound_views=r->sound_views,.operations=&r->ground_operations};
    r->image_clip.clip_x_min=view->state->viewport.screen_x;
    r->image_clip.clip_y_min=view->state->viewport.screen_y;
    r->image_clip.clip_x_max=view->state->viewport.screen_right;
    r->image_clip.clip_y_max=view->state->viewport.screen_bottom;
    pharaoh_city_draw_ground(&r->ground);
    {
        const PharaohSpriteGroupRecord *const groups=
            (const PharaohSpriteGroupRecord *)(const void *)h->sprite_tags;
        r->building_animations=(PharaohBuildingAnimationContext){&r->image_draw,groups,saved->view.walkers};
        r->building_stock=(PharaohBuildingStockDrawContext){saved->view.buildings,groups,
            &r->image_draw,(const uint32_t *)(const void *)saved->view.city_orientation};
        r->monument_spot_draw=(PharaohMonumentSpotDrawContext){
            .scripts=pharaoh_monument_animation_scripts,
            .script_count=PHARAOH_MONUMENT_ANIMATION_SCRIPT_COUNT,
            .image_group_first=r->image_group_first,
            .sprite_offset_x=r->sprite_offset_x,.sprite_offset_y=r->sprite_offset_y,
            .image_record_count=r->image_pixel_count,
            .map_orientation=*saved->view.city_orientation};
        r->monument_spot_operations=(PharaohMonumentSpotDrawOperations){r,top_draw_image};
        r->monument_spots=(PharaohMonumentSpotGroupDrawContext){
            .draw=&r->monument_spot_draw,.sphinx=saved->monument_data.sphinx,
            .sun_temple=saved->monument_data.sun_temple,
            .library=saved->monument_data.library,.abu_simbel=saved->monument_data.abu_simbel,
            .caesareum=saved->monument_data.caesareum,
            .lighthouse=saved->monument_data.lighthouse,
            .royal_tomb=saved->monument_data.royal_groups,
            .buildings=saved->view.buildings,
            .royal_tomb_spot_table=top_royal_tomb_table,
            .operations=&r->monument_spot_operations,
            .library_position_variant=(int32_t *)(void *)&saved->monument_data.library_position_variant};
        r->sun_temple=(PharaohSunTempleDrawContext){saved->view.buildings,
            (int32_t)groups[568].first_image,r,top_draw_image};
        r->obelisk=(PharaohObeliskDrawContext){
            .buildings=saved->view.buildings,.spot_draw=&r->monument_spot_draw,
            .small_spots=saved->monument_data.small_obelisk,
            .large_spots=saved->monument_data.large_obelisk,
            .operations=&r->monument_spot_operations,.user_data=r,
            .progress=top_obelisk_progress,.ground_image_first=(int32_t)groups[568].first_image};
        r->abu_simbel=(PharaohAbuSimbelDrawContext){
            .buildings=saved->view.buildings,.map_orientation=*saved->view.city_orientation,
            .part_a_image_first=(int32_t)groups[826].first_image,
            .part_b_image_first=(int32_t)groups[827].first_image,
            .user_data=r,.built_parts=top_abu_simbel_built_parts,.draw_image=top_draw_image};
        r->granary_draw=(PharaohGranaryDrawContext){
            .buildings=saved->view.buildings,.edge_flags=saved->view.properties,
            .food_count=pharaoh_mission_scene_food_count(r->scene),
            .food_list=pharaoh_mission_scene_food_ids(r->scene),
            .food_pile_image_first=(int32_t)groups[283].first_image,
            .user_data=r,.determine_available_foods=top_determine_foods,
            .draw_image=top_draw_image};
        r->temple_annex=(PharaohTempleAnnexDrawContext){
            .buildings=saved->view.buildings,
            .parts=(const PharaohTempleAnnexPart *)(const void *)
                pharaoh_temple_image_tables.gate_parts,
            .orientation_parts=(const int32_t *)(const void *)
                &pharaoh_temple_image_tables.gate_order[0][0],
            .god_image_groups=(const int32_t *)(const void *)
                pharaoh_temple_image_tables.gate_gods,
            .image_group_first=r->image_group_first,.draw=&r->image_draw,
            .map_orientation=*saved->view.city_orientation,
            .statue_image_first=(int32_t)groups[489].first_image,
            .tint=&r->tint,.user_data=r,
            .god_with_status=top_god_with_status,.project_tile=top_project_tile};
        pharaoh_pyramid_coordinate_tables_init(&r->pyramid_coordinate_storage,
            &pharaoh_pyramid_image_initial_data);
        r->pyramid_walker=(PharaohPyramidWalkerDrawContext){
            .walkers=saved->view.walkers,.buildings=saved->view.buildings,
            .view=view->state,.projection=view->projection,.bounds=view->map_bounds,
            .tables=&r->pyramid_coordinate_storage.tables,
            .pyramid_style=(const uint32_t *)(const void *)
                pharaoh_mission_scene_pyramid_style(r->scene),
            .anchor_orientation=(const uint32_t *)(const void *)
                saved->view.city_orientation};
        r->walker_overlay=(PharaohWalkerOverlayContext){
            .overlay=pharaoh_mission_scene_current_overlay(r->scene),.current_player=saved->view.current_player,
            .walkers=saved->view.walkers,.buildings=saved->view.buildings,
            .players=saved->view.players};
        r->walker_operations=(PharaohWalkerDrawOperations){
            .user_data=r,.pyramid_walker_offset=top_pyramid_walker_offset,
            .draw_number=top_walker_number};
        r->walker_draw=(PharaohWalkerDrawContext){
            .walkers=saved->view.walkers,
            .formations=(const PharaohFormationDrawRecord *)saved->destinations[24],
            .groups=groups,.draw=&r->image_draw,.overlay=&r->walker_overlay,
            .orientation=(const uint32_t *)(const void *)saved->view.city_orientation,
            .tile_half_width=&view->state->viewport.tile_half_width,
            .tile_half_height=&view->state->viewport.tile_half_height,
            .selected_x=&r->selected_walker_x,.selected_y=&r->selected_walker_y,
            .skip_draw=&r->skip_walker_draw,.operations=&r->walker_operations};
        r->pyramid_draw=(PharaohPyramidDrawContext){
            .buildings=saved->view.buildings,.building_ids=saved->view.building_ids,
            .walker_ids=saved->view.walker_ids,.random=saved->view.random,
            .groups=groups,.draw=&r->image_draw,.walker=&r->walker_draw,
            .layout=&pharaoh_pyramid_image_initial_data,
            .style=(const uint32_t *)(const void *)
                pharaoh_mission_scene_pyramid_style(r->scene),
            .orientation=(const uint32_t *)(const void *)saved->view.city_orientation,
            .tile_half_width=&view->state->viewport.tile_half_width,
            .tile_half_height=&view->state->viewport.tile_half_height};
        r->animation_offset=(PharaohBuildingAnimationOffsetContext){
            .building_ids=saved->view.building_ids,.sprites=saved->destinations[14],
            .buildings=saved->view.buildings,.models=h->building_models,
            .images=h->sprite_images,.image_count=r->image_pixel_count,
            .animation_should_advance=r->animation_frames.update_due,.cheat_allow_all=0,
            .user_data=r,.is_floodplain_farm=animation_is_floodplain_farm,
            .houses_water_in_radius=top_houses_water,.is_palace=top_is_palace,
            .main_building=top_main_building};
        /* Native mission start resets gameTimeDay and gameTimeTick to zero.
         * Their simulation-tick owner is not connected in this runtime yet. */
        r->farm_tiles=(PharaohFarmTileDrawContext){
            .surface=pharaoh_mission_scene_surface(r->scene),.draw=&r->image_draw,
            .meadow_farm_image_first=(int32_t)groups[494].first_image,
            .tile_half_width=view->state->viewport.tile_half_width,
            .tile_half_height=view->state->viewport.tile_half_height,
            .tile_height=view->state->viewport.tile_height,.user_data=r,
            .has_water_access=top_farm_has_water_access,
            .sprites=saved->destinations[14],
            .floodplain_farm_image_first=(int32_t)groups[306].first_image,
            .laborer_work_image_first=(int32_t)groups[157].first_image,
            .laborer_grown_image_first=(int32_t)groups[158].first_image,
            .laborer_harvest_image_first=(int32_t)groups[159].first_image,
            .farm_max_progress=2000,
            .flood_state=(int32_t)pharaoh_mission_scene_flood_state(r->scene),
            .game_day=(int32_t)r->farm_game_day,.game_tick=r->farm_game_tick,
            .locusts_countdown=*(const int32_t *)saved->destinations[40],
            .reed_gatherer_image_first=(int32_t)groups[293].first_image,
            .cattle_ranch_image_first=(int32_t)groups[374].first_image,
            .crop_image_first={
                (int32_t)groups[369].first_image,(int32_t)groups[463].first_image,
                (int32_t)groups[325].first_image,(int32_t)groups[282].first_image,
                (int32_t)groups[322].first_image,(int32_t)groups[324].first_image,
                (int32_t)groups[384].first_image,(int32_t)groups[384].first_image,
                (int32_t)groups[384].first_image,(int32_t)groups[715].first_image},
            .is_on_floodplain=animation_is_floodplain_farm,
            .is_near_water=top_farm_near_water};
        memset(&r->top_operations,0,sizeof(r->top_operations));
        r->top_operations.user_data=r;
        r->top_operations.draw_pyramid=top_draw_pyramid;
        r->top_operations.is_farm=top_is_farm;
        r->top_operations.draw_farm=top_draw_farm;
        r->top_operations.draw_0041fff0=top_sphinx_spots;
        r->top_operations.draw_abu_simbel=top_abu_simbel_built;
        r->top_operations.draw_004e5e90=top_obelisk;
        r->top_operations.draw_00542e10=top_sun_temple_blocks;
        r->top_operations.draw_00420050=top_sun_temple_spots;
        r->top_operations.draw_004206f0=top_library_spots;
        r->top_operations.draw_00420a30=top_abu_simbel_spots;
        r->top_operations.draw_00420d60=top_caesareum_spots;
        r->top_operations.draw_00421060=top_lighthouse_spots;
        r->top_operations.draw_00421920=top_royal_tomb_spots;
        r->top_operations.draw_walker=top_draw_walker;
        r->top_operations.draw_granary_stores=top_granary_stores;
        r->top_operations.draw_004c0160=top_temple_annex;
        r->top_operations.animation_offset=top_animation_offset;
        r->top_operations.is_floodplain_farm=top_is_floodplain_farm;
        r->top_operations.draw_building_details=top_building_stock;
        r->top=(PharaohCityTopContext){.ground=&r->ground,.draw=&r->image_draw,
            .animations=&r->building_animations,.sprites=saved->destinations[14],
            .orientation=(const uint32_t *)(const void *)saved->view.city_orientation,
            .pass_sequence=&r->top_pass_sequence,.operations=&r->top_operations};
        pharaoh_city_draw_top(&r->top,0);
        r->high_walker_operations=(PharaohCityObjectLayerOperations){
            .user_data=r,.draw_walker=top_draw_walker};
        r->high_walkers=(PharaohCityObjectLayerContext){
            .ground=&r->ground,.cycle_enabled=&r->walker_layer_cycle_enabled,
            .cycle_counter=&r->walker_layer_cycle_counter,
            .operations=&r->high_walker_operations};
        pharaoh_city_draw_high_walkers(&r->high_walkers,0);
    }
    r->image_clip.clip_x_min=0;r->image_clip.clip_y_min=0;
    r->image_clip.clip_x_max=h->graphics.render_width;
    r->image_clip.clip_y_max=h->graphics.render_height;
    pharaoh_windows_city_ui_draw_foreground(h);
    pharaoh_windows_startup_host_end_game_frame(h);
    r->diagnostic_frame_status=1;
    return 1;
}

int32_t pharaoh_windows_mission_runtime_diagnostic_begin_game(PharaohWindowsMissionRuntime *r)
{
    /* Diagnostic fixture: the first campaign mission, as selected by the
     * Begin Family History path. Do not inherit a saved custom-map setting. */
    r->host->settings.is_custom_scenario=0;
    r->host->settings.current_mission_id=0;
    r->host->settings.mission_id=0;
    if(r->briefing->started==0){
        r->briefing->started=1;
        r->draw.load_selected_mission(r->draw.load_user_data);
    }
    if(r->io_failed||pharaoh_mission_scene_status(r->scene)<0)return 0;
    dispatch_button(r,UINT32_C(0x00418220));
    return *r->window==1;
}

int32_t pharaoh_windows_mission_runtime_diagnostic_frame_status(const PharaohWindowsMissionRuntime *r)
{return r->diagnostic_frame_status;}
