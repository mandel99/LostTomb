#include "pharaoh/military/native_orders.h"
#include "pharaoh/core/memory_fill.h"
#include "pharaoh/core/static_objects.h"
#include "pharaoh/core/x87_numeric.h"

_Static_assert(sizeof(PharaohNativeLegionRecord)==144,"native formation stride");
_Static_assert(offsetof(PharaohNativeLegionRecord,standard_walker_id)==64 &&
    offsetof(PharaohNativeLegionRecord,cursed_by_seth)==100 &&
    offsetof(PharaohNativeLegionRecord,months_low_morale)==102 &&
    offsetof(PharaohNativeLegionRecord,months_very_low_morale)==115 &&
    offsetof(PharaohNativeLegionRecord,reserved_07c)==124 &&
    offsetof(PharaohNativeLegionRecord,reserved_084)==132,
    "native order formation overlays");
_Static_assert(sizeof(PharaohWalkerQueryRecord)==388 &&
    offsetof(PharaohWalkerQueryRecord,reserved_000)==0 &&
    offsetof(PharaohWalkerQueryRecord,target_walker_id)==130 &&
    offsetof(PharaohWalkerQueryRecord,combat_action_counter_134)==308 &&
    offsetof(PharaohWalkerQueryRecord,movement_speed)==316 &&
    offsetof(PharaohWalkerQueryRecord,reserved_140)==320,
    "native order walker overlays");

static uint32_t route_advance(int32_t *cursor)
{
    uint32_t next=(uint32_t)*cursor+1u;
    *cursor=(int32_t)next;
    if((int32_t)next>51983){*cursor=0;next=0;}
    return next;
}

static int route_tile_allowed(const PharaohRoutingSearchState *s,uint32_t tile)
{
    const int8_t kind=s->land_citizen[tile];
    if(kind<0 && kind!=-3){
        if(kind!=-1 || (s->terrain[tile]&0x1000u)==0)return 0;
    }
    if(kind==3 || kind==1)return 0;
    return kind<=4 || kind==6;
}

/* Whole current/old Unit02300 C. The four native neighbor blocks call the
 * actual friendly-combat query before reading the signed classification BYTE.
 * The route search's generic callback is deliberately not used here. */
uint32_t pharaoh_legion_citizen_route_no_ferry(
    const PharaohNativeOrderRouteContext *c,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y)
{
    static const int32_t delta[4]={-228,1,228,-1};
    PharaohRoutingSearchState *const s=c->search;
    pharaoh_memory_fill_bytes((uint32_t *)(void *)s->distance,207936u,0);
    const uint32_t start=(uint32_t)s->grid_start+(uint32_t)y*228u+(uint32_t)x;
    s->source_offset=(int32_t)start;
    s->total_routes_calculated=(int32_t)((uint32_t)s->total_routes_calculated+1u);
    s->distance[start]=1;
    s->queue_items[0]=s->source_offset;
    s->queue_head=0;s->queue_tail=1;
    uint32_t head=0,tail=1;
    const uint32_t target=(uint32_t)s->grid_start+
        (uint32_t)target_y*228u+(uint32_t)target_x;
    do{
        const uint32_t tile=(uint32_t)s->queue_items[head];
        if(tile==target)break;
        const uint32_t distance=(uint32_t)s->distance[tile]+1u;
        for(uint32_t i=0;i<4;++i){
            const uint32_t next=tile+(uint32_t)delta[i];
            if(s->distance[next]!=0)continue;
            if(pharaoh_map_no_fighting_friendly(c->friendly,(int32_t)next)!=0 &&
                route_tile_allowed(s,next)){
                s->distance[next]=(int32_t)distance;
                s->queue_items[s->queue_tail]=(int32_t)next;
                tail=route_advance(&s->queue_tail);
            }else tail=(uint32_t)s->queue_tail;
        }
        head=route_advance(&s->queue_head);
    }while(head!=tail);
    return s->distance[target]!=0;
}

#if !defined(_MSC_VER) || !defined(_M_IX86)
#error The inherited-precision native order search requires MSVC Win32 x87.
#endif

/* Exact pinned DWORD constants56F480/484/488:0,1,FLT_MAX. These are binary32
 * operands; slope/intersection spills remain exact native TBYTE values. */
static const uint32_t los_zero_bits=0,los_one_bits=0x3f800000u;
static const uint32_t los_max_bits=0x7f7fffffu;

