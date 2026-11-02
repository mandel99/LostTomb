#include "actor_routing.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Whole489970, AL only. The out slot is live and may alias mapped state.
   Reusing this actor's old slot clears both backlinks before returning it. */
uint8_t FUN_00489970(int32_t actor,int32_t *route)
{
    uint32_t p=0xbcd632u+(uint32_t)actor*388u,out=(uint32_t)(uintptr_t)route,index,other;
    int32_t owner;
    if(H(p)){
        index=(uint32_t)H(p);owner=H(0xd04e58u+index*2u);
        if(owner==actor){
            U(out)=index;index=(uint32_t)H(p);W(p)=0;W(0xd04e58u+index*2u)=0;return 1;
        }
        W(p)=0;
    }
    U(out)=1;
    for(;;){
        index=U(out);owner=H(0xd04e58u+index*2u);
        if(!owner)return 1;
        if(owner<0||owner>=2000){W(0xd04e58u+U(out)*2u)=0;return 1;}
        other=(uint32_t)owner*388u;
        if(H(0xbcd632u+other)!=(int32_t)index){W(0xd04e58u+U(out)*2u)=0;return 1;}
        if(B(0xbcd606u+other)!=1){
            W(0xbcd632u+other)=0;W(0xd04e58u+U(out)*2u)=0;return 1;
        }
        ++index;U(out)=index;
        if((int32_t)index>=1000){U(out)=0;return 0;}
    }
}
