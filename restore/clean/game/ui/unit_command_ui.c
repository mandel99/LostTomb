#include "pharaoh/ui/unit_command_ui.h"
#include "pharaoh/core/static_objects.h"

_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,request_event_id)==306 &&
    offsetof(PharaohWalkerQueryRecord,saved_source_x)==332 &&
    offsetof(PharaohWalkerQueryRecord,transport_order_15c)==348,
    "native command walker fields");
_Static_assert(sizeof(PharaohNativeLegionRecord)==144 &&
    offsetof(PharaohNativeLegionRecord,previous_movement_mode)==98 &&
    offsetof(PharaohNativeLegionRecord,enemy_type)==106 &&
    offsetof(PharaohNativeLegionRecord,reserved_074)==116,
    "native borrowed legion order fields");

static int32_t add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t sub(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a-(uint32_t)b);}
static int32_t sign(int32_t value)
{return value<0?-1:value>0?1:0;}
static uint32_t magnitude(int32_t value)
{
    const uint32_t mask=value<0?UINT32_MAX:0;
    return ((uint32_t)value^mask)-mask;
}
static int32_t square_sum(int32_t x,int32_t y)
{return (int32_t)((uint32_t)x*(uint32_t)x+(uint32_t)y*(uint32_t)y);}

static uint8_t terrain_byte(const PharaohNativeOrderLineContext *c,uint32_t tile)
{return *(const volatile uint8_t *)(const volatile void *)&c->terrain[tile];}

static int land_clear(const PharaohNativeOrderLineContext *c,uint32_t tile)
{
    const uint32_t flags=c->terrain[tile];
    if((flags&0x40u)!=0)return 1;
    if((flags&4u)!=0)return 0;
    static const int32_t delta[8]={-228,228,1,-1,-227,-229,229,227};
    for(uint32_t i=0;i<8;++i)
        if((terrain_byte(c,tile+(uint32_t)delta[i])&4u)!=0)return 0;
    return 1;
}

/* Complete existing Unit00086 C; the real shared native-order x87 helpers
 * retain extended intermediates and inherited control-word precision. */
int32_t pharaoh_unit_land_farthest_on_line(const PharaohNativeOrderLineContext *c,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y,
    int32_t *result_x,int32_t *result_y)
{
    const int32_t delta_x=sub(target_x,x),delta_y=sub(target_y,y);
    const int32_t step_x=sign(delta_x),step_y=sign(delta_y);
    const uint32_t nx=magnitude(delta_x),ny=magnitude(delta_y);
    int32_t best=0,best_x=x,best_y=y,working;
    PharaohFloat80 slope,inverse;
    if(x==target_x && y==target_y){*result_x=x;*result_y=y;return 0;}
    pharaoh_order_line_slopes80(delta_x,delta_y,&slope,&inverse);
    uint32_t row=(uint32_t)*c->water->origin+(uint32_t)y*228u;
    working=y;
    for(int32_t remaining=(int32_t)ny;remaining>0;--remaining){
        if(!pharaoh_order_line_equal_max80(&inverse)){
            working=add(working,step_y);row+=(uint32_t)step_y*228u;
            const int32_t candidate=pharaoh_order_line_intersection80(x,sub(working,y),&inverse);
            if(land_clear(c,row+(uint32_t)candidate)){
                const int32_t metric=square_sum(sub(candidate,x),sub(working,y));
                if(metric>=best){best=metric;best_x=candidate;best_y=working;}
            }
        }
    }
    working=x;
    for(int32_t remaining=(int32_t)nx;remaining>0;--remaining){
        if(!pharaoh_order_line_equal_max80(&slope)){
            working=add(working,step_x);
            const int32_t candidate=pharaoh_order_line_intersection80(y,sub(working,x),&slope);
            const uint32_t tile=(uint32_t)*c->water->origin+(uint32_t)candidate*228u+(uint32_t)working;
            if(land_clear(c,tile)){
                const int32_t metric=square_sum(sub(working,x),sub(candidate,y));
                if(metric>=best){best=metric;best_x=working;best_y=candidate;}
            }
        }
    }
    *result_x=best_x;*result_y=best_y;
    const int32_t actual_x=*result_x;
    row=(uint32_t)*c->water->origin+(uint32_t)best_y*228u+(uint32_t)actual_x;
    if(actual_x!=x || best_y!=y)return best;
    if((terrain_byte(c,row-228u)&4u)==0){*result_y=add(best_y,-1);return best;}
    if((terrain_byte(c,row-227u)&4u)==0){*result_y=add(best_y,-1);*result_x=add(*result_x,1);return best;}
    if((terrain_byte(c,row+228u)&4u)==0){*result_y=add(best_y,1);return best;}
    if((terrain_byte(c,row-229u)&4u)==0){*result_y=add(best_y,-1);*result_x=add(*result_x,-1);return best;}
    if((terrain_byte(c,row+1u)&4u)==0){*result_x=add(actual_x,1);return best;}
    if((terrain_byte(c,row+229u)&4u)==0){*result_y=add(best_y,1);*result_x=add(*result_x,1);return best;}
    if((terrain_byte(c,row-1u)&4u)==0){*result_x=add(actual_x,-1);return best;}
    if((terrain_byte(c,row+227u)&4u)==0){*result_y=add(best_y,1);*result_x=add(*result_x,-1);}
    return best;
}

