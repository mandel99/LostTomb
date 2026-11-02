#ifndef RESTORE_MENU_TEXT_GLYPH_MEMORY_H
#define RESTORE_MENU_TEXT_GLYPH_MEMORY_H
#include "menu_text_glyph.h"
#define TG_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define TG_I(a) (*(volatile int32_t *)(uintptr_t)(uint32_t)(a))
#define TG_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define TG_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
static __inline int32_t tg_add(int32_t a,int32_t b){return(int32_t)((uint32_t)a+(uint32_t)b);}
static __inline int32_t tg_sub(int32_t a,int32_t b){return(int32_t)((uint32_t)a-(uint32_t)b);}
static __inline int32_t tg_metric(uint32_t image,uint32_t offset,int32_t count)
{
    if((int32_t)image>=count)image=0;
    return *(volatile int16_t *)(uintptr_t)(0x010d69f8u+image*64u+offset);
}
#endif
