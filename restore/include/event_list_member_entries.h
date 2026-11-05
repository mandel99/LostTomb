#ifndef RESTORE_EVENT_LIST_MEMBER_ENTRIES_H
#define RESTORE_EVENT_LIST_MEMBER_ENTRIES_H
#include <stdint.h>
/* Only the native receiver/stack boundary is new; complete C bodies already
   exist in editor_event_list_actions.c. Logical results keep their widths. */
#define RESTORE_EVENT_LIST_MEMBER0(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push ecx } __asm { call body } __asm { add esp,4 } __asm { ret } }
#define RESTORE_EVENT_LIST_MEMBER1(name,body) \
 __declspec(naked) void name(void) { \
 __asm { push dword ptr[esp+4] } __asm { push ecx } \
 __asm { call body } __asm { add esp,8 } __asm { ret 4 } }
#endif
