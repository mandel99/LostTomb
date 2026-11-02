#ifndef RESTORE_EDITOR_EVENT_LIST_ACTIONS_H
#define RESTORE_EDITOR_EVENT_LIST_ACTIONS_H
#include <stdint.h>
void restore_editor_event_list_00448560(void);
void restore_editor_event_list_00448590(void);
void restore_editor_event_list_00448620(void);
void restore_event_detail_open_00448ca0(void);
/* Explicit ECX receivers. Native append/erase/due consume one stack DWORD
   with RET4; these C facades normalize that ABI to explicit arguments. */
uint32_t restore_event_list_append_00448d10(uint32_t self,uint32_t limited);
uint8_t restore_event_list_erase_00448e30(uint32_t self,int32_t index);
void restore_event_record_reset_004489c0(uint32_t self);
uint8_t restore_event_record_due_0044d680(uint32_t self,uint32_t ignore_flags);
int restore_editor_event_list_action(uint32_t token);
#endif
