#include "pharaoh/ui/building_info_action_bindings.h"
#include "pharaoh/ui/window_state_actions.h"
#include "pharaoh/ui/small_button_callbacks.h"
#include "pharaoh/runtime/platform_measure_speed.h"
#include <stddef.h>
#include <stdlib.h>

_Static_assert(sizeof(PharaohInfoSelectedBuildingPlayer)==0x49d8,
    "native EA5EE4 player stride");
_Static_assert(offsetof(PharaohBuildingTypeRecord,storage_id)==0xc1 &&
    sizeof(PharaohBuildingStorageRecord)==196 &&
    offsetof(PharaohBuildingStorageRecord,resource_state)==10,
    "native selection storage fields");
_Static_assert(offsetof(PharaohBuildingInfoLiveState,details_shown)==0x8c &&
    offsetof(PharaohBuildingInfoLiveState,help_id)==0x9c,
    "native EADD44/EADD54 info fields");

enum InputSource { INPUT_GENERIC,INPUT_IMAGE,INPUT_SQUARE };

static PharaohBuildingInfoActionResult action_result(
    enum PharaohBuildingInfoActionSupport support,uint32_t token,const char *field)
{ const PharaohBuildingInfoActionResult r={support,token,field};return r; }
static int same(const volatile void *a,const volatile void *b)
{ return a==b; }
static int32_t signed_bits(uint32_t value)
{ return value<=INT32_MAX?(int32_t)value:(int32_t)((int64_t)value-INT64_C(0x100000000)); }

static _Noreturn void divide_fault(const PharaohBuildingInfoActionProviders *p)
{
    p->divide_fault(p->divide_fault_user_data,0x004cde60u,
        PHARAOH_INFO_INIT_SIGNED_DIVIDE_OVERFLOW);
    abort(); /* A returning platform endpoint violated its required contract. */
}
static void percentage_status(const PharaohBuildingInfoActionProviders *p,
    PharaohPercentageScaleStatus status)
{ if(status!=PHARAOH_PERCENTAGE_SCALE_VALUE)divide_fault(p); }

static PharaohLegionOrderBorrowedState actual_order_fields(
    const PharaohUnitCommandUiContext *c,uint32_t id)
{
    PharaohNativeLegionRecord *f=&c->orders->formations[id];
    uint8_t *bytes=(uint8_t *)(void *)f;
    const PharaohLegionOrderBorrowedState s={id,
        {&f->strength_class,f->member_ids,&f->morale_leader_id,&f->movement_mode,&f->morale,
         &f->previous_movement_mode,(int16_t *)(void *)(bytes+124),
         (const int16_t *)(const void *)(bytes+126),&f->deployment_status,
         &f->months_low_morale,&f->enemy_type,&f->months_very_low_morale,
         bytes+119,(uint32_t *)(void *)(bytes+140)},
        {(uint8_t *)(void *)c->orders->walkers,sizeof(*c->orders->walkers),
         0,1,20,22,56,308,316,320},
        c->button_parameter,c->orders->current_walker_id,c->active_formation_id,
        c->orders->command_locked,c->orders->redraw_units,c->orders->redraw_city};
    return s;
}

static void selection_action(const PharaohBuildingInfoActionProviders *p,uint32_t token)
{
    PharaohBuildingInfoLiveContext *u=p->ui;
    PharaohSelectionButtonBorrowedState s={0};
    s.toggle=(uint32_t *)(void *)&u->window->details_shown;
    s.selected_building=(const uint32_t *)(const void *)&u->window->building_id;
    s.button_parameter=(const uint32_t *)(const void *)p->live->button_parameter;
    s.primary_count=u->food_count; /* Native F53DD8 count with F53E70 offsets. */
    s.secondary_count=u->resource_count;
    s.offsets=(const uint32_t *)(const void *)u->resources;
    s.redraw_requested=u->redraw_requested;
    if(token==0x004062d0u){pharaoh_selection_toggle_borrowed(&s);return;}
    const uint8_t owner=u->buildings[u->window->building_id].storage_id;
    if(token==0x00406620u) {
        if(*s.primary_count>0)s.resource_states=u->storage[owner].resource_state;
        pharaoh_selection_mark_primary_borrowed(&s);return;
    }
    const uint32_t parameter=*s.button_parameter;
    if(parameter==1u)s.player_selected_building=
        &p->selected_buildings[*p->current_player].selected_building;
    else if(parameter==2u&&*s.secondary_count>0)
        s.resource_states=u->storage[owner].resource_state;
    pharaoh_selection_apply_borrowed(&s);
}

