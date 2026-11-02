#include "menu_input.h"
#define I(a) (*(int32_t *)(uintptr_t)(a))
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define W(a) (*(int16_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
extern int FUN_004cde40(int,int);
extern int FUN_004cde60(int,int);
extern void FUN_0053b090(int);
static int32_t add(int32_t a,int32_t b) {return (int32_t)((uint32_t)a+(uint32_t)b);}
void FUN_004cf210(void)
{
    POINT point;
    uint32_t ticks;
    int xscale,yscale;
    if(!restore_input_cursor(&point)) return;
    ticks=U(0xe92e0c);
    I(0xe38dc4)=point.x;I(0xe38dc0)=point.y;
    if(I(0xe38dcc)) {W(0xe38e0a)=(int16_t)point.x;W(0xe38e08)=(int16_t)point.y;}
    if(I(0xe38e14)==1) {
        if(I(0xe38e34)>add(I(0xe38e1c),50)||I(0xe38e34)<add(I(0xe38e1c),-50)||
           I(0xe38e30)>add(I(0xe38e18),50)||I(0xe38e30)<add(I(0xe38e18),-50)) I(0xe38e14)=2;
    }
    I(0xe38e10)=I(0xe38e34);I(0xe38e0c)=I(0xe38e30);
    B(0xe38ded)=B(0xe38df4);B(0xe38df9)=B(0xe38e00);I(0xe38e40)=0;
    xscale=FUN_004cde60(I(0xe38d9c),(int32_t)(U(0xe39138)-U(0xe39130)));
    yscale=FUN_004cde60(I(0xe38d98),(int32_t)(U(0xe3913c)-U(0xe39134)));
    I(0xe38e34)=FUN_004cde40(W(0xe38e0a),xscale);
    I(0xe38e30)=FUN_004cde40(W(0xe38e08),yscale);
    B(0xe38df4)=(B(0xe38e07)&2)!=0;B(0xe38e00)=(B(0xe38e07)&1)!=0;
    I(0xe38e38)=0;B(0xe38e03)=0;B(0xe38e04)=0;B(0xe38df7)=0;B(0xe38df8)=0;
    if(I(0xe38e10)!=I(0xe38e34)||I(0xe38e0c)!=I(0xe38e30)) {
        I(0xe38e40)=1;U(0xe36c8c)=ticks;U(0xe38e3c)=0;
    } else U(0xe38e3c)=ticks-U(0xe36c8c);
    if(B(0xe38e00)!=B(0xe38df9)) {
        I(0xe38e40)=1;I(0xe38e38)=1;
        if(B(0xe38e00)) {B(0xe38e03)=B(0xe38e00);FUN_0053b090(0);}
        else B(0xe38e04)=1;
    }
    if(B(0xe38df4)!=B(0xe38ded)) {
        I(0xe38e40)=1;I(0xe38e38)=1;
        if(B(0xe38df4)==1) B(0xe38df7)=B(0xe38df4);
        else if(B(0xe38df4)==0) B(0xe38df8)=1;
    }
    B(0xe38e06)=0;
}
