#ifndef RESTORE_REMAINING_EDITOR_LIST_ACTIONS_H
#define RESTORE_REMAINING_EDITOR_LIST_ACTIONS_H
#include <stdint.h>
struct restore_remaining_editor_frame;
typedef void (*restore_remaining_editor_call)(uint32_t,uint32_t,struct restore_remaining_editor_frame *);
struct restore_remaining_editor_frame {uint32_t r[8],flags;restore_remaining_editor_call boundary;};
void restore_remaining_editor_C_0040c8a0(struct restore_remaining_editor_frame *);
void restore_remaining_editor_C_0040c920(struct restore_remaining_editor_frame *);
void restore_remaining_editor_C_0040cbe0(struct restore_remaining_editor_frame *);
void restore_remaining_editor_C_0040cc40(struct restore_remaining_editor_frame *);
void restore_remaining_editor_C_0040cc80(struct restore_remaining_editor_frame *);
void restore_remaining_editor_0040c8a0(void);
void restore_remaining_editor_0040c920(void);
void restore_remaining_editor_0040cbe0(void);
void restore_remaining_editor_0040cc80(void);
int restore_remaining_editor_list_action(uint32_t token);
uint32_t restore_remaining_editor_count_0040cc40(uint32_t phase);
#endif