static void storage_limit(const PharaohBuildingInfoActionProviders *p)
{
    PharaohBuildingInfoLiveContext *u=p->ui;
    PharaohBuildingTypeRecord *b=&u->buildings[u->window->building_id];
    PharaohBuildingStorageRecord *storage=&u->storage[b->storage_id];
    /* Resolve the real owner once; normalized owner0 addresses this selected
     * record directly. The shared whole C algorithm sees its original fields,
     * not fabricated packed arrays or copied storage records. */
    PharaohStorageLimitState s={b->building_type,0,&b->native_e0,
        *p->live->display_mode,*u->resource_count,u->resources,u->foods,
        *p->live->button_secondary_parameter,(uint32_t)*p->live->button_parameter,
        storage->resource_state,storage->accept_amount,storage->get_amount,u->redraw_requested};
    pharaoh_storage_orders_adjust_limit(&s);
}

static void legion_toggle(const PharaohBuildingInfoActionProviders *p)
{
    const PharaohUnitCommandUiContext *c=p->units;
    const uint32_t previous=*c->active_formation_id,current=*c->orders->selected_formation_id;
    /* Native chosen is current in both the equal and unequal cases. */
    PharaohNativeLegionRecord *f=&c->orders->formations[current];
    PharaohLegionOrderBorrowedState previous_button;
    const PharaohLegionOrderBorrowedState *previous_fields=NULL;
    if(*c->orders->command_locked==0&&*p->live->button_secondary_parameter==1) {
        previous_button=actual_order_fields(c,previous);previous_fields=&previous_button;
    }
    const PharaohLegionToggleBorrowedState s={previous,c->orders->command_locked,
        (uint8_t *)(void *)f+119,&f->movement_mode,c->button_parameter,
        (uint32_t *)(void *)&p->ui->window->formation_id,
        (const uint32_t *)(const void *)p->live->button_secondary_parameter,previous_fields};
    pharaoh_legion_order_toggle_borrowed(&s);
}

typedef struct TransportAdapter {
    const PharaohBuildingInfoActionProviders *providers;
    PharaohTransportShipButtonState *state;
} TransportAdapter;

static void transport_assign(void *user,int32_t walker)
{
    const TransportAdapter *a=user;
    *a->providers->units->transport_active=(uint32_t)a->state->selected_transport_active;
    pharaoh_unit_assign_dock((void *)a->providers->units,walker);
}
static void transport_shore(void *user,int32_t x,int32_t y,uint8_t size,
    int16_t *result_x,int16_t *result_y)
{
    const TransportAdapter *a=user;
    PharaohFerryMapContext *shore=a->providers->units->shore;
    *a->providers->units->transport_active=(uint32_t)a->state->selected_transport_active;
    (void)pharaoh_map_water_tile_next_to(shore,x,y,size);
    *result_x=(int16_t)*shore->result_x;*result_y=(int16_t)*shore->result_y;
}
static void transport_action(const PharaohBuildingInfoActionProviders *p,uint8_t returning)
{
    const PharaohUnitCommandUiContext *c=p->units;
    PharaohBuildingInfoLiveContext *u=p->ui;
    const int32_t id=u->window->transport_id;
    /* Both Native parents publish D05700 before ID/lock/percentage checks.
     * The normalized providers discard percentage status; enforce its actual
     * #DE here before their remaining algorithm, after that exact early store.
     * This pure whole percentage child has no other state effects. */
    *c->selected_transport_id=(uint32_t)id;
    if(id>0&&(returning!=0||*c->orders->command_locked==0)) {
        const PharaohWalkerQueryRecord *w=&c->orders->walkers[id];
        const PharaohPercentageScaleResult r=pharaoh_scale_value_to_percent(w->hit_points,w->maximum_hit_points);
        percentage_status(p,r.status);
    }
    PharaohTransportShipButtonState s={c->orders->walkers,id,*c->orders->command_locked,
        *c->button_parameter,signed_bits(*c->selected_transport_id),
        signed_bits(*c->transport_active),*u->current_window,*u->redraw_requested,NULL,transport_assign};
    const TransportAdapter adapter={p,&s};s.callback_context=(void *)&adapter;
    if(returning!=0)
        pharaoh_transport_ship_return_button(&s,c->buildings,c->orders->slot_walkers,
            signed_bits(*c->orders->current_walker_id),transport_shore,(void *)&adapter);
    else pharaoh_transport_ship_button(&s);
    /* Child boundaries above publish the active flag before real dock/shore
     * calls; remaining scalar results follow the complete normalized owner. */
    *c->transport_active=(uint32_t)s.selected_transport_active;
    *u->current_window=s.current_window;*u->redraw_requested=s.redraw_requested;
}