/* Reused complete ActorOrderLosNumeric C, retaining the native CW. */
static void los_slopes(int32_t delta_x,int32_t delta_y,
    PharaohFloat80 *slope,PharaohFloat80 *inverse)
{
    if(delta_x==0){
        __asm {
            fld dword ptr los_max_bits
            fld dword ptr los_zero_bits
        }
    }else if(delta_y==0){
        __asm {
            fld dword ptr los_zero_bits
            fld dword ptr los_max_bits
        }
    }else{
        __asm {
            fild delta_y
            fild delta_x
            fdivp st(1),st(0)
            fld dword ptr los_one_bits
            fdiv st(0),st(1)
        }
    }
    __asm {
        mov eax,inverse
        fstp tbyte ptr[eax]
        mov eax,slope
        fstp tbyte ptr[eax]
    }
}

static uint32_t los_equal_max(const PharaohFloat80 *value)
{
    uint16_t status;
    __asm {
        mov eax,value
        fld tbyte ptr[eax]
        fcom dword ptr los_max_bits
        fnstsw status
        fstp st(0)
    }
    return (status&0x4000u)!=0;
}

static int32_t los_intersection(int32_t origin,int32_t delta,
    const PharaohFloat80 *ratio)
{
    PharaohFloat80 result;
    __asm {
        mov eax,ratio
        fld tbyte ptr[eax]
        fild delta
        fmul st(0),st(1)
        fiadd origin
        fstp tbyte ptr result
        fstp st(0)
    }
    return (int32_t)(uint32_t)pharaoh_x87_truncate80(&result);
}

static int32_t wrap_add(int32_t a,int32_t b)
{return (int32_t)((uint32_t)a+(uint32_t)b);}
static int32_t wrap_sub(int32_t a,int32_t b)
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

static uint8_t terrain_low_byte(const PharaohNativeOrderLineContext *c,uint32_t tile)
{return *(const volatile uint8_t *)(const volatile void *)&c->terrain[tile];}

static int water_reachable(const PharaohNativeOrderLineContext *c,uint32_t tile)
{
    if((terrain_low_byte(c,tile)&4u)==0)return 0;
    if((terrain_low_byte(c,tile-228u)&4u)==0)return 0;
    if((terrain_low_byte(c,tile+228u)&4u)==0)return 0;
    if((terrain_low_byte(c,tile+1u)&4u)==0)return 0;
    if((terrain_low_byte(c,tile-1u)&4u)==0)return 0;
    if(c->water->routing_tiles[tile]==0xff)return 0;
    return (int32_t)c->water->distance[tile]>0;
}

/* Whole Unit00087 C, with the genuine0051CE90 water-distance algorithm and
 * shared0056133C truncate body. No endpoint callback or host double arithmetic. */
int32_t pharaoh_legion_water_farthest_on_line(
    const PharaohNativeOrderLineContext *c,
    int32_t x,int32_t y,int32_t target_x,int32_t target_y,
    int32_t *result_x,int32_t *result_y)
{
    int32_t best=0,best_x=x,best_y=y,working;
    PharaohFloat80 slope,inverse;
    if(x==target_x && y==target_y){*result_x=x;*result_y=y;return 0;}
    (void)pharaoh_water_routing_build_distance(c->water,x,y);
    const int32_t dx=wrap_sub(target_x,x),dy=wrap_sub(target_y,y);
    const int32_t step_x=sign(dx),step_y=sign(dy);
    const uint32_t nx=magnitude(dx),ny=magnitude(dy);
    los_slopes(dx,dy,&slope,&inverse);
    uint32_t row=(uint32_t)*c->water->origin+(uint32_t)y*228u;
    working=y;
    for(int32_t remaining=(int32_t)ny;remaining>0;--remaining){
        if(!los_equal_max(&inverse)){
            row+=(uint32_t)step_y*228u;working=wrap_add(working,step_y);
            const int32_t candidate=los_intersection(x,wrap_sub(working,y),&inverse);
            if(water_reachable(c,row+(uint32_t)candidate)){
                const int32_t metric=square_sum(wrap_sub(working,y),wrap_sub(candidate,x));
                if(metric>=best){best=metric;best_x=candidate;best_y=working;}
            }
        }
    }
    working=x;
    for(int32_t remaining=(int32_t)nx;remaining>0;--remaining){
        if(!los_equal_max(&slope)){
            working=wrap_add(working,step_x);
            const int32_t candidate=los_intersection(y,wrap_sub(working,x),&slope);
            const uint32_t tile=(uint32_t)*c->water->origin+
                (uint32_t)candidate*228u+(uint32_t)working;
            if(water_reachable(c,tile)){
                const int32_t metric=square_sum(wrap_sub(working,x),wrap_sub(candidate,y));
                if(metric>=best){best=metric;best_x=working;best_y=candidate;}
            }
        }
    }
    *result_x=best_x;*result_y=best_y;return best;
}

