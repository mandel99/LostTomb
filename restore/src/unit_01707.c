#include "ui_rect_text.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004ccbb0(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
/* Complete4CD1F0: signed WORD point, DWORD origin and wrapped additions. */
void restore_ui_label_at_rect(uint32_t group,uint32_t entry,const int16_t *point,
    uint32_t font,uint32_t color,int32_t dx,int32_t dy)
{
    uint32_t y=(uint32_t)(int32_t)point[1]+U(0xe38da4u)+(uint32_t)dy;
    uint32_t x=(uint32_t)(int32_t)point[0]+U(0xe38da8u)+(uint32_t)dx;
    FUN_004ccbb0(group,entry,x,y,font,color);
}
void FUN_004cd1f0(uint32_t group,uint32_t entry,const int16_t *point,
    uint32_t font,uint32_t color,int32_t dx,int32_t dy)
{restore_ui_label_at_rect(group,entry,point,font,color,dx,dy);}