static void show_selected_message(void *user,int32_t message,int32_t background,int32_t templated)
{
    const PharaohBuildingInfoActionProviders *p=user;
    /* The whole511C50 provider always supplies background0. */
    (void)background;
    pharaoh_city_message_dialog_show(p->actions->messages,message,(uint8_t)templated);
}
static void selected_message(const PharaohBuildingInfoActionProviders *p)
{
    PharaohSmallButtonState s={0};
    s.message_id=p->ui->window->help_id;s.open_message=show_selected_message;s.user_data=(void *)p;
    s.redraw_requested=*p->ui->redraw_requested;
    pharaoh_small_button_show_selected_message(&s);
    *p->ui->redraw_requested=s.redraw_requested;
}

static void run_action(const PharaohBuildingInfoActionProviders *p,uint32_t token)
{
    switch(token) {
    case 0x004062d0u:case 0x00406620u:case 0x00406680u:selection_action(p,token);break;
    case 0x004062f0u:pharaoh_building_info_show_orders(&p->ui->window->orders_shown,p->ui->redraw_requested);break;
    case 0x00406300u:pharaoh_building_info_live_toggle_resource(p->ui,(uint32_t)*p->live->button_parameter);break;
    case 0x00406460u:storage_limit(p);break;
    case 0x00406700u:pharaoh_building_info_actions_select_walker(p->actions);break;
    case 0x00406720u:pharaoh_unit_legion_order_button(p->units);break;
    case 0x00406b40u:transport_action(p,0);break;
    case 0x00406c90u:percentage_status(p,pharaoh_unit_warship_button(p->units));break;
    case 0x00406de0u:percentage_status(p,pharaoh_unit_warship_return_button(p->units));break;
    case 0x00406e80u:transport_action(p,1);break;
    case 0x00406fb0u:pharaoh_legion_return_button_native(p->units->orders);break;
    case 0x00407110u:legion_toggle(p);break;
    case 0x004071c0u:pharaoh_unit_legion_exit(p->units);break;
    case 0x0040a0e0u:percentage_status(p,pharaoh_advisors_show_checked(p->advisors));break;
    case 0x0040aa50u:pharaoh_building_info_actions_modal_40aa50(p->actions);break;
    case 0x0040ad30u:pharaoh_building_info_actions_modal_40ad30(p->actions);break;
    case 0x0040c6b0u:pharaoh_storage_orders_scroll((uint32_t)*p->live->button_parameter,
        *p->live->display_mode,*p->ui->resource_count,
        &p->ui->buildings[p->ui->window->building_id].native_e0,p->ui->redraw_requested);break;
    case 0x004e2360u:pharaoh_building_info_actions_show_message_help(p->actions);break;
    case 0x00511c50u:selected_message(p);break;
    case 0x0051f780u:pharaoh_actor_walking_no_operation();break;
    case 0x0053d730u:pharaoh_building_info_actions_next_phrase(p->actions);break;
    }
}

static uint32_t input_clock(void *user)
{
    const PharaohBuildingInfoActionBindings *b=user;
    return b->clock(b->clock_user_data);
}

#define NEED(x) do { if(!(x))return action_result(PHARAOH_INFO_ACTION_MISSING_DATA,token,#x); } while(0)
#define CONTEXT(x) do { if(!(x))return action_result(PHARAOH_INFO_ACTION_MISSING_CONTEXT,token,#x); } while(0)
#define ALIAS(a,c) do { if(!same(a,c))return action_result(PHARAOH_INFO_ACTION_STATE_MISMATCH,token,#a " == " #c); } while(0)

