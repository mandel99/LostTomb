#include "actor_message.h"
#include "message_category.h"
#include "town_record_queries.h"
#include "building_processing_query.h"
#include "building_occupancy_query.h"
#include "storage_trade.h"
#include "storage_delivery.h"
/* Whole integer actor-message controller and genuine dependency bodies.
 * Pinned PE SHA256 fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d.
 * Code-address tokens below are readonly table data or dispatcher keys, never native code pointers. */
#include "menu_actor_message.h"
#include "town_menu.h"
#include "actor_predicate.h"
#include "training_queries.h"
#include "menu_list_scroll.h"
#include "menu_building_kind.h"
#include <stdio.h>
#include <stdlib.h>
#define MI_U8(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define MI_U16(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MI_U32(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
void restore_actor_message_C_004434e0(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_trade_can_export((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_actor_message_C_004435f0(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_trade_can_import((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_actor_message_C_0044d6d0(struct restore_actor_message_frame *f)
{
    f->r[0]=restore_event_is_future_result(f->r[1]);f->r[4]+=4;
}

void restore_actor_message_C_0044d700(struct restore_actor_message_frame *f)
{
    f->r[0]=restore_event_within_window_result(f->r[1],f->r[0]);f->r[4]+=4;
}

void restore_actor_message_C_00459240(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_building_resource_is_permitted((int32_t)MI_U32(sp+4u));f->r[4]=sp+4u;
}

void restore_actor_message_C_0045b230(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_storage_limit_exceeded((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u));f->r[4]=sp+4u;
}

void restore_actor_message_C_0045cf50(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_can_export((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_actor_message_C_0045d180(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_can_import((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));f->r[4]=sp+4u;
}

void restore_actor_message_C_0045d510(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=(uint32_t)restore_storage_resource_amount((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u));f->r[4]=sp+4u;
}

void restore_actor_message_C_004893c0(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_region_has_blocking_tile((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u),MI_U32(sp+20u));f->r[4]=sp+4u;
}

void restore_actor_message_C_00489470(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_region_has_active_occupant((int32_t)MI_U32(sp+4u),(int32_t)MI_U32(sp+8u),(int32_t)MI_U32(sp+12u),(int32_t)MI_U32(sp+16u));f->r[4]=sp+20u;
}

void restore_actor_message_C_004894a0(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_building_has_active_occupant((int32_t)MI_U32(sp+4u));f->r[4]=sp+8u;
}

void restore_actor_message_C_00495b60(struct restore_actor_message_frame *f)
{
    f->r[0]=restore_actor_building_work_state(MI_U32(f->r[4]+4));f->r[4]+=4;
}

void restore_actor_message_C_004bf9d0(struct restore_actor_message_frame *f)
{
 uint32_t sp=f->r[4];f->r[0]=restore_city_has_unpermitted_occupied_building(f->r[1]);f->r[4]=sp+4u;
}

void restore_actor_message_C_0053c060(struct restore_actor_message_frame *f)
{
    f->r[0]=restore_message_choose_weighted_category();f->r[4]+=4;
}

void restore_actor_message_C_0053c100(struct restore_actor_message_frame *f)
{
    uint32_t sp=f->r[4];f->r[0]=restore_actor_update_message_result((int32_t)MI_U32(sp+4),f->r[0]);f->r[4]=sp+4;
}

void restore_actor_message_C_0053d3a0(struct restore_actor_message_frame *f)
{
    uint32_t sp=f->r[4];
    f->r[0]=restore_message_build_category_weights((volatile uint8_t *)(uintptr_t)MI_U32(sp+4),(volatile uint8_t *)(uintptr_t)MI_U32(sp+8));f->r[4]=sp+4;
}

__declspec(noinline) __declspec(noreturn) void restore_actor_message_failure(uint32_t target)
{fprintf(stderr,"Unsupported actor-message boundary %08lx\n",(unsigned long)target);abort();}
void restore_actor_message_dispatch(uint32_t site,uint32_t target,struct restore_actor_message_frame *frame)
{
 uint32_t sp=frame->r[4];(void)site;
 switch(target){

 case 0x004434e0u:restore_actor_message_C_004434e0(frame);return;
 case 0x004435f0u:restore_actor_message_C_004435f0(frame);return;
 case 0x0044d6d0u:restore_actor_message_C_0044d6d0(frame);return;
 case 0x0044d700u:restore_actor_message_C_0044d700(frame);return;
 case 0x00459240u:restore_actor_message_C_00459240(frame);return;
 case 0x0045b230u:restore_actor_message_C_0045b230(frame);return;
 case 0x0045cf50u:restore_actor_message_C_0045cf50(frame);return;
 case 0x0045d180u:restore_actor_message_C_0045d180(frame);return;
 case 0x0045d510u:restore_actor_message_C_0045d510(frame);return;
 case 0x004893c0u:restore_actor_message_C_004893c0(frame);return;
 case 0x00489470u:restore_actor_message_C_00489470(frame);return;
 case 0x004894a0u:restore_actor_message_C_004894a0(frame);return;
 case 0x00495b60u:restore_actor_message_C_00495b60(frame);return;
 case 0x004bf9d0u:restore_actor_message_C_004bf9d0(frame);return;
 case 0x0053c060u:restore_actor_message_C_0053c060(frame);return;
 case 0x0053c100u:restore_actor_message_C_0053c100(frame);return;
 case 0x0053d3a0u:restore_actor_message_C_0053d3a0(frame);return;
 /* Native iterator ECX is explicit;RET8. Only EAX is consumed by these callers. */
 case 0x44bb90u:frame->r[0]=(uint32_t)(uintptr_t)restore_town_next((RestoreTownList *)(uintptr_t)frame->r[1],(const int16_t *)(uintptr_t)MI_U32(sp+4),(uint8_t)MI_U32(sp+8));frame->r[4]=sp+12;return;
 /* These existing typed providers have RET0 and only EAX consumed here. */
 case 0x4c5120u:frame->r[0]=FUN_004c5120((int32_t)MI_U32(sp+4));frame->r[4]=sp+4;return;
 case 0x4c5240u:frame->r[0]=(uint32_t)FUN_004c5240((int32_t)MI_U32(sp+4));frame->r[4]=sp+4;return;
 case 0x4c5190u:frame->r[0]=FUN_004c5190();frame->r[4]=sp+4;return;
 case 0x4cde60u:frame->r[0]=(uint32_t)FUN_004cde60((int32_t)MI_U32(sp+4),(int32_t)MI_U32(sp+8));frame->r[4]=sp+4;return;
 /* AL-only native contract. Each caller overwrites EAX before consuming its high bits. */
 case 0x4c52d0u:frame->r[0]=(frame->r[0]&0xffffff00u)|FUN_004c52d0((int32_t)MI_U32(sp+4),MI_U32(sp+8));frame->r[4]=sp+4;return;
 /* Native RET4; receiver ECX is ignored by the genuine WORD wrapper. */
 case 0x488030u:frame->r[0]=FUN_00488030(MI_U32(sp+4));frame->r[4]=sp+8;return;
 default:restore_actor_message_failure(target);
 }
}
uint32_t FUN_00495b60(uint32_t actor){return restore_actor_building_work_state(actor);}
uint32_t FUN_0053c060(void){return restore_message_choose_weighted_category();}
