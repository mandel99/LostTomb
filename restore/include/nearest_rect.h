#ifndef RESTORE_NEAREST_RECT_H
#define RESTORE_NEAREST_RECT_H
#include <stdint.h>
#ifndef RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
#define RESTORE_RESTORE_MENU_INPUT_FRAME_DEFINED
struct restore_menu_input_frame;
typedef void (*restore_menu_input_frame_call)(uint32_t,uint32_t,struct restore_menu_input_frame *);
struct restore_menu_input_frame { uint32_t r[8],flags; restore_menu_input_frame_call boundary; };
#endif
void restore_C_00407870(struct restore_menu_input_frame *frame);
void restore_C_00407a10(struct restore_menu_input_frame *frame);
int32_t FUN_00407870(void);
void FUN_00407a10(int32_t *out_record,int32_t *out_rectangle);
#endif
