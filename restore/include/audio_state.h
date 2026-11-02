#ifndef RESTORE_AUDIO_STATE_H
#define RESTORE_AUDIO_STATE_H
#include <stdint.h>
/* Native 0041fca0 receives the record in ECX and a 16-bit state on the stack. */
void FUN_0041fca0(void *record,uint16_t state);
/* Native ECX receiver and one actor DWORD, RET4; normalized C is cdecl. */
void FUN_0041fd80(void *record,int32_t actor);
#endif
