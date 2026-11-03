#include "town_record_queries.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define S(a) ((uint32_t)(int32_t)(int16_t)W(a))

uint32_t restore_event_kind_is_1_19_20(uint32_t self,uint32_t incoming_eax)
{
    uint32_t kind=B(self+2u);
    return (incoming_eax&0xffffff00u)|(kind==1u||kind==19u||kind==20u);
}
uint32_t restore_event_kind2_range_at_least9(uint32_t self,uint32_t incoming_eax)
{
    uint32_t value=incoming_eax;
    if(B(self+2u)==2u){
        value=(value&0xffff0000u)|W(self+0x1eu);
        if((int16_t)value<0)value=(value&0xffff0000u)|W(self+0x22u);
        else if((int16_t)W(self+0x20u)>=0)return value&0xffffff00u;
        if((int16_t)value>=9)return (value&0xffffff00u)|1u;
    }
    return value&0xffffff00u;
}
uint32_t restore_event_kind1_mode3(uint32_t self,uint32_t incoming_eax)
{
    uint32_t kind=B(self+2u);
    return (incoming_eax&0xffffff00u)|(kind==1u&&B(self+0x60u)==3u);
}
uint32_t restore_event_due_result(uint32_t self,uint32_t ignore_flags,uint32_t incoming_eax)
{
    uint32_t date,current;
    if(!(uint8_t)ignore_flags&&(B(self+0x28u)&5u))return incoming_eax&0xffffff00u;
    date=S(self+0x14u)+S(0x7848d8u);current=U(0x78d7bcu);
    if(current!=date)return date&0xffffff00u;
    date=B(self+3u);current=U(0x78d7c0u);
    return current==date;
}
uint32_t restore_event_is_future_result(uint32_t self)
{
    uint32_t date=S(self+0x14u)+S(0x7848d8u),current=U(0x78d7bcu);
    if((int32_t)current<(int32_t)date)return (date&0xffffff00u)|1u;
    if((int32_t)current>(int32_t)date)return date&0xffffff00u;
    date=B(self+3u);current=U(0x78d7c0u);
    return (int32_t)current<(int32_t)date;
}
uint32_t restore_event_within_window_result(uint32_t self,uint32_t incoming_eax)
{
    uint32_t result,elapsed,offset,base,month,current_month;int32_t duration;
    if(B(self+0x28u)&0x11u)return incoming_eax&0xffffff00u;
    result=restore_event_is_future_result(self);
    if((uint8_t)result)return result&0xffffff00u;
    offset=S(self+0x14u);elapsed=U(0x78d7bcu);base=S(0x7848d8u);
    month=B(self+3u);current_month=U(0x78d7c0u);
    elapsed=(elapsed-offset-base)*12u-month+current_month;
    duration=(int16_t)W(self+0x2cu);
    return (elapsed&0xffffff00u)|((int32_t)elapsed<=duration);
}
uint32_t restore_event_list_record_kind2(uint32_t self,int32_t index)
{
    uint32_t result=0,kind;
    if(index==140)return self+0x43d4u;
    if(index>=0&&index<(int32_t)U(self))result=self+4u+(uint32_t)index*124u;
    kind=B(result+2u);
    return kind==2u?result:0;
}
uint32_t restore_event_list_count_kind3(uint32_t self)
{
    int32_t remaining=(int32_t)U(self);uint32_t cursor=self+6u,result=0;
    if(remaining>0)do{if(B(cursor)==3u)++result;cursor+=124u;}while(--remaining);
    return result;
}
