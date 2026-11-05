#ifndef RESTORE_ACTOR_WIDGET_MEMBERS_H
#define RESTORE_ACTOR_WIDGET_MEMBERS_H
#include <stdint.h>
/* Only real member context is adapted. Arguments keep their native DWORD
   stack slots; typed bodies below state the logical result widths. */
void restore_route_release_samples_004454a0(uint32_t self);
uint32_t restore_event_active_trade_00449390(uint32_t self);
uint32_t restore_event_trade_progress_00449750(uint32_t self,int32_t *route,
    int32_t *percentage,uint8_t *special,uint32_t *unused);
int32_t restore_event_route_months_0044c110(uint32_t self);
int32_t restore_event_warning_months_0044c9a0(uint32_t self);
#define RESTORE_WIDGET_MEMBER0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_WIDGET_MEMBER3(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ebp } __asm { mov ebp,esp } \
 __asm { push dword ptr[ebp+16] } __asm { push dword ptr[ebp+12] } \
 __asm { push dword ptr[ebp+8] } __asm { push ecx } __asm { call body } \
 __asm { add esp,16 } __asm { pop ebp } __asm { ret 12 } }
#define RESTORE_WIDGET_MEMBER4(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ebp } __asm { mov ebp,esp } \
 __asm { push dword ptr[ebp+20] } __asm { push dword ptr[ebp+16] } \
 __asm { push dword ptr[ebp+12] } __asm { push dword ptr[ebp+8] } \
 __asm { push ecx } __asm { call body } __asm { add esp,20 } \
 __asm { pop ebp } __asm { ret 16 } }
#endif
