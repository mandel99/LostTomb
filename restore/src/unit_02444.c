#include <stdint.h>
#include "sprite_render.h"
#define U(a) (*(int32_t *)(uintptr_t)(a))
void FUN_00536710(int style,int x,int y,int segments,int pressed)
{
    int cap=U(0x00f53f10u+(uint32_t)style*4u),i,base=U(0x00f56494u);
    if(cap<=4) return;
    if(cap<15) {
        uintptr_t tuple=0x005f81e4u+(uint32_t)(cap-5)*12u;
        int image=U(tuple)+24+pressed*8+base;
        U(0x00d3b5dc)=U(tuple+8)+y;
        U(0x00d3b5e0)=(segments-2)*16/2+U(tuple+4)+x;
        FUN_0053e590(image,U(0x00d3b5e0),U(0x00d3b5dc));
        return;
    }
    cap-=13;
    if(cap<=segments){x+=(segments-cap)*16/2;segments=cap;}
    for(i=0;i<segments;++i) {
        U(0x00d3b5e0)=i*16+x;U(0x00d3b5dc)=y;
        U(0x00d3b5c0)=base+pressed*3+(i==0?0:i<segments-1?1:2);
        FUN_0053e590(U(0x00d3b5c0),U(0x00d3b5e0),y);
    }
}
