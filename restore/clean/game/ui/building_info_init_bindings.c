#include "pharaoh/ui/building_info_init_bindings.h"
#include "pharaoh/core/percentage_scale.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/map/terrain_queries.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

_Static_assert(sizeof(PharaohBuildingInfoInitReports)==0x180,
    "native EADB38..EADCB8 reports");
_Static_assert(offsetof(PharaohBuildingInfoInitReports,mausoleum)==0x20 &&
    offsetof(PharaohBuildingInfoInitReports,lighthouse)==0x50 &&
    offsetof(PharaohBuildingInfoInitReports,caesareum)==0x70 &&
    offsetof(PharaohBuildingInfoInitReports,abu_simbel)==0x90 &&
    offsetof(PharaohBuildingInfoInitReports,library)==0xa0 &&
    offsetof(PharaohBuildingInfoInitReports,sun_temple)==0xd8 &&
    offsetof(PharaohBuildingInfoInitReports,pyramid)==0x110,
    "native report destinations passed by 4F7E50");
_Static_assert(sizeof(PharaohNativeLegionRecord)==0x90 &&
    offsetof(PharaohNativeLegionRecord,kind)==0x42 &&
    offsetof(PharaohNativeLegionRecord,recruit_type)==0x46,
    "4B7590 record view");

static const PharaohBuildingInfoInitProviders *providers(void *user_data)
{ return &((PharaohBuildingInfoInitBindings *)user_data)->providers; }

static _Noreturn void divide_fault(const PharaohBuildingInfoInitProviders *p,
    uint32_t entry,enum PharaohBuildingInfoInitDivideFault kind)
{
    p->divide_fault(p->divide_fault_user_data,entry,kind);
    abort(); /* The required platform endpoint violated its no-return contract. */
}

static void reset_warning(void *v)
{ pharaoh_city_warning_clear_all(providers(v)->warnings); }
static void rebuild_resources(void *v)
{ pharaoh_city_resource_determine_available(providers(v)->resources); }
static int32_t pyramid_from_complex(void *v,int32_t building)
{ return pharaoh_info_pyramid_from_complex(providers(v)->pyramid_selection,building); }
static int32_t building_main(void *v,int32_t building)
{ return pharaoh_building_follow_link(providers(v)->initializer->ui->buildings,building); }
static int32_t percentage(void *v,int32_t value,int32_t denominator)
{
    const PharaohPercentageScaleResult r=pharaoh_scale_value_to_percent(value,denominator);
    if(r.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)
        divide_fault(providers(v),0x004cde60u,PHARAOH_INFO_INIT_SIGNED_DIVIDE_OVERFLOW);
    return r.value;
}
static int32_t any_legion_with_soldiers(void *v)
{
    /* Native BCBA9A points to +42 of record1; end BCD62A excludes record50. */
    const PharaohNativeLegionRecord *r=providers(v)->initializer->ui->formations;
    return pharaoh_native_record90_has_ready_004b7590(
        (const uint8_t *)&r[1],PHARAOH_NATIVE_LEGION_COUNT);
}
static void house_worst_desirability(void *v)
{ pharaoh_info_house_find_worst_desirability(providers(v)->house_worst); }
static void house_evolve_text(void *v,int32_t building)
{ pharaoh_house_update_evolve_text(providers(v)->house_economy,building); }

