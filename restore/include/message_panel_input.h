#ifndef RESTORE_MESSAGE_PANEL_INPUT_H
#define RESTORE_MESSAGE_PANEL_INPUT_H
#include "message_panel.h"
#include "menu_input.h"
#define MI_I(a) (*(int32_t *)(uintptr_t)(a))
#define MI_U(a) (*(uint32_t *)(uintptr_t)(a))
#define MI_W(a) (*(uint16_t *)(uintptr_t)(a))
#define MI_B(a) (*(uint8_t *)(uintptr_t)(a))
static __inline int32_t mi_add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t mi_sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static __inline int32_t mi_cells(int32_t a){return (int32_t)((uint32_t)a*16u);}
void FUN_004c7770(void);
void FUN_004072c0(void);
int32_t FUN_004c7db0(void);
void FUN_004c7f30(void);
int32_t FUN_004c7f70(void);
void FUN_004c7e90(void);
void FUN_004c7ee0(void);
void FUN_004c8040(void);
int32_t FUN_00425990(int32_t x,int32_t y,const void *buttons,int32_t count);
void FUN_00425bc0(int32_t x,int32_t y,const void *buttons,int32_t count,int32_t first);
int32_t FUN_004cde40(int32_t value,int32_t percent);
int32_t FUN_004cde60(int32_t value,int32_t maximum);
#endif
