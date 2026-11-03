#ifndef RESTORE_EVENT_PACKET_H
#define RESTORE_EVENT_PACKET_H
#include <stdint.h>
int32_t restore_resource_lookup_slot(uint32_t commodity);
uint32_t restore_event_record_lookup(int32_t index);
uint32_t restore_event_link_distinct_result(uint32_t self,int32_t index);
int32_t restore_event_remaining_months(uint32_t self);
int32_t restore_event_message_kind_0044f660(uint32_t self);
void restore_event_message_parameters_0044e6f0(uint32_t self,int16_t parameters[16]);
void restore_flood_event_apply(uint32_t self,uint32_t increase);
int32_t restore_event_followup_message(uint32_t self,uint32_t slot);
struct restore_event_actions_frame;
void restore_flood_event_apply_context(uint32_t self,uint32_t increase,struct restore_event_actions_frame *context);
int32_t restore_event_followup_message_context(uint32_t self,uint32_t slot,struct restore_event_actions_frame *context);
void restore_event_schedule_context(uint32_t self,int32_t template_id,int16_t reason,struct restore_event_actions_frame *context);
#define RESTORE_EVENT_MEMBER0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_EVENT_MEMBER1(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+4] } __asm { push ecx } \
 __asm { call body } __asm { add esp,8 } __asm { ret } }
#endif