static int32_t publish_road_result(const PharaohBuildingInfoInitProviders *p,int32_t result)
{
    /* All four whole C children update their result fields only on success;
     * native failure leaves the genuine shared DWORD outputs untouched. */
    if(result) {
        *p->road_result_x=p->road->result_x;
        *p->road_result_y=p->road->result_y;
    }
    return result;
}
static int32_t road(void *v,int32_t x,int32_t y,int32_t size)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    return publish_road_result(p,pharaoh_map_has_road_access(p->road,x,y,size));
}
static int32_t granary_road(void *v,int32_t x,int32_t y)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    return publish_road_result(p,pharaoh_map_has_road_access_granary(p->road,x,y));
}
static int32_t nearby_road(void *v,int32_t x,int32_t y)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    return publish_road_result(p,pharaoh_map_has_road_access_nearby(p->road,x,y));
}
static int32_t temple_road(void *v,int32_t x,int32_t y,int32_t orientation)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    return publish_road_result(p,pharaoh_map_has_road_access_temple_complex(p->road,x,y,orientation));
}
static void walker_message(void *v,int32_t walker)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    const PharaohWalkerPhraseStatus s=pharaoh_walker_phrase_update(p->phrase,walker);
    if(s!=PHARAOH_WALKER_PHRASE_VALUE)
        divide_fault(p,0x0053c100u,s==PHARAOH_WALKER_PHRASE_DIVIDE_BY_ZERO?
            PHARAOH_INFO_INIT_DIVIDE_BY_ZERO:PHARAOH_INFO_INIT_SIGNED_DIVIDE_OVERFLOW);
}
static void advisor_buttons(void *v,uint32_t output[15])
{ pharaoh_info_advisor_buttons(providers(v)->advisors,output); }
static void pyramid_progress(void *v,int32_t building)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    pharaoh_pyramid_build_progress_report(p->pyramid_progress,building,&p->reports->pyramid);
}
static void sun_temple_progress(void *v,int32_t building)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    (void)pharaoh_sun_temple_progress(p->complex_progress,building,p->reports->sun_temple);
}
static void library_progress(void *v,int32_t building)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    (void)pharaoh_library_progress(p->library,building,p->reports->library);
}
static void abu_simbel_progress(void *v,int32_t building)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    (void)pharaoh_abu_simbel_progress(p->initializer->ui->buildings,building,p->reports->abu_simbel);
}
static void caesareum_progress(void *v,int32_t building)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    (void)pharaoh_caesareum_progress(p->caesareum,building,p->reports->caesareum);
}
static void lighthouse_progress(void *v,int32_t building)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    (void)pharaoh_lighthouse_progress(p->lighthouse,building,p->reports->lighthouse);
}
static void mausoleum_progress(void *v,int32_t building)
{
    const PharaohBuildingInfoInitProviders *p=providers(v);
    (void)pharaoh_mausoleum_progress(p->complex_progress,building,p->reports->mausoleum);
}
#define TOMB_ADAPTER(name,index,child) \
static void name(void *v,int32_t building) \
{ const PharaohBuildingInfoInitProviders *p=providers(v); \
  (void)child(p->royal_tombs[index],building,p->reports->royal_tomb); }
TOMB_ADAPTER(small_tomb_progress,0,pharaoh_small_royal_tomb_progress)
TOMB_ADAPTER(medium_tomb_progress,1,pharaoh_medium_royal_tomb_progress)
TOMB_ADAPTER(large_tomb_progress,2,pharaoh_large_royal_tomb_progress)
TOMB_ADAPTER(grand_tomb_progress,3,pharaoh_grand_royal_tomb_progress)
#undef TOMB_ADAPTER

void pharaoh_building_info_init_bindings_bind(PharaohBuildingInfoInitBindings *b,
    const PharaohBuildingInfoInitProviders *p)
{
    if(!b)return;
    if(p)b->providers=*p;else memset(&b->providers,0,sizeof(b->providers));
    const PharaohBuildingInfoLiveInitOperations o={
        b,reset_warning,rebuild_resources,pyramid_from_complex,building_main,
        percentage,any_legion_with_soldiers,house_worst_desirability,house_evolve_text,
        road,granary_road,nearby_road,temple_road,walker_message,advisor_buttons,
        {pyramid_progress,sun_temple_progress,library_progress,abu_simbel_progress,
         caesareum_progress,lighthouse_progress,mausoleum_progress,
         small_tomb_progress,medium_tomb_progress,large_tomb_progress,grand_tomb_progress}
    };
    b->operations=o;
    if(b->providers.initializer)b->providers.initializer->operations=&b->operations;
}

static PharaohBuildingInfoInitResult result(enum PharaohBuildingInfoInitSupport s,
    uint32_t entry,const char *field)
{ const PharaohBuildingInfoInitResult r={s,entry,field,0};return r; }
static int same(const volatile void *a,const volatile void *b)
{ return a==b; }