static PharaohBuildingInfoActionResult common_support(
    const PharaohBuildingInfoActionBindings *b,uint32_t token,uint8_t installed)
{
    CONTEXT(b);
    const PharaohBuildingInfoActionProviders *p=&b->providers;
    CONTEXT(p->ui);CONTEXT(p->live);
    const PharaohInfoProgramBorrowed *l=p->live;
    NEED(p->ui->window);NEED(p->ui->buildings);NEED(p->ui->walkers);
    NEED(p->ui->formations);NEED(p->ui->storage);NEED(p->ui->current_window);
    NEED(p->ui->redraw_requested);NEED(p->ui->resource_count);NEED(p->ui->resources);
    NEED(p->ui->food_count);NEED(p->ui->foods);NEED(l->display_mode);
    NEED(l->button_parameter);NEED(l->button_secondary_parameter);
    if(same(l->button_parameter,l->button_secondary_parameter))
        return action_result(PHARAOH_INFO_ACTION_STATE_MISMATCH,token,"distinct D3B198/D3B194 parameter cells");
    NEED(l->generic_input);NEED(l->image_input);NEED(l->square_input);
    NEED(l->square_input->repeat_schedule);NEED(b->clock);
    if(p->ui->button_parameter)ALIAS(p->ui->button_parameter,l->button_parameter);
    if(p->ui->generic_input)ALIAS(p->ui->generic_input,l->generic_input);
    if(p->ui->image_input)ALIAS(p->ui->image_input,l->image_input);
    if(p->ui->square_input)ALIAS(p->ui->square_input,l->square_input);
    if(installed!=0) {
        ALIAS(l->generic_input->action_context,b);ALIAS(l->image_input->user_data,b);
        ALIAS(l->square_input->user_data,b);
        if(l->image_input->dispatch!=pharaoh_building_info_action_bindings_image_dispatch ||
            l->square_input->dispatch!=pharaoh_building_info_action_bindings_square_dispatch ||
            l->square_input->clock!=input_clock)
            return action_result(PHARAOH_INFO_ACTION_STATE_MISMATCH,token,"installed info input callbacks");
    }
    return action_result(PHARAOH_INFO_ACTION_READY,token,NULL);
}

