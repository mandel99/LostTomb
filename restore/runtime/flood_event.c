#include "event_packet.h"
#include "flood_settings.h"
#include "town_record_queries.h"
#include "menu_event_actions.h"
#include "event_publisher.h"
#include "town_menu.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
void restore_flood_event_apply(uint32_t self,uint32_t increase)
{restore_flood_event_apply_context(self,increase,0);}
void restore_flood_event_apply_context(uint32_t self,uint32_t increase,struct restore_event_actions_frame *context)
{
    int16_t packet[16];int32_t message,reason;
    if(U(0x605a3cu)==1u||!(uint8_t)restore_event_due_result(self,0,0))return;
    U(self+0x28u)|=4u;
    if(!increase){
        restore_flood_quality_decrease(0xd05720u);packet[1]=0xf7;packet[0]=0xf8;
        packet[8]=(int16_t)restore_event_message_kind_0044f660(self);message=0x158;
    }else{
        restore_flood_quality_increase(0xd05720u);packet[1]=0xfb;packet[0]=0xfc;
        packet[8]=(int16_t)restore_event_message_kind_0044f660(self);message=0x15d;
    }
    /* First two WORDs and WORD8 come from this caller; the builder fills
       every other byte copied by the publisher. The spare final WORD is unused. */
    restore_event_message_parameters_0044e6f0(self,packet);
    U(0xe950dcu)=1;FUN_004e0cf0(message,0,0,packet,0);
    if((uint8_t)restore_event_link_distinct_result(self,H(self+0x24u))){
        reason=restore_event_followup_message_context(self,0,context);
        restore_event_schedule_context(self,H(self+0x24u),(int16_t)reason,context);
    }
}