#define NEED(x,entry) do { if(!(x))return result(PHARAOH_INFO_INIT_MISSING_DATA,entry,#x); } while(0)
#define CONTEXT(x,entry) do { if(!(x))return result(PHARAOH_INFO_INIT_MISSING_CONTEXT,entry,#x); } while(0)
#define ALIAS(a,b,entry) do { if(!same(a,b))return result(PHARAOH_INFO_INIT_STATE_MISMATCH,entry,#a " == " #b); } while(0)
#define DISTINCT(a,b,entry) do { if(same(a,b))return result(PHARAOH_INFO_INIT_STATE_MISMATCH,entry,#a " != " #b); } while(0)

static PharaohBuildingInfoInitResult pyramid_ready(const PharaohPyramidSummaryContext *c,
    const PharaohBuildingInfoLiveContext *ui)
{
    CONTEXT(c,0x004f1310u);
    NEED(c->buildings,0x004f1310u);NEED(c->walkers,0x004f1310u);
    NEED(c->style,0x004f1310u);NEED(c->frontier_count,0x004f1310u);
    NEED(c->frontier_parts,0x004f1310u);NEED(c->part_counts,0x004f1310u);
    NEED(c->mastaba_part_counts,0x004f1310u);NEED(c->layer_counts,0x004f1310u);
    NEED(c->bent_layer_counts,0x004f1310u);NEED(c->mastaba_layer_counts,0x004f1310u);
    NEED(c->bent_casing_totals,0x004f1310u);NEED(c->block_totals,0x004f1310u);
    NEED(c->stepped_block_totals,0x004f1310u);NEED(c->bent_block_totals,0x004f1310u);
    NEED(c->mastaba_block_totals,0x004f1310u);NEED(c->prep_totals,0x004f1310u);
    NEED(c->mastaba_prep_totals,0x004f1310u);NEED(c->pyramid_layout,0x004f1310u);
    NEED(c->mastaba_layout,0x004f1310u);NEED(c->bent_half_layer_bytes,0x004f1310u);
    ALIAS(c->buildings,ui->buildings,0x004f1310u);ALIAS(c->walkers,ui->walkers,0x004f1310u);
    return result(PHARAOH_INFO_INIT_READY,0,NULL);
}
static PharaohBuildingInfoInitResult complex_ready(const PharaohComplexProgressContext *c,
    const PharaohBuildingInfoLiveContext *ui)
{
    CONTEXT(c,0x004df040u);NEED(c->buildings,0x004df040u);NEED(c->walkers,0x004df040u);
    NEED(c->mausoleum_parts,0x004df040u);NEED(c->sun_temple_parts,0x00542ad0u);
    NEED(c->reference_ids,0x00542ad0u);
    if(c->reference_walker_ids!=NULL) {
        for(int32_t i=0;i<13;++i)NEED(c->reference_walker_ids[i],0x00542ad0u);
    } else {
        NEED(c->reference_targets,0x00542ad0u);
        for(int32_t i=0;i<13;++i)NEED(c->reference_targets[i],0x00542ad0u);
    }
    ALIAS(c->buildings,ui->buildings,0x004df040u);ALIAS(c->walkers,ui->walkers,0x004df040u);
    return result(PHARAOH_INFO_INIT_READY,0,NULL);
}
static PharaohBuildingInfoInitResult base_ready(const PharaohBaseMonumentProgress *c,
    const PharaohBuildingInfoLiveContext *ui,uint32_t entry)
{
    CONTEXT(c,entry);NEED(c->buildings,entry);NEED(c->walkers,entry);
    ALIAS(c->buildings,ui->buildings,entry);ALIAS(c->walkers,ui->walkers,entry);
    return result(PHARAOH_INFO_INIT_READY,0,NULL);
}

