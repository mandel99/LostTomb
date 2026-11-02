#include "refresh_dispatch.h"
#include "terrain_redraw.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004776f0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004770f0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475dc0(void);
extern void FUN_00477430(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047bd50(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
void FUN_00477dd0(int32_t x_in,int32_t y_in,int32_t radius_in)
{
    uint32_t x=(uint32_t)x_in,y=(uint32_t)y_in,r=(uint32_t)radius_in;
    uint32_t right=U(0xbca2d0u),left=x-r,top=y-r,bottom;
    if((int32_t)left<0)left=0;if((int32_t)right>=(int32_t)(x+r))right=x+r;
    bottom=U(0xbca2d4u);if((int32_t)top<0)top=0;if((int32_t)bottom>=(int32_t)(y+r))bottom=y+r;
    FUN_00475cc0((int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom,1);
    FUN_004776f0((int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    FUN_004770f0((int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    FUN_00475dc0();
    FUN_00477430((int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    FUN_004767d0((int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom,0,1);
    FUN_004777c0((int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
    FUN_0047b590((int32_t)(left-1u),(int32_t)(top-1u),(int32_t)(right+1u),(int32_t)(bottom+1u));
    FUN_0047bd50((int32_t)(left-1u),(int32_t)(top-1u),(int32_t)(right+1u),(int32_t)(bottom+1u));
    FUN_00477500((int32_t)left,(int32_t)top,(int32_t)right,(int32_t)bottom);
}