static PharaohBuildingInfoActionResult message_support(
    const PharaohBuildingInfoActionProviders *p,uint32_t token)
{
    CONTEXT(p->actions);CONTEXT(p->actions->messages);
    const PharaohCityMessageQueueContext *m=p->actions->messages;
    NEED(m->layouts);NEED(m->dialog);
    const PharaohMessageDialogOpenState *d=m->dialog;
    NEED(d->has_phrases);NEED(d->phrase_count);NEED(d->dialog_words_a);NEED(d->dialog_words_b);
    NEED(d->message_id);NEED(d->message_parameter);NEED(d->dialog_state_aae0);
    NEED(d->dialog_state_b130);NEED(d->dialog_state_b138);NEED(d->dialog_state_b12c);
    NEED(d->dialog_visible);NEED(d->previous_window_id);NEED(d->event54_popup);
    NEED(d->current_window_id);NEED(d->redraw_requested);NEED(d->rich_text);
    ALIAS(d->current_window_id,p->ui->current_window);ALIAS(d->redraw_requested,p->ui->redraw_requested);
    return action_result(PHARAOH_INFO_ACTION_READY,token,NULL);
}
static PharaohBuildingInfoActionResult close_support(
    const PharaohBuildingInfoActionProviders *p,uint32_t token)
{
    CONTEXT(p->actions);CONTEXT(p->actions->dialog_close);
    const PharaohMessageDialogLiveCloseContext *d=p->actions->dialog_close;
    NEED(d->show_video);NEED(d->previous_window);NEED(d->current_window);
    NEED(d->message_category);NEED(d->redraw);NEED(d->video);NEED(d->video_stop);
    NEED(d->video_stop->resolve_media_path);NEED(d->video_stop->close_movie);
    NEED(d->video_stop->close_media_resources);NEED(d->video_stop->report_user_stop);
    NEED(d->video_stop->available_memory);NEED(d->video_stop->report_memory_after_stop);
    ALIAS(d->current_window,p->ui->current_window);ALIAS(d->redraw,p->ui->redraw_requested);
    return action_result(PHARAOH_INFO_ACTION_READY,token,NULL);
}
static PharaohBuildingInfoActionResult voice_support(
    const PharaohBuildingInfoActionProviders *p,uint32_t token)
{
    CONTEXT(p->actions);CONTEXT(p->actions->selection_voice);
    const PharaohInfoWalkerSelectionVoiceContext *v=p->actions->selection_voice;
    ALIAS(v->ui,p->ui);NEED(v->portrait);NEED(v->sound);NEED(v->voice);
    NEED(v->portrait->walkers);NEED(v->portrait->buildings);NEED(v->portrait->portrait_codes);
    NEED(v->portrait->scripts);NEED(v->portrait->selected_scenario);
    ALIAS(v->portrait->walkers,p->ui->walkers);ALIAS(v->portrait->buildings,p->ui->buildings);
    const PharaohWalkerSoundContext *s=v->sound;
    NEED(s->walkers);NEED(s->city_view);NEED(s->projection);NEED(s->map_bounds);
    NEED(s->enabled);NEED(s->effects_enabled);NEED(s->fullscreen);NEED(s->fullscreen_width);
    NEED(s->window_width);NEED(s->screen_height);NEED(s->volume);NEED(s->sample_ids);
    NEED(s->request);NEED(s->device);NEED(s->operations);
    NEED(s->operations->get_windows_version);NEED(s->operations->ail_startup);
    NEED(s->operations->ail_set_preference);NEED(s->operations->wave_out_open);
    NEED(s->operations->output_debug_string);NEED(s->operations->end_spatial_sample);
    NEED(s->operations->allocate_bytes);NEED(s->operations->deallocate_bytes);
    NEED(s->operations->release_sample_handle);NEED(s->operations->free_locked_memory);
    NEED(s->operations->sample_status);NEED(s->operations->end_sample);
    NEED(s->operations->allocate_sample_handle);NEED(s->operations->file_read);
    NEED(s->operations->initialize_sample);NEED(s->operations->set_sample_file);
    NEED(s->operations->set_sample_loop_count);NEED(s->operations->set_sample_pan);
    NEED(s->operations->set_sample_volume);NEED(s->operations->set_sample_reverb);
    NEED(s->operations->start_sample);
    ALIAS(s->walkers,p->ui->walkers);
    NEED(v->voice->stream);NEED(v->voice->voice_names);NEED(v->voice->special_voice_names);
    const PharaohInfoVoiceStreamContext *stream=v->voice->stream;
    NEED(stream->enabled);NEED(stream->volume);NEED(stream->voice_enabled);NEED(stream->prefix);
    NEED(stream->filename_scratch);NEED(stream->device);NEED(stream->operations);
    NEED(stream->operations->ail_close_stream);NEED(stream->operations->ail_stream_status);
    NEED(stream->operations->ail_stream_volume);NEED(stream->operations->ail_set_stream_volume);
    NEED(stream->operations->ail_open_stream);NEED(stream->operations->ail_set_stream_loop_count);
    NEED(stream->operations->ail_start_stream);
    return action_result(PHARAOH_INFO_ACTION_READY,token,NULL);
}
static PharaohBuildingInfoActionResult units_support(
    const PharaohBuildingInfoActionProviders *p,uint32_t token)
{
    CONTEXT(p->units);CONTEXT(p->units->orders);
    /*4071C0 has a real short close path with no formation/order children. */
    if(token==0x004071c0u) {
        const PharaohUnitCommandUiContext *c=p->units;
        NEED(c->orders_open);NEED(c->panel_mode);NEED(c->orders->redraw_city);NEED(c->orders->redraw_units);
        ALIAS(c->orders_open,&p->ui->window->orders_shown);ALIAS(c->panel_mode,&p->ui->window->mode);
        ALIAS(c->orders->redraw_city,p->ui->redraw_requested);ALIAS(c->orders->redraw_units,p->ui->current_window);
        if(*c->orders_open!=0||*c->panel_mode!=4)return action_result(PHARAOH_INFO_ACTION_READY,token,NULL);
    }
    const PharaohUnitCommandUiContext *u=p->units;
    const PharaohNativeLegionOrderContext *o=u->orders;
    NEED(o->formations);NEED(o->walkers);NEED(o->slot_walkers);NEED(o->command_locked);
    NEED(o->button_parameter);NEED(o->current_walker_id);NEED(o->selected_formation_id);
    NEED(o->redraw_units);NEED(o->redraw_city);NEED(o->warnings);
    NEED(o->route.search);NEED(o->route.friendly);NEED(o->line.water);NEED(o->line.terrain);
    NEED(u->buildings);NEED(u->building_grid);NEED(u->shipwright_orders);NEED(u->shore);
    NEED(u->panel_warship_id);NEED(u->selected_warship_id);NEED(u->selected_transport_id);
    NEED(u->warship_active);NEED(u->transport_active);NEED(u->active_formation_id);
    NEED(u->button_parameter);NEED(u->orders_open);NEED(u->panel_mode);
    NEED(u->shore->origin);NEED(u->shore->orientation);NEED(u->shore->terrain);
    NEED(u->shore->shore_patterns);NEED(u->shore->corner_offsets);NEED(u->shore->ring_offsets);
    NEED(u->shore->ring_xy);NEED(u->shore->current_tile);NEED(u->shore->adjacent_flags);
    NEED(u->shore->result_x);NEED(u->shore->result_y);
    ALIAS(o->formations,p->ui->formations);ALIAS(o->walkers,p->ui->walkers);
    ALIAS(u->buildings,p->ui->buildings);ALIAS(u->button_parameter,p->live->button_parameter);
    ALIAS(o->button_parameter,p->live->button_parameter);
    ALIAS(o->selected_formation_id,&p->ui->window->formation_id);
    ALIAS(u->panel_warship_id,&p->ui->window->warship_id);
    ALIAS(u->orders_open,&p->ui->window->orders_shown);ALIAS(u->panel_mode,&p->ui->window->mode);
    ALIAS(o->redraw_units,p->ui->current_window);ALIAS(o->redraw_city,p->ui->redraw_requested);
    if(!p->divide_fault)return action_result(PHARAOH_INFO_ACTION_MISSING_FAULT_ENDPOINT,token,"non-returning #DE endpoint");
    return action_result(PHARAOH_INFO_ACTION_READY,token,NULL);
}

