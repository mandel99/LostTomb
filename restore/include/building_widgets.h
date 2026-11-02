#ifndef RESTORE_BUILDING_WIDGETS_H
#define RESTORE_BUILDING_WIDGETS_H
#include <stdint.h>
/* Native41FCA0: thiscall one-WORD-in-DWORD argument, RET4. Explicit self;
   preserve full incidental native EAX for consumers that require it. */
uint32_t restore_widget_initialize_0041fca0(void *self,uint16_t style);
/* Preserve the already-canonical explicit-self void API for existing callers. */
void FUN_0041fca0(void *self,uint16_t style);
void FUN_0041ff90(void);
void FUN_0041ffc0(void);
void FUN_004206a0(void);
void FUN_004209e0(void);
void FUN_00420d10(void);
void FUN_00421010(void);
void FUN_004212f0(void);
void FUN_004e5b90(int32_t building);
#endif