static volatile uint32_t *formation_dword(volatile PharaohNativeLegionRecord *f,uint32_t offset)
{return (volatile uint32_t *)((volatile uint8_t *)f+offset);}
static volatile int16_t *formation_word(volatile PharaohNativeLegionRecord *f,uint32_t offset)
{return (volatile int16_t *)((volatile uint8_t *)f+offset);}
static volatile int32_t *walker_target_y(volatile PharaohWalkerQueryRecord *w)
{return (volatile int32_t *)((volatile uint8_t *)w+320u);}

static int order_eligible(const PharaohNativeLegionOrderContext *c,
    volatile PharaohNativeLegionRecord *f,int32_t id,int skip_fighting)
{
    const uint8_t action=c->walkers[id].action_state;
    if(action==4 || (skip_fighting && action==5) ||
        f->months_very_low_morale!=0 || f->months_low_morale>1)return 0;
    return (uint8_t)pharaoh_walker_action_is_6_7_22_or_23(c->walkers,id)==0;
}

static void order_route_from_formation(const PharaohNativeLegionOrderContext *c,
    volatile PharaohNativeLegionRecord *f)
{
    const int32_t y=f->y,x=f->x;
    (void)pharaoh_map_routing_citizen_access(c->route.search,x,y,-2,-2);
}

/* Whole current/old Unit01467 C, preserving the native two member phases. */
uint32_t pharaoh_formation_legion_move_to_native(
    const PharaohNativeLegionOrderContext *c,
    int32_t x,int32_t y,int32_t formation_id,uint8_t quiet)
{
    volatile PharaohNativeLegionRecord *const f=&c->formations[formation_id];
    if(*c->command_locked)return 0;
    const int32_t leader=f->standard_walker_id;
    order_route_from_formation(c,f);
    int32_t from_x,from_y;
    if(*formation_dword(f,132u)!=0){from_y=f->y;from_x=f->x;}
    else{from_y=c->walkers[leader].y;from_x=c->walkers[leader].x;}
    if(!pharaoh_legion_citizen_route_no_ferry(&c->route,from_x,from_y,x,y)){
        pharaoh_city_warning_post(c->warnings,0xd3);return 0;
    }
    const uint32_t tile=(uint32_t)c->route.search->grid_start+(uint32_t)y*228u+(uint32_t)x;
    if(c->route.search->distance[tile]<=0)return 0;
    if(x==f->x && y==f->y)return 0;
    if(f->cursed_by_seth!=0)return 0;
    f->order_x=(int16_t)x;f->order_y=(int16_t)y;f->deployment_blocked=0;
    if(quiet)return 1;
    if(f->morale<=20)pharaoh_city_warning_post(c->warnings,0x33);
    if(f->strength_class==13 && c->walkers[f->member_ids[0]].action_state==19 &&
        *c->button_parameter!=1){
        for(uint32_t i=0;i<16;++i){
            const int32_t id=f->member_ids[i];if(id==0)break;
            volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
            if(!order_eligible(c,f,id,0))continue;
            if(f->months_low_morale==1)pharaoh_formation_change_morale_native(c->formations,formation_id,10);
            const uint32_t selected=*c->current_walker_id;
            w->reserved_000=0;w->combat_action_counter_134=0;f->movement_mode=15;
            w->movement_speed=w->x;*walker_target_y(w)=w->y;w->action_state=20;
            c->walkers[selected].animation_frame=0;
        }
    }
    for(uint32_t i=0;i<16;++i){
        const int32_t id=f->member_ids[i];if(id==0)break;
        volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
        if(!order_eligible(c,f,id,1))continue;
        if(f->months_low_morale==1)pharaoh_formation_change_morale_native(c->formations,formation_id,10);
        w->reserved_000=0;w->action_state=11;
        pharaoh_walker_detach_slot_link(c->walkers,c->slot_walkers,id);
    }
    return 1;
}