PharaohBuildingInfoActionResult pharaoh_building_info_action_bindings_support(
    const PharaohBuildingInfoActionBindings *b,uint32_t token)
{
    PharaohBuildingInfoActionResult r=common_support(b,token,1);
    if(r.support!=PHARAOH_INFO_ACTION_READY)return r;
    const PharaohBuildingInfoActionProviders *p=&b->providers;
    switch(token) {
    case 0x004062d0u:case 0x004062f0u:case 0x00406300u:case 0x00406460u:
    case 0x00406620u:case 0x0040c6b0u:case 0x0051f780u:break;
    case 0x00406680u:
        NEED(p->selected_buildings);NEED(p->current_player);
        if(p->actions) {
            NEED(p->actions->players);
            ALIAS(p->current_player,p->actions->current_player);
            ALIAS(p->selected_buildings,(const uint8_t *)(const void *)p->actions->players+0x4674);
        }
        break;
    case 0x00406720u:case 0x00406b40u:case 0x00406c90u:case 0x00406de0u:
    case 0x00406e80u:case 0x00406fb0u:case 0x00407110u:case 0x004071c0u:
        return units_support(p,token);
    case 0x00406700u:case 0x0040aa50u:case 0x0040ad30u:case 0x004e2360u:case 0x0053d730u: {
        CONTEXT(p->actions);
        const PharaohBuildingInfoActionsLiveContext *a=p->actions;
        NEED(a->window);NEED(a->walkers);NEED(a->players);NEED(a->current_player);
        NEED(a->button_parameter);NEED(a->current_window);NEED(a->redraw);
        ALIAS(a->window,p->ui->window);ALIAS(a->walkers,p->ui->walkers);
        ALIAS(a->button_parameter,p->live->button_parameter);
        ALIAS(a->current_window,p->ui->current_window);ALIAS(a->redraw,p->ui->redraw_requested);
        if(p->current_player)ALIAS(a->current_player,p->current_player);
        if(token==0x00406700u)return voice_support(p,token);
        if(token==0x0040aa50u||token==0x0040ad30u) {
            NEED(a->popup);NEED(a->mouse_x);NEED(a->mouse_y);
            if(token==0x0040ad30u) {
                NEED(a->native_e38da8);NEED(a->native_e38da4);NEED(a->native_f53f0c);
                NEED(a->native_f53f08);NEED(a->input_transient);
            }
        } else if(token==0x004e2360u) {
            r=message_support(p,token);if(r.support!=PHARAOH_INFO_ACTION_READY)return r;
            return close_support(p,token);
        } else {
            NEED(a->phrase_bank);NEED(p->live->phrase_counts);
            ALIAS(p->live->phrase_counts,a->phrase_bank->weights);
            if(p->ui->phrase_counts)ALIAS(p->ui->phrase_counts,a->phrase_bank->weights);
        }
        break;
    }
    case 0x00511c50u:return message_support(p,token);
    case 0x0040a0e0u: {
        CONTEXT(p->advisors);
        const PharaohAdvisorOpenContext *a=p->advisors;
        NEED(a->tutorial_override);NEED(a->requested_advisor);NEED(a->tutorial_advisors);
        NEED(a->current_window);NEED(a->redraw);NEED(a->active_advisor);NEED(a->saved_advisor);
        NEED(a->animation_step);NEED(a->gift_notification_banks);NEED(a->initialize);
        NEED(a->close_dialog);NEED(a->warnings);
        NEED(a->initialize->labor);NEED(a->initialize->ferry);NEED(a->initialize->taxes);
        NEED(a->initialize->finance);NEED(a->initialize->reasons);NEED(a->initialize->coverage);
        NEED(a->initialize->food);NEED(a->initialize->formations);NEED(a->initialize->walkers);
        NEED(a->initialize->current_player);NEED(a->initialize->naval_77);
        NEED(a->initialize->naval_78);NEED(a->initialize->naval_flagged_78);
        NEED(a->initialize->culture);NEED(a->initialize->prosperity);NEED(a->initialize->kingdom);
        NEED(a->initialize->buildings);NEED(a->initialize->win_house_threshold);NEED(a->initialize->win_house_count);
        ALIAS(a->requested_advisor,p->live->button_parameter);
        ALIAS(a->tutorial_advisors,p->live->tutorial_flags);
        ALIAS(a->current_window,p->ui->current_window);ALIAS(a->redraw,p->ui->redraw_requested);
        ALIAS(a->initialize->buildings,p->ui->buildings);ALIAS(a->initialize->walkers,p->ui->walkers);
        if(p->current_player)ALIAS(a->initialize->current_player,p->current_player);
        if(!p->divide_fault)return action_result(PHARAOH_INFO_ACTION_MISSING_FAULT_ENDPOINT,token,"non-returning #DE endpoint");
        break;
    }
    default:return action_result(PHARAOH_INFO_ACTION_UNKNOWN_TOKEN,token,"info native action catalogue");
    }
    return action_result(PHARAOH_INFO_ACTION_READY,token,NULL);
}