/* Existing Unit02334 C, with wrapping signed differences and the real
 * percentage algorithm's exceptional result propagated instead of hidden. */
PharaohUnitDirectionResult pharaoh_unit_target_direction(
    int32_t x,int32_t y,int32_t target_x,int32_t target_y)
{
    PharaohUnitDirectionResult result={PHARAOH_PERCENTAGE_SCALE_VALUE,0};
    const int32_t dx=target_x<x?sub(x,target_x):sub(target_x,x);
    const int32_t dy=target_y<y?sub(y,target_y):sub(target_y,y);
    int32_t ratio=100;
    if(dy<dx || dx<dy){
        const PharaohPercentageScaleResult p=dy<dx?
            pharaoh_scale_value_to_percent(dx,dy):pharaoh_scale_value_to_percent(dy,dx);
        result.status=p.status;
        if(p.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return result;
        ratio=dy<dx?p.value:(int32_t)(0u-(uint32_t)p.value);
    }
    if(x==target_x){
        if(target_y<y)return result;
        if(y<target_y){result.direction=4;return result;}
    }
    if(target_x<x){
        if(y==target_y)result.direction=6;
        else if(target_y<y)result.direction=ratio>399?6:(uint8_t)(((uint32_t)(ratio<-399)-1u)&7u);
        else result.direction=ratio>399?6:(uint8_t)((ratio>-400)+4);
    }else if(target_x>x){
        if(y==target_y)result.direction=2;
        else if(y<=target_y)result.direction=ratio>399?2:(uint8_t)((ratio<-399)+3);
        else result.direction=ratio>399?2:(uint8_t)(ratio>-400);
    }
    return result;
}

static uint32_t cursor_tile(const PharaohUnitCommandUiContext *c)
{return (uint32_t)*c->grid_start+(uint32_t)*c->cursor_y*228u+(uint32_t)*c->cursor_x;}
static void redraw(const PharaohUnitCommandUiContext *c)
{*c->orders->redraw_units=1;*c->orders->redraw_city=1;}

/* Both existing data models bind one shared algorithm, with no pool copies. */
void pharaoh_unit_legion_order_button(const PharaohUnitCommandUiContext *c)
{
    const uint32_t id=*c->orders->selected_formation_id;
    PharaohNativeLegionRecord *const f=&c->orders->formations[id];
    uint8_t *const bytes=(uint8_t *)(void *)f;
    const PharaohLegionOrderBorrowedState borrowed={id,
        {&f->strength_class,f->member_ids,&f->morale_leader_id,&f->movement_mode,&f->morale,
            &f->previous_movement_mode,(int16_t *)(void *)(bytes+124),
            (const int16_t *)(const void *)(bytes+126),&f->deployment_status,
            &f->months_low_morale,&f->enemy_type,&f->months_very_low_morale,
            bytes+119,(uint32_t *)(void *)(bytes+140)},
        {(uint8_t *)(void *)c->orders->walkers,sizeof(*c->orders->walkers),
            0,1,20,22,56,308,316,320},
        c->button_parameter,c->orders->current_walker_id,c->active_formation_id,
        c->orders->command_locked,c->orders->redraw_units,c->orders->redraw_city};
    pharaoh_legion_order_button_borrowed(&borrowed);
}

typedef struct DockResolution {
    PharaohFerryMapContext *shore;
    int32_t old_x,old_y;
    uint8_t used;
} DockResolution;

static void resolve_dock(void *user,int32_t x,int32_t y,uint8_t size,
    int16_t *result_x,int16_t *result_y)
{
    DockResolution *const r=user;
    r->old_y=*r->shore->result_y;r->old_x=*r->shore->result_x;r->used=1;
    (void)pharaoh_map_water_tile_next_to(r->shore,x,y,size);
    *result_x=(int16_t)*r->shore->result_x;*result_y=(int16_t)*r->shore->result_y;
}

/* Reuse the canonical complete dock selection/assignment C. This callback
 * reaches the real shore query, not a replacement result or game model. */
void pharaoh_unit_assign_dock(void *context,int32_t walker_id)
{
    const PharaohUnitCommandUiContext *const c=context;
    DockResolution resolution={c->shore,0,0,0};
    pharaoh_transport_assign_dock(c->buildings,c->orders->walkers,c->shipwright_orders,
        c->orders->slot_walkers,(int32_t)*c->orders->current_walker_id,walker_id,
        resolve_dock,&resolution);
    if(resolution.used){*c->shore->result_x=resolution.old_x;*c->shore->result_y=resolution.old_y;}
}

/* Complete42-instruction4BC670. The changed walker and freshly selected
 * walker used for slot removal deliberately remain separate native inputs. */
void pharaoh_unit_warship_return_to_dock(const PharaohUnitCommandUiContext *c,int32_t id)
{
    const int32_t old_y=*c->shore->result_y,old_x=*c->shore->result_x;
    volatile PharaohWalkerQueryRecord *const w=&c->orders->walkers[id];
    w->action_state=11;
    const int32_t building=w->building_id;
    const uint8_t size=c->buildings[building].size;
    const int32_t y=c->buildings[building].y,x=c->buildings[building].x;
    (void)pharaoh_map_water_tile_next_to(c->shore,x,y,size);
    const uint16_t previous_x=(uint16_t)w->x,target_x=(uint16_t)*c->shore->result_x;
    w->source_x=(int16_t)previous_x;
    const uint16_t previous_y=(uint16_t)w->y,target_y=(uint16_t)*c->shore->result_y;
    w->source_y=(int16_t)previous_y;
    const int32_t walking=(int32_t)*c->orders->current_walker_id;
    w->destination_x=(int16_t)target_x;w->destination_y=(int16_t)target_y;
    *c->shore->result_x=old_x;*c->shore->result_y=old_y;
    pharaoh_walker_detach_slot_link(c->orders->walkers,c->orders->slot_walkers,walking);
}

/* Complete75-instruction406C90, including its four native switch targets. */
PharaohPercentageScaleStatus pharaoh_unit_warship_button(const PharaohUnitCommandUiContext *c)
{
    if(*c->orders->command_locked!=0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    const int32_t id=(int32_t)*c->panel_warship_id;*c->selected_warship_id=(uint32_t)id;
    if(id<=0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    volatile PharaohWalkerQueryRecord *const w=&c->orders->walkers[id];
    const int32_t maximum=w->maximum_hit_points,value=w->hit_points;
    const PharaohPercentageScaleResult percentage=pharaoh_scale_value_to_percent(value,maximum);
    if(percentage.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return percentage.status;
    const uint8_t action=w->action_state;
    if((action==13 || action==15) && percentage.value<=15)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    const uint32_t mode=*c->button_parameter;
    if(mode==3 && (action==13 || action==15 || percentage.value==100))return PHARAOH_PERCENTAGE_SCALE_VALUE;
    if(w->request_event_id>=0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    switch(mode){
    case 0:w->action_state=20;*c->warship_active=1;*c->orders->redraw_units=32;*c->orders->redraw_city=1;break;
    case 1:w->action_state=23;w->transport_order_15c=22;*c->warship_active=1;*c->orders->redraw_units=32;*c->orders->redraw_city=1;break;
    case 2:w->action_state=10;w->transport_order_15c=16;*c->warship_active=0;redraw(c);break;
    case 3:*c->warship_active=0;pharaoh_unit_assign_dock((void *)c,id);*c->orders->redraw_units=32;*c->orders->redraw_city=1;break;
    default:break;
    }
    return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

/* Complete46-instruction406DE0. No selected-warship assignment occurs. */
PharaohPercentageScaleStatus pharaoh_unit_warship_return_button(const PharaohUnitCommandUiContext *c)
{
    const int32_t id=(int32_t)*c->panel_warship_id;
    if(id<=0 || *c->orders->command_locked!=0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    volatile PharaohWalkerQueryRecord *const w=&c->orders->walkers[id];
    const int32_t maximum=w->maximum_hit_points,value=w->hit_points;
    const PharaohPercentageScaleResult p=pharaoh_scale_value_to_percent(value,maximum);
    if(p.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return p.status;
    const uint8_t action=w->action_state;
    if((action==13 || action==15) && p.value<=15)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    if(*c->button_parameter==4 && pharaoh_ship_is_docked(c->orders->walkers,c->buildings,id))return PHARAOH_PERCENTAGE_SCALE_VALUE;
    if(w->request_event_id>=0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    if(*c->button_parameter==4){*c->warship_active=0;pharaoh_unit_warship_return_to_dock(c,id);}
    redraw(c);return PHARAOH_PERCENTAGE_SCALE_VALUE;
}

/* Complete36-instruction4071C0; authenticated4-way table and15-byte mapper.
 * Unknown orders still invoke the same406720 algorithm with the old mode. */
void pharaoh_unit_legion_exit(const PharaohUnitCommandUiContext *c)
{
    if(*c->orders_open!=0){*c->orders_open=0;*c->orders->redraw_city=1;return;}
    if(*c->panel_mode==4){
        if(*c->orders->command_locked!=0)return;
        const uint32_t group=*c->active_formation_id;
        switch(c->orders->formations[group].movement_mode){
        case 1:case 2:*c->button_parameter=0;break;
        case 3:case 4:*c->button_parameter=1;break;
        case 15:*c->button_parameter=2;break;
        default:break;
        }
        pharaoh_unit_legion_order_button(c);
    }
    redraw(c);
}

/* Complete Unit00084 C. Unlike the command buttons, there is no lock test. */
void pharaoh_unit_warship_target_input(const PharaohUnitCommandUiContext *c)
{
    const int32_t id=(int32_t)*c->selected_warship_id;
    if(id<=0){redraw(c);return;}
    volatile PharaohWalkerQueryRecord *const w=&c->orders->walkers[id];
    if(w->request_event_id>=0 || w->action_state==8 || *c->cursor_enabled==0){redraw(c);return;}
    const int32_t target=pharaoh_fighter_at_grid_offset(c->tile_walkers,cursor_tile(c));
    if(target!=0){
        w->action_state_before_6_7=w->action_state;w->action_state=24;
        w->transport_order_15c=16;w->target_walker_id=(int16_t)target;
        *c->orders->redraw_units=1;*c->warship_active=0;return;
    }
    const uint8_t action=w->action_state;
    switch(action){
    case 10:case 11:case 12:case 13:case 20:case 23:case 9:{
        const int32_t ty=*c->cursor_y,tx=*c->cursor_x,ay=w->y,ax=w->x;
        int32_t x,y;
        (void)pharaoh_legion_water_farthest_on_line(&c->orders->line,ax,ay,tx,ty,&x,&y);
        w->destination_x=(int16_t)x;w->destination_y=(int16_t)y;
        if(action!=9){w->saved_source_x=w->destination_x;w->saved_source_y=w->destination_y;}
        pharaoh_walker_detach_slot_link(c->orders->walkers,c->orders->slot_walkers,id);
        if(action==9){*c->orders->redraw_city=1;return;}
        const uint8_t fresh=w->action_state;*c->orders->redraw_city=1;
        if(fresh==23){w->transport_order_15c=22;return;}
        if(fresh==10){w->transport_order_15c=16;*c->orders->redraw_units=1;*c->warship_active=0;}
        return;
    }
    default:return;
    }
}

static void transport_finish(const PharaohUnitCommandUiContext *c)
{redraw(c);*c->transport_active=0;}

/* Complete Unit00085 C. All route/embark/land changes reach real order and
 * route algorithms; the only non-normal return propagates native IDIV#DE. */
PharaohPercentageScaleStatus pharaoh_unit_transport_target_input(const PharaohUnitCommandUiContext *c)
{
    if(*c->orders->command_locked!=0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
    const int32_t id=(int32_t)*c->selected_transport_id;
    if(id<=0){redraw(c);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
    volatile PharaohWalkerQueryRecord *const w=&c->orders->walkers[id];
    if(w->action_state==8 || *c->cursor_enabled==0){redraw(c);return PHARAOH_PERCENTAGE_SCALE_VALUE;}
    if(*c->transport_active!=0){
        const uint32_t cursor=*c->cursor_image;
        if(cursor==*c->embark_image)w->action_state=12;
        if(cursor==*c->land_image)w->action_state=13;
    }
    int32_t x,y;
    switch(w->action_state){
    case 9:case 11:case 20:
        (void)pharaoh_legion_water_farthest_on_line(&c->orders->line,w->x,w->y,*c->cursor_x,*c->cursor_y,&x,&y);
        w->destination_x=(int16_t)x;w->destination_y=(int16_t)y;
        pharaoh_walker_detach_slot_link(c->orders->walkers,c->orders->slot_walkers,id);
        *c->orders->redraw_city=1;return PHARAOH_PERCENTAGE_SCALE_VALUE;
    case 10:{
        (void)pharaoh_legion_water_farthest_on_line(&c->orders->line,w->x,w->y,*c->cursor_x,*c->cursor_y,&x,&y);
        w->destination_x=(int16_t)x;w->destination_y=(int16_t)y;
        const PharaohUnitDirectionResult direction=pharaoh_unit_target_direction(w->x,w->y,x,y);
        if(direction.status!=PHARAOH_PERCENTAGE_SCALE_VALUE)return direction.status;
        w->direction=direction.direction;w->transport_order_15c=9;
        pharaoh_walker_detach_slot_link(c->orders->walkers,c->orders->slot_walkers,id);
        transport_finish(c);return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    case 12:{
        (void)pharaoh_legion_water_farthest_on_line(&c->orders->line,w->x,w->y,*c->cursor_x,*c->cursor_y,&x,&y);
        const int32_t group=pharaoh_formation_legion_at_grid_offset(c->tile_walkers,cursor_tile(c));
        if(group==0){w->destination_x=(int16_t)x;w->destination_y=(int16_t)y;}
        else{
            int32_t land_x,land_y;
            (void)pharaoh_unit_land_farthest_on_line(&c->orders->line,*c->cursor_x,*c->cursor_y,x,y,&land_x,&land_y);
            if(pharaoh_formation_legion_move_to_native(c->orders,land_x,land_y,group,0)){
                w->ship_formation_id_092=(uint8_t)group;
                w->destination_x=(int16_t)x;w->destination_y=(int16_t)y;
                pharaoh_walker_detach_slot_link(c->orders->walkers,c->orders->slot_walkers,id);
            }
        }
        transport_finish(c);return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
    case 13:
        if(w->transport_state_130==0)return PHARAOH_PERCENTAGE_SCALE_VALUE;
        if((c->orders->line.terrain[cursor_tile(c)]&4u)==0){
            (void)pharaoh_legion_water_farthest_on_line(&c->orders->line,w->x,w->y,*c->cursor_x,*c->cursor_y,&x,&y);
            w->destination_x=(int16_t)x;w->destination_y=(int16_t)y;
            w->ship_landing_x=(int16_t)*c->cursor_x;w->ship_landing_y=(int16_t)*c->cursor_y;
            pharaoh_walker_detach_slot_link(c->orders->walkers,c->orders->slot_walkers,id);
        }
        transport_finish(c);return PHARAOH_PERCENTAGE_SCALE_VALUE;
    default:return PHARAOH_PERCENTAGE_SCALE_VALUE;
    }
}

/* Complete Unit00088 C: a fort hit is handled before the fighter order. */
void pharaoh_unit_legion_target_input(const PharaohUnitCommandUiContext *c)
{
    if(*c->orders->command_locked!=0)return;
    if(*c->cursor_enabled!=0){
        const uint32_t tile=cursor_tile(c),group=*c->active_formation_id;
        const PharaohNativeLegionRecord *const f=&c->orders->formations[group];
        if(f->deployment_status>=0 || f->cursed_by_seth!=0)return;
        const int32_t building=pharaoh_formation_legion_at_building(c->building_grid,c->buildings,(int32_t)tile);
        const int32_t actor=pharaoh_fighter_at_grid_offset(c->tile_walkers,tile);
        const int32_t fresh_group=(int32_t)*c->active_formation_id;
        if(building!=0 && building==fresh_group){pharaoh_formation_legion_to_standard_native(c->orders,fresh_group,14);redraw(c);return;}
        int32_t tag=-1;const uint8_t kind=c->orders->walkers[actor].type;
        if(kind!=92 && kind!=101 && kind!=93 && kind!=100)tag=c->orders->walkers[actor].formation_id_60;
        const int32_t y=*c->cursor_y,x=*c->cursor_x;
        if(actor!=0)(void)pharaoh_formation_legion_attack_near_native(c->orders,x,y,fresh_group,(uint32_t)tag);
        else (void)pharaoh_formation_legion_move_to_native(c->orders,x,y,fresh_group,0);
        (void)pharaoh_native_return_zero_0047d350();
    }
    redraw(c);
}

/* Complete Unit00089 C, preserving its separate no-selection finish path. */
void pharaoh_unit_legion_fort_input(const PharaohUnitCommandUiContext *c)
{
    if(*c->orders->command_locked!=0)return;
    if(*c->cursor_enabled!=0){
        const uint32_t tile=cursor_tile(c),group=*c->active_formation_id;
        const PharaohNativeLegionRecord *const f=&c->orders->formations[group];
        if(f->deployment_status>=0 || f->cursed_by_seth!=0)return;
        const int32_t building=pharaoh_formation_legion_at_building(c->building_grid,c->buildings,(int32_t)tile);
        const int32_t fresh_group=(int32_t)*c->active_formation_id;
        if(building!=0 && building==fresh_group){pharaoh_formation_legion_to_standard_native(c->orders,fresh_group,14);redraw(c);return;}
        const int32_t x=*c->cursor_x,y=*c->cursor_y;
        (void)pharaoh_formation_legion_return_to_fort_native(c->orders,x,y,fresh_group);
        (void)pharaoh_native_return_zero_0047d350();
    }
    redraw(c);
}
