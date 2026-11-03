#ifndef RESTORE_TOWN_RECORD_QUERIES_H
#define RESTORE_TOWN_RECORD_QUERIES_H
#include <stdint.h>
/* Full EAX results retain native partial AL/AX writes. Incoming EAX is an
   explicit argument, not an uninitialized C variable. Calendar base/offset
   years are signed WORDs; current year and current month are DWORDs. */
uint32_t restore_event_kind_is_1_19_20(uint32_t self,uint32_t incoming_eax);
uint32_t restore_event_kind2_range_at_least9(uint32_t self,uint32_t incoming_eax);
uint32_t restore_event_kind1_mode3(uint32_t self,uint32_t incoming_eax);
uint32_t restore_event_due_result(uint32_t self,uint32_t ignore_flags,uint32_t incoming_eax);
uint32_t restore_event_is_future_result(uint32_t self);
uint32_t restore_event_within_window_result(uint32_t self,uint32_t incoming_eax);
uint32_t restore_event_list_record_kind2(uint32_t self,int32_t index);
uint32_t restore_event_list_count_kind3(uint32_t self);
/* Original44BB40's invalid-index arm reads address2. The raw method above
   preserves that access; the existing global gameplay facade retains its
   explicit invalid-index guard. Special index140 bypasses count/type checks. */
#define RESTORE_RECORD_ENTRY0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_RECORD_EAX_ENTRY0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push eax } __asm { push ecx } __asm { call body } \
 __asm { add esp,8 } __asm { ret } }
#define RESTORE_RECORD_EAX_ENTRY1(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push eax } __asm { push dword ptr[esp+8] } __asm { push ecx } \
 __asm { call body } __asm { add esp,12 } __asm { ret } }
#endif
