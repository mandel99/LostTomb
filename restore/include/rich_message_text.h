#ifndef RESTORE_RICH_MESSAGE_TEXT_H
#define RESTORE_RICH_MESSAGE_TEXT_H
#include "restore_types.h"
#include "sprite_render.h"
#define RT_I(a) (*(int32_t *)(uintptr_t)(a))
#define RT_U(a) (*(uint32_t *)(uintptr_t)(a))
#define RT_W(a) (*(uint16_t *)(uintptr_t)(a))
#define RT_B(a) (*(uint8_t *)(uintptr_t)(a))
static __inline int32_t rt_add(int32_t a,int32_t b){return (int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t rt_sub(int32_t a,int32_t b){return (int32_t)((uint32_t)a-(uint32_t)b);}
static __inline int16_t rt_metric(int32_t image,uint32_t field){if(image>=RT_I(0x126517c))image=0;return *(int16_t *)(uintptr_t)(0x10d69f8u+(uint32_t)image*64u+field);}
void FUN_004c7fd0(int32_t link,int32_t left,int32_t right,int32_t y);
void FUN_004c8040(void);
void FUN_004c8070(const char *text,int32_t x,int32_t y,int32_t width,int32_t calculate,int32_t flags);
int32_t FUN_004c8480(const char *text);
void FUN_004c8590(const char *text,int32_t x,int32_t y,int32_t calculate,int32_t flags);
int32_t FUN_004c8680(int32_t font,uint8_t glyph,int32_t x,int32_t y,int32_t calculate,int32_t flags);
int32_t FUN_004cc020(uint8_t character,int32_t font);
int32_t FUN_004cc070(const char *text);
#endif
