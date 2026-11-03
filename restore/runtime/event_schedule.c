#include "event_schedule.h"
#include "event_packet.h"
#include "event_parameters.h"
#include "editor_event_list_actions.h"
#include "town_menu.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

int32_t restore_event_parameter_count(uint32_t self)
{
    int32_t first=H(self+2u),second,third;
    if(first>=0){if(H(self+4u)<0)return 1;return H(self+6u)>=0?3:2;}
    second=H(self+4u);if(second<0)return 0;
    third=H(self+6u);return third<0?0:third-second+1;
}
uint32_t restore_event_dispatch_deferred(uint32_t self,int32_t index,uint32_t incoming_eax)
{
    int32_t current=(int32_t)U(0x5d9a40u);uint32_t record;uint8_t kind,phase;
    if(current<0)return incoming_eax&0xffffff00u;
    if(current>index)return (uint32_t)index&0xffffff00u;
    if(index<0||index>=(int32_t)U(self))return (uint32_t)index&0xffffff00u;
    record=self+4u+(uint32_t)index*124u;if(!record)return 0;
    kind=B(record+2u);phase=B(0x78a634u);
    return (record&0xffffff00u)|(kind==2u?(phase!=0):(phase==0));
}
void restore_event_rearm_completed(uint32_t self)
{
    uint32_t index=0,flags,record=self+4u,delta;uint8_t month;
    if((int32_t)U(self)<=0)return;
    do{
        flags=U(record+0x28u);
        if((flags&4u)&&!B(record+0x32u)){
            if(flags&2u){
                U(record+0x28u)=flags&0xfffffffbu;
                (void)restore_event_record_reset(record);
                delta=U(0x78d7bcu)-U(0x7848d8u);
                W(record+0x14u)=(uint16_t)(W(record+0x14u)+delta);
            }else if(flags&16u){
                month=(uint8_t)(B(record+3u)-1u);B(record+3u)=month;
                if(!month){
                    U(record+0x28u)&=0xfffffffbu;
                    (void)restore_event_record_reset(record);
                }
            }
        }
        ++index;record+=124u;
    }while((int32_t)index<(int32_t)U(self));
}
static void inherit_parameter(uint32_t group,uint32_t parent_field)
{
    int32_t lower,upper,value;
    if(H(group+2u)<0){lower=H(group+4u);upper=H(group+6u);}
    else if(restore_event_parameter_count(group)==1){lower=H(group+2u);upper=lower;}
    else{lower=-1;upper=-1;}
    value=H(parent_field);
    if(value>=lower&&value<=upper)W(group)=(uint16_t)value;
}
void restore_event_schedule_apply(uint32_t self,int32_t template_id,int16_t reason,struct restore_event_actions_frame *context)
{
    uint32_t source=restore_event_record_lookup(template_id),record,i;uint16_t identity,value;
    uint8_t classification,month;int32_t delay,years;
    if(!source)return;
    record=restore_event_list_append_00448d10(0x785d70u,0);if(!record)return;
    identity=W(record);
    /* Native REP MOVSD is forward even when records overlap. */
    for(i=0;i<31u;++i)U(record+i*4u)=U(source+i*4u);
    W(record)=identity;(void)restore_event_record_reset(record);U(record+0x28u)=8;
    classification=(uint8_t)restore_event_graph_query_context(0x449390u,record,0,context);
    if(classification==(uint8_t)restore_event_graph_query_context(0x449390u,self,0,context))
        inherit_parameter(record+0x1cu,self+0x1cu);
    if(FUN_004493c0((const RestoreTownRecord *)(uintptr_t)record)&&
       FUN_004493c0((const RestoreTownRecord *)(uintptr_t)self)&&
       (uint8_t)restore_event_graph_query_context(0x449550u,record,H(self+4u),context))
        inherit_parameter(record+4u,self+4u);
    W(record+0x64u)=(uint16_t)reason;W(record+0x34u)=W(self);
    if((uint8_t)restore_event_graph_query_context(0x449390u,self,0,context))B(record+0x63u)=B(self+0x1cu);
    else B(record+0x63u)=0;
    value=W(self+12u);delay=H(record+0x14u);W(record+0x68u)=value;
    W(record+0x66u)=W(self+4u);B(record+0x37u)=B(self+0x36u);
    month=(uint8_t)((int32_t)(U(0x78d7c0u)+(uint32_t)delay)%12);
    years=delay/12;B(record+3u)=month;
    if((int32_t)(uint32_t)month<(int32_t)U(0x78d7c0u))++years;
    W(record+0x14u)=(uint16_t)(U(0x78d7bcu)-U(0x7848d8u)+(uint32_t)years);
    if(!delay&&!(uint8_t)restore_event_dispatch_deferred(0x785d70u,H(record),0))
        (void)restore_event_graph_query_context(B(record+2u)==2u?0x451540u:0x44c660u,record,0,context);
}