static void invoke(PharaohBuildingInfoActionBindings *b,uint32_t token,enum InputSource source)
{
    if(!b)return;
    b->last_result=common_support(b,token,1);
    if(b->last_result.support!=PHARAOH_INFO_ACTION_READY)return;
    const PharaohInfoProgramBorrowed *l=b->providers.live;
    if(source==INPUT_GENERIC) {
        *l->button_parameter=l->generic_input->selected_primary_parameter;
        *l->button_secondary_parameter=l->generic_input->selected_secondary_parameter;
    } else if(source==INPUT_IMAGE) {
        *l->button_parameter=signed_bits(l->image_input->primary_parameter);
        *l->button_secondary_parameter=signed_bits(l->image_input->secondary_parameter);
    } else {
        *l->button_parameter=signed_bits(l->square_input->primary_parameter);
        *l->button_secondary_parameter=signed_bits(l->square_input->secondary_parameter);
    }
    b->last_result=pharaoh_building_info_action_bindings_support(b,token);
    if(b->last_result.support==PHARAOH_INFO_ACTION_READY)run_action(&b->providers,token);
    /* These normalized output fields view the same two Native DWORDs.
     * A real action such as407110 may change them; never republish stale
     * pre-action arguments over those Native stores on return to the host. */
    l->generic_input->selected_primary_parameter=*l->button_parameter;
    l->generic_input->selected_secondary_parameter=*l->button_secondary_parameter;
    l->image_input->primary_parameter=(uint32_t)*l->button_parameter;
    l->image_input->secondary_parameter=(uint32_t)*l->button_secondary_parameter;
    l->square_input->primary_parameter=(uint32_t)*l->button_parameter;
    l->square_input->secondary_parameter=(uint32_t)*l->button_secondary_parameter;
}

#define ACTION(name,token) static void name(void *b){invoke(b,token,INPUT_GENERIC);}
ACTION(action_4062d0,0x004062d0u) ACTION(action_4062f0,0x004062f0u)
ACTION(action_406300,0x00406300u) ACTION(action_406460,0x00406460u)
ACTION(action_406620,0x00406620u) ACTION(action_406680,0x00406680u)
ACTION(action_406700,0x00406700u) ACTION(action_406720,0x00406720u)
ACTION(action_406b40,0x00406b40u) ACTION(action_406c90,0x00406c90u)
ACTION(action_406de0,0x00406de0u) ACTION(action_406e80,0x00406e80u)
ACTION(action_406fb0,0x00406fb0u) ACTION(action_407110,0x00407110u)
ACTION(action_4071c0,0x004071c0u) ACTION(action_40a0e0,0x0040a0e0u)
ACTION(action_40aa50,0x0040aa50u) ACTION(action_40ad30,0x0040ad30u)
ACTION(action_40c6b0,0x0040c6b0u) ACTION(action_4e2360,0x004e2360u)
ACTION(action_511c50,0x00511c50u) ACTION(action_51f780,0x0051f780u)
ACTION(action_53d730,0x0053d730u)
#undef ACTION