/* Whole Unit01468 C. The attack target is searched in native nine-tile order;
 * accepted orders retain the old formation coordinates, rather than cx/cy. */
uint32_t pharaoh_formation_legion_attack_near_native(
    const PharaohNativeLegionOrderContext *c,
    int32_t x,int32_t y,int32_t formation_id,uint32_t task)
{
    static const int32_t delta[9][2]={{-1,-1},{-1,0},{-1,1},{0,-1},{0,0},{0,1},{1,-1},{1,0},{1,1}};
    volatile PharaohNativeLegionRecord *const f=&c->formations[formation_id];
    const uint8_t blocked=*c->command_locked;
    const int32_t leader=f->standard_walker_id;
    if(blocked)return 0;
    order_route_from_formation(c,f);
    uint32_t index;int32_t cx=0,cy=0;
    for(index=0;index<9;++index){
        cy=wrap_add(y,delta[index][1]);cx=wrap_add(x,delta[index][0]);
        const int32_t ay=c->walkers[leader].y,ax=c->walkers[leader].x;
        if(pharaoh_legion_citizen_route_no_ferry(&c->route,ax,ay,cx,cy))break;
    }
    if(index==9){pharaoh_city_warning_post(c->warnings,0xd3);return 0;}
    const uint32_t tile=(uint32_t)c->route.search->grid_start+(uint32_t)cy*228u+(uint32_t)cx;
    if(c->route.search->distance[tile]<=0)return 0;
    const int16_t old_x=f->x;
    if(cx==old_x && cy==f->y)return 0;
    if(f->cursed_by_seth!=0)return 0;
    f->order_x=old_x;f->order_y=f->y;f->deployment_blocked=0;*formation_dword(f,140u)=task;
    if(f->morale<=20)pharaoh_city_warning_post(c->warnings,0x33);
    if(f->strength_class==13 && c->walkers[f->member_ids[0]].action_state==19 &&
        *c->button_parameter!=1){
        for(uint32_t i=0;i<16;++i){
            const int32_t id=f->member_ids[i];if(id==0)break;
            volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
            if(!order_eligible(c,f,id,0))continue;
            if(f->months_low_morale==1)pharaoh_formation_change_morale_native(c->formations,formation_id,10);
            w->reserved_000=0;w->combat_action_counter_134=0;
            w->movement_speed=w->x;*walker_target_y(w)=w->y;
            w->action_state=21;w->target_walker_id=0;
            c->walkers[*c->current_walker_id].animation_frame=0;
        }
    }
    for(uint32_t i=0;i<16;++i){
        const int32_t id=f->member_ids[i];if(id==0)break;
        volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
        if(!order_eligible(c,f,id,1))continue;
        if(f->months_low_morale==1)pharaoh_formation_change_morale_native(c->formations,formation_id,10);
        w->reserved_000=0;w->action_state=21;w->target_walker_id=0;
        pharaoh_walker_detach_slot_link(c->walkers,c->slot_walkers,id);
    }
    return 1;
}

/* Whole Unit01469 C. Native WORD7E=1, WORD80=0 precede the warning. */
uint32_t pharaoh_formation_legion_return_to_fort_native(
    const PharaohNativeLegionOrderContext *c,int32_t x,int32_t y,int32_t formation_id)
{
    volatile PharaohNativeLegionRecord *const f=&c->formations[formation_id];
    const uint8_t blocked=*c->command_locked;
    const int32_t leader=f->standard_walker_id;
    if(blocked)return 0;
    order_route_from_formation(c,f);
    const int32_t ay=c->walkers[leader].y,ax=c->walkers[leader].x;
    if(!pharaoh_legion_citizen_route_no_ferry(&c->route,ax,ay,x,y)){
        pharaoh_city_warning_post(c->warnings,0xd3);return 0;
    }
    const uint32_t tile=(uint32_t)c->route.search->grid_start+(uint32_t)y*228u+(uint32_t)x;
    if(c->route.search->distance[tile]<=0)return 0;
    if(x==f->x && y==f->y)return 0;
    if(f->cursed_by_seth!=0)return 0;
    f->order_x=(int16_t)x;f->order_y=(int16_t)y;
    *formation_word(f,126u)=1;*formation_word(f,128u)=0;f->deployment_blocked=0;
    if(f->morale<=20)pharaoh_city_warning_post(c->warnings,0x33);
    f->movement_mode=0;
    for(uint32_t i=0;i<16;++i){
        const int32_t id=f->member_ids[i];if(id==0)break;
        volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
        if(!order_eligible(c,f,id,1))continue;
        if(f->months_low_morale==1)pharaoh_formation_change_morale_native(c->formations,formation_id,10);
        w->reserved_000=0;w->combat_action_counter_134=0;
        w->movement_speed=w->x;*walker_target_y(w)=w->y;w->action_state=19;
        pharaoh_walker_detach_slot_link(c->walkers,c->slot_walkers,id);
    }
    return 1;
}

