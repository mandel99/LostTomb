#include "event_packet.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))

int32_t restore_resource_lookup_slot(uint32_t commodity)
{
    uint32_t index,address=0x77de04u;
    for(index=0;index<61u;++index,address+=106u)
        if(B(address-4u)&&B(address)==commodity)return (int32_t)index;
    return 0;
}
uint32_t restore_event_record_lookup(int32_t index)
{
    if(index<0||index>=(int32_t)U(0x785d70u))return 0;
    return 0x785d74u+(uint32_t)index*124u;
}
uint32_t restore_event_link_distinct_result(uint32_t self,int32_t index)
{
    uint32_t linked=restore_event_record_lookup(index),id;
    if(!linked)return 0;
    if(!(B(linked+0x28u)&1u))return linked&0xffffff00u;
    id=(uint32_t)H(self);
    return (id&0xffffff00u)|(index!=(int32_t)id);
}
int32_t restore_event_remaining_months(uint32_t self)
{
    uint32_t years=(uint32_t)H(self+0x14u)+(uint32_t)H(0x7848d8u)-U(0x78d7bcu);
    return (int32_t)(years*12u+(uint32_t)H(self+0x2cu)+B(self+3u)-U(0x78d7c0u));
}
void restore_event_message_parameters_0044e6f0(uint32_t self,int16_t parameters[16])
{
    uint32_t resource=(uint32_t)restore_resource_lookup_slot((uint32_t)H(self+0x1cu));
    uint32_t out=(uint32_t)(uintptr_t)parameters;uint16_t reason;uint8_t commodity,flag;
    W(out+6u)=(uint16_t)resource;W(out+0x12u)=(uint16_t)resource;
    W(out+0x18u)=W(self+12u);W(out+0x1au)=W(self+4u);B(out+0x1du)=B(self+0x36u);
    if(restore_event_record_lookup(H(self+0x34u))){
        reason=W(self+0x64u);if((int16_t)reason>=0)W(out+0x10u)=reason;
        commodity=B(self+0x63u);
        if(commodity)W(out+0x12u)=(uint16_t)restore_resource_lookup_slot(commodity);
        W(out+0x18u)=W(self+0x68u);W(out+0x1au)=W(self+0x66u);B(out+0x1du)=B(self+0x37u);
    }
    W(out+4u)=0xffff;W(out+8u)=W(self+12u);W(out+10u)=W(self+4u);
    W(out+12u)=W(self+0x2cu);W(out+14u)=(uint16_t)restore_event_remaining_months(self);
    W(out+20u)=0;flag=B(self+0x36u);W(out+22u)=0;B(out+28u)=flag;
}
