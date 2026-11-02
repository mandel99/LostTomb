#include <windows.h>
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern int32_t FUN_004cc000(const char *);
extern void FUN_004cd9d0(int32_t,int32_t,int32_t,int32_t,uint32_t);
/* Whole4CF7B0. Game and desktop dimensions are DWORDs. Compute both
   coordinate pairs after measuring text, then retain them across drawing. */
void FUN_004cf7b0(const char *text)
{
    int32_t count=FUN_004cc000(text),x,y,desktop_x,desktop_y,extra,half;
    SIZE size={0,0};HDC dc;
    if(count<=0)return;
    dc=GetDC(NULL);if(!dc)return;
    (void)GetTextExtentPointA(dc,text,count,&size);
    desktop_x=I(U(0xe38db8u)-(uint32_t)size.cx)/2;
    x=I(U(0xe38db0u)-(uint32_t)size.cx)/2;
    half=I(U(0xe38dacu)-(uint32_t)size.cy)/2;
    extra=I((uint32_t)half*50u)/100;
    y=I((uint32_t)half+(uint32_t)extra);
    desktop_y=I((uint32_t)(I(U(0xe38db4u)-(uint32_t)size.cy)/2)+(uint32_t)extra);
    if(U(0xe38db4u)!=U(0xe38dacu))y=I((uint32_t)y-24u);
    FUN_004cd9d0(I((uint32_t)x-26u),I((uint32_t)y-11u),I((uint32_t)size.cx+52u),I((uint32_t)size.cy+22u),0);
    FUN_004cd9d0(I((uint32_t)x-25u),I((uint32_t)y-10u),I((uint32_t)size.cx+50u),I((uint32_t)size.cy+20u),65535);
    (void)SetBkColor(dc,0xffffffu);
    (void)TextOutA(dc,desktop_x,desktop_y,text,count);
    (void)ReleaseDC(NULL,dc);
}