PharaohGenericButtonAction pharaoh_building_info_action_bindings_resolve_compiled(void *user,uint32_t token)
{
    (void)user; /*code lookup only; every adapter still checks runtime support*/
    switch(token) {
#define RESOLVE(token,name) case token:return name;
    RESOLVE(0x004062d0u,action_4062d0) RESOLVE(0x004062f0u,action_4062f0)
    RESOLVE(0x00406300u,action_406300) RESOLVE(0x00406460u,action_406460)
    RESOLVE(0x00406620u,action_406620) RESOLVE(0x00406680u,action_406680)
    RESOLVE(0x00406700u,action_406700) RESOLVE(0x00406720u,action_406720)
    RESOLVE(0x00406b40u,action_406b40) RESOLVE(0x00406c90u,action_406c90)
    RESOLVE(0x00406de0u,action_406de0) RESOLVE(0x00406e80u,action_406e80)
    RESOLVE(0x00406fb0u,action_406fb0) RESOLVE(0x00407110u,action_407110)
    RESOLVE(0x004071c0u,action_4071c0) RESOLVE(0x0040a0e0u,action_40a0e0)
    RESOLVE(0x0040aa50u,action_40aa50) RESOLVE(0x0040ad30u,action_40ad30)
    RESOLVE(0x0040c6b0u,action_40c6b0) RESOLVE(0x004e2360u,action_4e2360)
    RESOLVE(0x00511c50u,action_511c50) RESOLVE(0x0051f780u,action_51f780)
    RESOLVE(0x0053d730u,action_53d730)
#undef RESOLVE
    default:return NULL;
    }
}

PharaohGenericButtonAction pharaoh_building_info_action_bindings_resolve(void *user,uint32_t token)
{
    PharaohBuildingInfoActionBindings *b=user;
    if(!b)return NULL;
    b->last_result=pharaoh_building_info_action_bindings_support(b,token);
    return b->last_result.support==PHARAOH_INFO_ACTION_READY?
        pharaoh_building_info_action_bindings_resolve_compiled(user,token):NULL;
}
void pharaoh_building_info_action_bindings_image_dispatch(void *b,uint32_t token)
{ invoke(b,token,INPUT_IMAGE); }
void pharaoh_building_info_action_bindings_square_dispatch(void *b,uint32_t token)
{ invoke(b,token,INPUT_SQUARE); }

PharaohBuildingInfoActionResult pharaoh_building_info_action_bindings_bind(
    PharaohBuildingInfoActionBindings *b,const PharaohBuildingInfoActionProviders *p)
{
    if(!b)return action_result(PHARAOH_INFO_ACTION_MISSING_CONTEXT,0,"action bindings");
    if(!p)return action_result(PHARAOH_INFO_ACTION_MISSING_CONTEXT,0,"action providers");
    b->providers=*p;
    if(p->live&&p->live->square_input) {
        PharaohSquareButtonInputState *s=p->live->square_input;
        if(s->clock!=input_clock||s->user_data!=b) {
            b->clock=s->clock;b->clock_user_data=s->user_data;
        }
    } else {b->clock=NULL;b->clock_user_data=NULL;}
    b->last_result=common_support(b,0,0);
    if(b->last_result.support!=PHARAOH_INFO_ACTION_READY)return b->last_result;
    PharaohGenericButtonInputState *g=p->live->generic_input;
    PharaohImageButtonInputState *i=p->live->image_input;
    PharaohSquareButtonInputState *s=p->live->square_input;
    g->action_context=b;i->user_data=b;i->dispatch=pharaoh_building_info_action_bindings_image_dispatch;
    s->user_data=b;s->clock=input_clock;s->dispatch=pharaoh_building_info_action_bindings_square_dispatch;
    return b->last_result;
}

#undef NEED
#undef CONTEXT
#undef ALIAS