/* Whole Unit01470 C. The cached home tile survives the real route query. */
void pharaoh_formation_legion_to_standard_native(
    const PharaohNativeLegionOrderContext *c,int32_t formation_id,int32_t command)
{
    volatile PharaohNativeLegionRecord *const f=&c->formations[formation_id];
    const int32_t leader=f->standard_walker_id;
    int32_t id;
    if(command!=0 && (id=(int32_t)*formation_dword(f,132u))!=0){
        volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
        w->action_state=(uint8_t)command;
        int32_t x,y,out_x,out_y;
        if(command==-1){x=f->x;y=f->y;}else{x=f->home_x;y=f->home_y;}
        const int32_t ay=w->y,ax=w->x;
        (void)pharaoh_legion_water_farthest_on_line(&c->line,ax,ay,x,y,&out_x,&out_y);
        w->destination_x=(int16_t)out_x;w->destination_y=(int16_t)out_y;
        w->ship_landing_x=(int16_t)x;w->ship_landing_y=(int16_t)y;
        pharaoh_walker_detach_slot_link(c->walkers,c->slot_walkers,id);return;
    }
    order_route_from_formation(c,f);
    const int32_t y=f->home_y,x=f->home_x;
    const uint32_t tile=(uint32_t)c->route.search->grid_start+(uint32_t)y*228u+(uint32_t)x;
    const int32_t ay=c->walkers[leader].y,ax=c->walkers[leader].x;
    if(!pharaoh_legion_citizen_route_no_ferry(&c->route,ax,ay,x,y)){
        pharaoh_city_warning_post(c->warnings,0xd3);return;
    }
    if(c->route.search->distance[tile]<=0 || f->cursed_by_seth!=0)return;
    f->deployment_blocked=1;
    for(uint32_t i=0;i<16;++i){
        id=f->member_ids[i];if(id==0)break;
        if(!order_eligible(c,f,id,1))continue;
        if(f->months_low_morale==1)pharaoh_formation_change_morale_native(c->formations,formation_id,10);
        c->walkers[id].action_state=9;
        pharaoh_walker_detach_slot_link(c->walkers,c->slot_walkers,id);
    }
}

/* Whole85-instruction406FB0 C. This parent has no refuses-orders query.
 * ECX's selected walker is cached across members and reloaded after morale. */
void pharaoh_legion_return_button_native(const PharaohNativeLegionOrderContext *c)
{
    const uint32_t formation_id=*c->selected_formation_id;
    volatile PharaohNativeLegionRecord *const f=&c->formations[formation_id];
    if(f->deployment_status>=0 || f->deployment_blocked==1 || *c->command_locked!=0)return;
    if(f->strength_class==13 && c->walkers[f->member_ids[0]].action_state==19 &&
        *c->button_parameter!=1){
        uint32_t selected=*c->current_walker_id;
        for(uint32_t i=0;i<16;++i){
            const int32_t id=f->member_ids[i];if(id==0)break;
            volatile PharaohWalkerQueryRecord *const w=&c->walkers[id];
            if(w->action_state==4 || f->months_very_low_morale!=0)continue;
            const uint8_t mode=f->months_low_morale;if(mode>1)continue;
            if(mode==1){
                pharaoh_formation_change_morale_native(c->formations,(int32_t)formation_id,10);
                selected=*c->current_walker_id;
            }
            w->reserved_000=0;w->combat_action_counter_134=0;
            w->movement_speed=w->x;*walker_target_y(w)=w->y;w->action_state=12;
            c->walkers[selected].animation_frame=0;
        }
    }
    pharaoh_formation_legion_to_standard_native(c,(int32_t)formation_id,14);
    (void)pharaoh_native_return_zero_0047d350();
    *c->redraw_units=1;*c->redraw_city=1;
}