static PharaohBuildingInfoInitResult resources_ready(
    const PharaohBuildingInfoInitProviders *p,const PharaohBuildingInfoLiveContext *ui)
{
    CONTEXT(p->resources,0x005596d0u);
    const PharaohResourceAvailabilityContext *r=p->resources;
    CONTEXT(r->supply,0x005596d0u);NEED(r->supply->cities,0x005596d0u);
    NEED(r->supply->route_limits,0x005596d0u);NEED(r->supply->allowed_buildings,0x005596d0u);
    NEED(r->supply->players,0x005596d0u);NEED(r->supply->current_player,0x005596d0u);
    NEED(r->events,0x005596d0u);NEED(r->event_count,0x005596d0u);
    NEED(r->resource_count,0x005596d0u);NEED(r->resources,0x005596d0u);
    NEED(r->food_count,0x005596d0u);NEED(r->foods,0x005596d0u);
    ALIAS(r->resource_count,ui->resource_count,0x005596d0u);
    ALIAS(r->resources,ui->resources,0x005596d0u);
    ALIAS(r->food_count,ui->food_count,0x005596d0u);ALIAS(r->foods,ui->foods,0x005596d0u);
    return result(PHARAOH_INFO_INIT_READY,0,NULL);
}

PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_support(
    const PharaohBuildingInfoInitBindings *b)
{
    CONTEXT(b,0x004f7e50u);
    const PharaohBuildingInfoInitProviders *p=&b->providers;
    PharaohBuildingInfoLiveInitContext *c=p->initializer;
    CONTEXT(c,0x004f7e50u);CONTEXT(c->ui,0x004f7e50u);
    PharaohBuildingInfoLiveContext *ui=c->ui;
    NEED(ui->window,0x004f7e50u);NEED(ui->buildings,0x004f7e50u);
    NEED(ui->walkers,0x004f7e50u);NEED(ui->formations,0x004f7e50u);
    NEED(ui->food_count,0x004f7e50u);NEED(ui->foods,0x004f7e50u);
    NEED(ui->resource_count,0x004f7e50u);NEED(ui->resources,0x004f7e50u);
    NEED(ui->display_mode,0x004f7e50u);NEED(ui->focus_primary,0x004f7e50u);
    NEED(ui->focus_secondary,0x004f7e50u);NEED(ui->focus_building,0x004f7e50u);
    NEED(ui->current_window,0x004f7e50u);NEED(ui->redraw_requested,0x004f7e50u);
    NEED(c->models,0x004f7e50u);NEED(c->editor_active,0x004f7e50u);
    NEED(c->pending_warning_count,0x004f7e50u);NEED(c->cursor_tile_x,0x004f7e50u);
    NEED(c->cursor_tile_y,0x004f7e50u);NEED(c->grid_start,0x004f7e50u);
    NEED(c->building_ids,0x004f7e50u);NEED(c->walker_ids,0x004f7e50u);
    NEED(c->rubble_types,0x004f7e50u);NEED(c->canals,0x004f7e50u);
    NEED(c->bitfields,0x004f7e50u);NEED(c->sprites,0x004f7e50u);
    NEED(c->terrain,0x004f7e50u);NEED(c->entry_flag,0x004f7e50u);
    NEED(c->exit_flag,0x004f7e50u);NEED(c->neighbours,0x004f7e50u);
    NEED(c->sentry_request,0x004f7e50u);NEED(c->display_height,0x004f7e50u);
    NEED(c->mouse_y,0x004f7e50u);NEED(c->panel_position_mode,0x004f7e50u);
    ALIAS(c->operations,&b->operations,0x004f7e50u);
    ALIAS(b->operations.user_data,b,0x004f7e50u);
    if(!p->divide_fault)return result(PHARAOH_INFO_INIT_MISSING_FAULT_ENDPOINT,
        0x004cde60u,"providers.divide_fault (synchronous non-returning)");

    CONTEXT(p->warnings,0x004e2ab0u);NEED(p->warnings->warning_state,0x004e2ab0u);
    NEED(p->warnings->warning_positions,0x004e2ab0u);NEED(p->warnings->warnings,0x004e2ab0u);
    const PharaohBuildingInfoInitResult resource_status=resources_ready(p,ui);
    if(resource_status.support!=PHARAOH_INFO_INIT_READY)return resource_status;
    const PharaohResourceAvailabilityContext *r=p->resources;

    CONTEXT(p->pyramid_selection,0x004f5d90u);
    const PharaohInfoPyramidSelectionContext *selection=p->pyramid_selection;
    CONTEXT(selection->surface,0x004f5d90u);
    const PharaohMapSurfaceContext *s=selection->surface;
    NEED(selection->part_counts,0x004f5d90u);NEED(selection->mastaba_part_counts,0x004f5d90u);
    NEED(s->map,0x004f5d90u);NEED(s->map->terrain,0x004f5d90u);
    NEED(s->geometry,0x004f5d90u);NEED(s->pyramid_placement,0x004f5d90u);
    NEED(s->pyramid_style,0x004f5d90u);NEED(s->pyramid_complex_offsets,0x004f5d90u);
    NEED(s->buildings,0x004f5d90u);NEED(s->building_ids,0x004f5d90u);
    NEED(s->preview_count,0x004f5d90u); /* Whole ED360 loads it even in mode6. */
    ALIAS(s->buildings,ui->buildings,0x004f5d90u);ALIAS(s->building_ids,c->building_ids,0x004f5d90u);
    ALIAS(s->map->terrain,c->terrain,0x004f5d90u);
    if(s->map->origin!=*c->grid_start)return result(PHARAOH_INFO_INIT_STATE_MISMATCH,
        0x004f5d90u,"pyramid surface map origin == initializer grid_start");

    CONTEXT(p->road,0x00480180u);NEED(p->road->terrain,0x00480180u);
    NEED(p->road->building_ids,0x00480180u);NEED(p->road->buildings,0x00480180u);
    NEED(p->road->regions,0x00480180u);NEED(p->road->ranks,0x00480180u);
    NEED(p->road->ring_offsets,0x00480180u);NEED(p->road->ring_xy,0x00480180u);
    NEED(p->road_result_x,0x00480180u);NEED(p->road_result_y,0x00480180u);
    ALIAS(p->road->terrain,c->terrain,0x00480180u);
    ALIAS(p->road->building_ids,c->building_ids,0x00480180u);
    ALIAS(p->road->buildings,ui->buildings,0x00480180u);
    if(p->road->grid_start!=*c->grid_start)return result(PHARAOH_INFO_INIT_STATE_MISMATCH,
        0x00480180u,"road grid_start == initializer grid_start");

    CONTEXT(p->house_worst,0x0050f0f0u);
    const PharaohInfoWorstDesirabilityContext *w=p->house_worst;
    NEED(w->models,0x0050f0f0u);NEED(w->building_ids,0x0050f0f0u);
    NEED(w->map_width,0x0050f0f0u);NEED(w->map_height,0x0050f0f0u);
    NEED(w->grid_origin,0x0050f0f0u);NEED(w->distance_x,0x0050f0f0u);
    NEED(w->distance_y,0x0050f0f0u);
    ALIAS(w->ui,ui,0x0050f0f0u);ALIAS(w->models,c->models,0x0050f0f0u);
    ALIAS(w->building_ids,c->building_ids,0x0050f0f0u);ALIAS(w->grid_origin,c->grid_start,0x0050f0f0u);
    ALIAS(w->worst_desirability_building,&ui->window->worst_desirability_building,0x0050f0f0u);
    ALIAS(w->worst_desirability_score,&ui->window->worst_desirability_score,0x0050f0f0u);
    CONTEXT(p->house_economy,0x00454190u);
    const PharaohHouseEconomyContext *h=p->house_economy;
    NEED(h->buildings,0x00454190u);NEED(h->models,0x00454190u);
    NEED(h->players,0x00454190u);NEED(h->current_player,0x00454190u);
    NEED(h->food_count,0x00454190u);NEED(h->foods,0x00454190u);
    ALIAS(h->buildings,ui->buildings,0x00454190u);
    ALIAS(h->food_count,ui->food_count,0x00454190u);ALIAS(h->foods,ui->foods,0x00454190u);
    ALIAS(h->negative_influence,&ui->window->worst_desirability_building,0x00454190u);
    ALIAS(h->current_player,r->supply->current_player,0x00454190u);

    CONTEXT(p->phrase,0x0053c100u);
    const PharaohWalkerPhraseContext *f=p->phrase;
    NEED(f->walkers,0x0053c100u);NEED(f->buildings,0x0053c100u);
    NEED(f->players,0x0053c100u);NEED(f->current_player,0x0053c100u);
    NEED(f->models,0x0053c100u);NEED(f->storages,0x0053c100u);
    NEED(f->traders,0x0053c100u);NEED(f->resources,0x0053c100u);
    NEED(f->route_limits,0x0053c100u);NEED(f->route_traded,0x0053c100u);
    NEED(f->export_keep,0x0053c100u);NEED(f->events,0x0053c100u);
    NEED(f->event_count,0x0053c100u);NEED(f->invasion_search_flag,0x0053c100u);
    NEED(f->current_event_index,0x0053c100u);NEED(f->calendar,0x0053c100u);
    NEED(f->calendar->scenario_start_year,0x0053c100u);
    NEED(f->calendar->current_year,0x0053c100u);NEED(f->calendar->current_month,0x0053c100u);
    NEED(f->occupancy,0x0053c100u);NEED(f->occupancy->offsets,0x0053c100u);
    NEED(f->occupancy->terrain,0x0053c100u);NEED(f->occupancy->objects,0x0053c100u);
    NEED(f->processing_mode,0x0053c100u);NEED(f->native_bcb9b4,0x0053c100u);
    NEED(f->random_second_7,0x0053c100u);NEED(f->native_d38374,0x0053c100u);
    NEED(f->native_d386dc,0x0053c100u);NEED(f->weights,0x0053c100u);
    ALIAS(f->walkers,ui->walkers,0x0053c100u);ALIAS(f->buildings,ui->buildings,0x0053c100u);
    ALIAS(f->models,c->models,0x0053c100u);ALIAS(f->storages,ui->storage,0x0053c100u);
    ALIAS(f->current_player,h->current_player,0x0053c100u);
    ALIAS(f->events,r->events,0x0053c100u);ALIAS(f->event_count,r->event_count,0x0053c100u);
    ALIAS(f->shared_result,&ui->window->walker_phrase,0x0053c100u);
    ALIAS(f->distance_x,w->distance_x,0x0053c100u);ALIAS(f->distance_y,w->distance_y,0x0053c100u);
    DISTINCT(w->distance_x,w->distance_y,0x0050f0f0u);
    DISTINCT(p->road_result_x,p->road_result_y,0x00480180u);
    DISTINCT(p->road_result_x,w->distance_x,0x00480180u);
    DISTINCT(p->road_result_x,w->distance_y,0x00480180u);
    DISTINCT(p->road_result_y,w->distance_x,0x00480180u);
    DISTINCT(p->road_result_y,w->distance_y,0x00480180u);
    ALIAS(f->occupancy->terrain,c->terrain,0x0053c100u);
    if(f->occupancy->grid_origin!=(uint32_t)*c->grid_start)
        return result(PHARAOH_INFO_INIT_STATE_MISMATCH,0x0053c100u,
            "phrase occupancy grid_origin == initializer grid_start");

    CONTEXT(p->reports,0x004f1310u);
    PharaohBuildingInfoInitResult status=pyramid_ready(p->pyramid_progress,ui);
    if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    status=complex_ready(p->complex_progress,ui);
    if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    status=base_ready(p->library,ui,0x0040d940u);if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    status=base_ready(p->caesareum,ui,0x00417050u);if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    status=base_ready(p->lighthouse,ui,0x004e6710u);if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    const uint32_t tomb_entries[4]={0x00545740u,0x00558be0u,0x00555e80u,0x0054f470u};
    for(int32_t i=0;i<4;++i) {
        status=base_ready(p->royal_tombs[i],ui,tomb_entries[i]);
        if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    }
    ALIAS(p->pyramid_progress->style,s->pyramid_style,0x004f1310u);
    ALIAS(p->pyramid_progress->part_counts,selection->part_counts,0x004f1310u);
    ALIAS(p->pyramid_progress->mastaba_part_counts,selection->mastaba_part_counts,0x004f1310u);

    CONTEXT(p->advisors,0x00515130u);
    const PharaohInfoAdvisorButtonsContext *a=p->advisors;
    NEED(a->models,0x00515130u);NEED(a->events,0x00515130u);
    NEED(a->event_count,0x00515130u);NEED(a->resource_advisors,0x00515130u);
    CONTEXT(a->requirements,0x00515130u);
    PharaohMonumentBuildRequirements *q=a->requirements;
    NEED(q->definitions,0x00515130u);CONTEXT(q->demand,0x00515130u);
    NEED(q->demand->players,0x00515130u);NEED(q->demand->current_player,0x00515130u);
    NEED(q->demand->planned_kinds,0x00515130u);
    ALIAS(a->ui,ui,0x00515130u);ALIAS(a->models,c->models,0x00515130u);
    ALIAS(a->events,r->events,0x00515130u);ALIAS(a->event_count,r->event_count,0x00515130u);
    ALIAS(q->demand->complex,p->complex_progress,0x00515130u);
    ALIAS(q->demand->pyramid,p->pyramid_progress,0x00515130u);
    ALIAS(q->demand->players,r->supply->players,0x00515130u);
    ALIAS(q->demand->current_player,h->current_player,0x00515130u);
    ALIAS(q->library,p->library,0x00515130u);
    ALIAS(q->caesareum,p->caesareum,0x00515130u);
    ALIAS(q->lighthouse,p->lighthouse,0x00515130u);
    status=base_ready(q->library,ui,0x00515130u);if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    status=base_ready(q->caesareum,ui,0x00515130u);if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    status=base_ready(q->lighthouse,ui,0x00515130u);if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    status=base_ready(q->royal_tombs,ui,0x00515130u);if(status.support!=PHARAOH_INFO_INIT_READY)return status;
    return result(PHARAOH_INFO_INIT_READY,0,NULL);
}

PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_initialize(
    const PharaohBuildingInfoInitBindings *b)
{
    PharaohBuildingInfoInitResult r=pharaoh_building_info_init_bindings_support(b);
    if(r.support==PHARAOH_INFO_INIT_READY)
        r.opened=pharaoh_building_info_live_initialize(b->providers.initializer);
    return r;
}

PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_empty_terrain_support(
    const PharaohBuildingInfoInitBindings *b,const PharaohBuildingInfoEmptyTerrainInitDomain *domain)
{
    CONTEXT(b,0x004f7e50u);
    const PharaohBuildingInfoInitProviders *p=&b->providers;
    const PharaohBuildingInfoLiveInitContext *c=p->initializer;
    CONTEXT(c,0x004f7e50u);CONTEXT(c->ui,0x004f7e50u);
    NEED(c->editor_active,0x004f7e50u);NEED(c->pending_warning_count,0x004f7e50u);
    if(*c->editor_active==1)
        return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: editor arm");
    if(*c->pending_warning_count>0)
        return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: warning/reset arm");
    const PharaohBuildingInfoLiveContext *ui=c->ui;
    NEED(ui->window,0x004f7e50u);NEED(ui->buildings,0x004f7e50u);
    NEED(ui->focus_primary,0x004f7e50u);NEED(ui->focus_secondary,0x004f7e50u);
    NEED(ui->focus_building,0x004f7e50u);NEED(ui->current_window,0x004f7e50u);
    NEED(ui->redraw_requested,0x004f7e50u);
    NEED(c->cursor_tile_x,0x004f7e50u);NEED(c->cursor_tile_y,0x004f7e50u);
    NEED(c->grid_start,0x004f7e50u);NEED(c->building_ids,0x004f7e50u);
    NEED(c->walker_ids,0x004f7e50u);NEED(c->neighbours,0x004f7e50u);
    NEED(c->rubble_types,0x004f7e50u);NEED(c->canals,0x004f7e50u);
    NEED(c->terrain,0x004f7e50u);NEED(c->sprites,0x004f7e50u);
    NEED(c->bitfields,0x004f7e50u);NEED(c->entry_flag,0x004f7e50u);NEED(c->exit_flag,0x004f7e50u);
    NEED(c->display_height,0x004f7e50u);NEED(c->mouse_y,0x004f7e50u);
    NEED(c->panel_position_mode,0x004f7e50u);
    ALIAS(c->operations,&b->operations,0x004f7e50u);ALIAS(b->operations.user_data,b,0x004f7e50u);
    if(b->operations.rebuild_resources!=rebuild_resources)
        return result(PHARAOH_INFO_INIT_STATE_MISMATCH,0x005596d0u,"genuine resources adapter");

    CONTEXT(domain,0x004f7e50u);CONTEXT(domain->map,0x004f7e50u);
    CONTEXT(domain->geometry,0x004f7e50u);
    const PharaohMapTerrainView *map=domain->map;
    const PharaohMapGridGeometry *g=domain->geometry;
    ALIAS(map->terrain,c->terrain,0x004f7e50u);
    if(map->origin!=*c->grid_start || map->width!=g->width || map->height!=g->height)
        return result(PHARAOH_INFO_INIT_STATE_MISMATCH,0x004f7e50u,"empty terrain: actual map origin/dimensions");
    if(map->width<=0||map->width>PHARAOH_MAP_GRID_WIDTH ||
        map->height<=0||map->height>PHARAOH_MAP_GRID_WIDTH ||
        g->diamond_x_min<0||g->diamond_y_min<0 ||
        g->diamond_x_max>=PHARAOH_MAP_GRID_WIDTH||g->diamond_y_max>=PHARAOH_MAP_GRID_WIDTH ||
        g->diamond_x_min>g->diamond_x_max||g->diamond_y_min>g->diamond_y_max ||
        g->diamond_x_center<0||g->diamond_x_center>=PHARAOH_MAP_GRID_WIDTH ||
        g->diamond_y_center<0||g->diamond_y_center>=PHARAOH_MAP_GRID_WIDTH)
        return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: allocated map geometry");
    const int32_t x=*c->cursor_tile_x,y=*c->cursor_tile_y;
    if(x<0||y<0||x>=map->width||y>=map->height)
        return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: actual cursor rectangle");
    /* Accepted arithmetic cannot wrap32: every resulting index lies in the
     * real51984-cell backing. Bounds use actual physical grid coordinates. */
    const int64_t grid=(int64_t)*c->grid_start+x+(int64_t)y*PHARAOH_MAP_GRID_WIDTH;
    if(grid<0||grid>=PHARAOH_MAP_GRID_WIDTH*PHARAOH_MAP_GRID_WIDTH)
        return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: actual cursor grid index");
    const int32_t index=(int32_t)grid;
    if(!pharaoh_map_grid_is_inside(g,index%PHARAOH_MAP_GRID_WIDTH,index/PHARAOH_MAP_GRID_WIDTH))
        return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: actual cursor diamond");
    if(c->building_ids[index]!=0 || ui->buildings[0].building_type!=0)
        return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: grid building0 / record0 sentinel");
    for(int32_t i=0;i<9;++i) {
        const int64_t neighbour=grid+c->neighbours[i];
        if(neighbour<0||neighbour>=PHARAOH_MAP_GRID_WIDTH*PHARAOH_MAP_GRID_WIDTH)
            return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: neighbour allocation index");
        if(c->walker_ids[(int32_t)neighbour]>0)
            return result(PHARAOH_INFO_INIT_OUTSIDE_EMPTY_TERRAIN_DOMAIN,0x004f7e50u,"empty terrain: positive neighbour walker head");
    }
    /* With buildingID0 every terrain_kind return is a terrain value or-1,
     * never the-2 building arm. The whole native classifier remains single.
     * Nine nonpositive heads skip all walker record/phrase reads; the whole
     * initializer clears seven slots, so military selection also stays mode1. */
    return resources_ready(p,ui);
}

PharaohBuildingInfoInitResult pharaoh_building_info_init_bindings_initialize_empty_terrain(
    const PharaohBuildingInfoInitBindings *b,const PharaohBuildingInfoEmptyTerrainInitDomain *domain)
{
    PharaohBuildingInfoInitResult r=pharaoh_building_info_init_bindings_empty_terrain_support(b,domain);
    if(r.support==PHARAOH_INFO_INIT_READY)
        r.opened=pharaoh_building_info_live_initialize(b->providers.initializer);
    return r;
}

#undef NEED
#undef CONTEXT
#undef ALIAS
#undef DISTINCT
