#include "ui_rect_text.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004cccb0(uint32_t,uint32_t,uint32_t,uint32_t,uint32_t,uint32_t);
/* Complete4CD340: identical point arithmetic, preserving number style. */
void restore_ui_number_at_rect(uint32_t style,uint32_t value,const int16_t *point,
    uint32_t font,uint32_t color,int32_t dx,int32_t dy)
{
    uint32_t y=(uint32_t)(int32_t)point[1]+U(0xe38da4u)+(uint32_t)dy;
    uint32_t x=(uint32_t)(int32_t)point[0]+U(0xe38da8u)+(uint32_t)dx;
    FUN_004cccb0(style,value,x,y,font,color);
}
void FUN_004cd340(uint32_t style,uint32_t value,const int16_t *point,
    uint32_t font,uint32_t color,int32_t dx,int32_t dy)
{restore_ui_number_at_rect(style,value,point,font,color,dx,dy);}
