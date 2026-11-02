#include "ui_rect_text.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004cce90(uint32_t,uint32_t,const char *,uint32_t,uint32_t,int32_t,uint32_t,uint32_t);
/* Complete4CD2E0: signed WORD width; all position arithmetic is32-bit. */
void restore_ui_clipped_number_at_rect(uint32_t value,uint32_t separator,const char *suffix,
    const int16_t *point,uint32_t font,uint32_t color,int32_t dx,int32_t dy)
{
    int32_t width=(int32_t)point[2]-(int32_t)point[0];
    uint32_t y=(uint32_t)(int32_t)point[1]+U(0xe38da4u)+(uint32_t)dy;
    uint32_t x=(uint32_t)(int32_t)point[0]+U(0xe38da8u)+(uint32_t)dx;
    FUN_004cce90(value,separator,suffix,x,y,width,font,color);
}
void FUN_004cd2e0(uint32_t value,uint32_t separator,const char *suffix,
    const int16_t *point,uint32_t font,uint32_t color,int32_t dx,int32_t dy)
{restore_ui_clipped_number_at_rect(value,separator,suffix,point,font,color,dx,dy);}
