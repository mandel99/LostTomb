#ifndef RESTORE_SMALL_BUTTON_INPUT_H
#define RESTORE_SMALL_BUTTON_INPUT_H
#include <stdint.h>
/* Native cdecl, RET0. Records are 36 bytes; count is a signed DWORD. */
int32_t FUN_00425990(int32_t x,int32_t y,const void *buttons,int32_t count);
void FUN_00425b50(uint32_t buttons,int32_t count);
void FUN_00425b80(uint32_t buttons,int32_t count);
#endif
