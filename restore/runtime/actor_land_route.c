#include "actor_routing.h"
#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole51B400: walk the distance gradient backwards. Scan using the live
   neighbor table, move by the native fixed deltas, skip the reverse of the
   last step, prefer the geometric AL on ties, then reverse the owned buffer.
   Reaching500 bytes fails before writing a route, including a500-step path. */
int32_t FUN_0051b400(int32_t directions,int32_t slot,int32_t x,int32_t y,int32_t tx,int32_t ty)
{
    static const int8_t dx[8]={0,1,1,1,0,-1,-1,-1};
    static const int8_t dy[8]={-1,-1,0,1,1,1,0,-1};
    static const int32_t delta[8]={-228,-227,1,229,228,227,-1,-229};
    uint32_t tile=U(0xbca2d8u)+(uint32_t)ty*228u+(uint32_t)tx;
    uint32_t count=0,stride=directions==8?1u:2u,best,weight,preferred,next,i;
    int32_t chosen,previous=-1;
    U(0xeae004u)=tile;
    best=U(0xeed760u+tile*4u);if(I(best)<=0)return 0;
    if(I(best)>1)do{
        best=U(0xeed760u+tile*4u);
        preferred=FUN_0051cba0(tx,ty,x,y)&255u;chosen=-1;
        for(i=0;i<8u;i+=stride){
            if((int32_t)i==previous)continue;
            next=tile+U(0x5f80fcu+i*4u);weight=U(0xeed760u+next*4u);
            if(weight&&(I(weight)<I(best)||(weight==best&&(i==preferred||chosen==-1)))){
                best=weight;chosen=(int32_t)i;
            }
        }
        if(chosen==-1)return 0;
        tx=I((uint32_t)tx+(uint32_t)(int32_t)dx[chosen]);
        ty=I((uint32_t)ty+(uint32_t)(int32_t)dy[chosen]);
        tile+=(uint32_t)delta[chosen];previous=(chosen+4)&7;
        B(0xf533c8u+count)=(uint8_t)previous;++count;
        if(count>=500u)return 0;
    }while(I(best)>1);
    for(i=0;i<count;++i)B(0xc8ad38u+(uint32_t)slot*500u+i)=B(0xf533c8u+count-1u-i);
    return (int32_t)count;
}
