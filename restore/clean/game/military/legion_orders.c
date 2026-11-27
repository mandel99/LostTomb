#include <stddef.h>
#include "pharaoh/core/static_objects.h"
#include "pharaoh/military/legion_orders.h"

/* Shared original morale algorithm; retain intermediate WORD publication. */
static void formation_morale_apply(int16_t kind,int16_t leader,uint8_t subtype,
    volatile int16_t *value,int16_t delta)
{
    int32_t maximum;
    if(kind==13)maximum=leader!=0?100:80;
    else if(kind==57)maximum=100;
    else if(kind==12||kind==11)maximum=leader!=0?80:60;
    else maximum=subtype==13?80:70;
    *value=(int16_t)((uint16_t)*value+(uint16_t)delta);
    if(*value>maximum)*value=(int16_t)maximum;
    if(*value<0)*value=0;
}

void pharaoh_formation_change_morale(PharaohLegionOrderState *state,
    int32_t formation_id,int16_t delta)
{
    if(formation_id==0)return;
    PharaohLegionOrderFormation *f=&state->formations[(uint32_t)formation_id];
    formation_morale_apply(f->kind,f->leader_id,f->subtype,&f->morale,delta);
}

void pharaoh_formation_change_morale_native(PharaohNativeLegionRecord *formations,
    int32_t formation_id,int16_t delta)
{
    if(formation_id==0)return;
    PharaohNativeLegionRecord *f=&formations[formation_id];
    formation_morale_apply(f->strength_class,f->morale_leader_id,f->enemy_type,&f->morale,delta);
}

static volatile uint8_t *order_walker_field(
    const PharaohLegionOrderWalkerFields *v,uint32_t id,size_t offset)
{
    return (volatile uint8_t *)(uintptr_t)((uintptr_t)v->base+
        (uint32_t)(id*(uint32_t)v->stride)+(uint32_t)offset);
}
void pharaoh_legion_order_button_borrowed(const PharaohLegionOrderBorrowedState *s)
{
    const PharaohLegionOrderFormationFields *const f=&s->formation;
    const PharaohLegionOrderWalkerFields *const w=&s->walkers;
    if(*f->availability>=0 || *s->command_locked!=0)return;
    uint32_t mode=*s->button_parameter;
    int16_t order;
    if(mode==3){order=*f->order;if(order!=6)*f->previous_order=order;}
    if(*f->kind==13 &&
        *order_walker_field(w,(uint32_t)(int32_t)f->walkers[0],w->action_state)==19 && mode!=1){
        uint32_t selected=*s->current_walker_id;
        for(uint32_t i=0;i<PHARAOH_LEGION_WALKER_COUNT;++i){
            const int32_t id=f->walkers[i];if(id==0)break;
            const uint32_t index=(uint32_t)id;
            if(*order_walker_field(w,index,w->action_state)==4 ||
                *f->disabled!=0 || *f->order_mode>1)continue;
            if(*f->order_mode==1){
                if(s->formation_id!=0)
                    formation_morale_apply(*f->kind,*f->leader_id,*f->subtype,f->morale,10);
                selected=*s->current_walker_id;
            }
            *order_walker_field(w,index,w->state)=0;
            *(volatile uint32_t *)order_walker_field(w,index,w->route)=0;
            *(volatile int32_t *)order_walker_field(w,index,w->target_x)=
                *(volatile int16_t *)order_walker_field(w,index,w->x);
            *(volatile int32_t *)order_walker_field(w,index,w->target_y)=
                *(volatile int16_t *)order_walker_field(w,index,w->y);
            *order_walker_field(w,index,w->action_state)=12;
            *order_walker_field(w,selected,w->animation_frame)=0;
            mode=*s->button_parameter;
        }
    }
    switch(mode){
    case 0:*f->order=*f->alternate_order!=0?2:1;(void)pharaoh_native_return_zero_0047d350();break;
    case 1:
        if(*f->kind==13){
            if(*f->field_126==0){order=*f->order;if(order!=0)*f->resume_order=order;*f->order=0;}
        }else *f->order=*f->alternate_order!=0?4:3;
        (void)pharaoh_native_return_zero_0047d350();break;
    case 2:*f->order=15;(void)pharaoh_native_return_zero_0047d350();break;
    case 3:*f->order=6;(void)pharaoh_native_return_zero_0047d350();break;
    default:break;
    }
    *s->active_formation_id=s->formation_id;order=*f->order;
    if(order==6){*f->task=UINT32_MAX;*s->sidebar_update_code=1;*s->redraw_requested=1;return;}
    if(order==0){*f->task=UINT32_MAX;*s->sidebar_update_code=0x49;*s->redraw_requested=1;return;}
    if(*f->field_126!=0){*s->sidebar_update_code=9;if(*s->button_parameter!=1)*s->sidebar_update_code=0x1e;}
    else *s->sidebar_update_code=0x1e;
    *s->redraw_requested=1;*f->task=UINT32_MAX;
}
void pharaoh_legion_order_button(PharaohLegionOrderState *state)
{
    PharaohLegionOrderFormation *const f=&state->formations[state->selected_formation_id];
    const PharaohLegionOrderBorrowedState borrowed={
        state->selected_formation_id,
        {&f->kind,f->walkers,&f->leader_id,&f->order,&f->morale,&f->previous_order,
            &f->resume_order,&f->field_126,&f->availability,&f->order_mode,&f->subtype,
            &f->disabled,&f->alternate_order,&f->task},
        {(uint8_t *)(void *)state->walkers,sizeof(*state->walkers),
            offsetof(PharaohLegionOrderWalker,state),offsetof(PharaohLegionOrderWalker,animation_frame),
            offsetof(PharaohLegionOrderWalker,x),offsetof(PharaohLegionOrderWalker,y),
            offsetof(PharaohLegionOrderWalker,action_state),offsetof(PharaohLegionOrderWalker,route),
            offsetof(PharaohLegionOrderWalker,target_x),offsetof(PharaohLegionOrderWalker,target_y)},
        &state->button_parameter,&state->current_walker_id,&state->active_formation_id,
        &state->command_locked,&state->sidebar_update_code,&state->redraw_requested};
    pharaoh_legion_order_button_borrowed(&borrowed);
}
