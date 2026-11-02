#include "actor_routing.h"
#include "actor_transport.h"
#include "actor_constable.h"
#include "actor_order_flood.h"
#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)(int16_t)W(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
#define XY H(0xbcd60cu+off),H(0xbcd60eu+off)
#define COORDS XY,H(0xbcd61cu+off),H(0xbcd61eu+off)

/* Whole4B28F0. The allocator always writes this owned out-only slot before
   reading it; the original slot's incoming scratch ECX is not a logical input.
   Land returns the selected1-based goal index even if route construction
   fails; water returns the constructed length. Commit releases any route
   installed by callbacks, then reloads the escaped out slot. */
int32_t FUN_004b28f0(int32_t actor,int32_t targets,uint32_t count)
{
    uint32_t off=(uint32_t)actor*388u,result=0,length=0,terrain;
    int32_t slot,difference;int diagonal=1;uint8_t water;
    W(0xbcd632u+off)=0;W(0xbcd634u+off)=0;W(0xbcd636u+off)=0;
    if(!FUN_00489970(actor,&slot))return 0;
    water=B(0xbcd66eu+off);
    if(water){
        if(U(0x605a3cu)==1u)(void)FUN_0051d160(H(0x784c8cu),H(0x784c8eu));
        else if(water==2u)(void)FUN_0051d160(XY);
        else(void)FUN_0051ce90(XY);
        if(B(0xbcd66eu+off)==2u)length=(uint32_t)FUN_0051dc10(slot,COORDS,1);
        else if(B(0xbcd602u+off)==78u&&(B(0xbcd630u+off)==9u||U(0xbcd754u+off)==9u)){
            length=(uint32_t)FUN_0051d980(slot,COORDS,I(U(0xbcd738u+off)));
            if(!length)length=(uint32_t)FUN_0051dc10(slot,COORDS,0);
        }else length=(uint32_t)FUN_0051dc10(slot,COORDS,0);
        result=length;
    }else{
        terrain=B(0xbcd66au+off);
        switch(terrain){
        case 2:
            result=(uint32_t)FUN_0051a9d0(XY,targets,I(count),0,100000);
            if(!result)result=(uint32_t)FUN_0051b220(XY,targets,I(count));break;
        case 4:result=(uint32_t)FUN_0051a220(XY,targets,I(count));diagonal=0;break;
        case 0:case 5:
            result=(uint32_t)FUN_00518b30(XY,(const int32_t *)(uintptr_t)(uint32_t)targets,I(count));break;
        case 3:
            result=(uint32_t)FUN_00519e30(XY,targets,I(count));diagonal=0;
            if(!result){diagonal=1;result=(uint32_t)FUN_005190a0(XY,(int32_t *)(uintptr_t)(uint32_t)targets,I(count));}break;
        case 1:result=(uint32_t)FUN_00519e30(XY,targets,I(count));diagonal=0;break;
        case 9:result=(uint32_t)FUN_00518dd0(XY,targets,I(count));break;
        case 10:result=(uint32_t)FUN_0051d6d0(XY,targets,I(count));break;
        default:return 0;
        }
        if(!result)return 0;
        difference=I(U((uint32_t)targets+result*4u-4u)-U(0xbca2d8u));
        W(0xbcd61cu+off)=(uint16_t)(difference%228);
        W(0xbcd61eu+off)=(uint16_t)(difference/228);
        if(!diagonal){
            length=(uint32_t)FUN_0051b400(4,slot,COORDS);
            if(I(length)>0)goto commit;
        }
        length=(uint32_t)FUN_0051b400(8,slot,COORDS);
    }
    if(!length)return I(result);
commit:
    FUN_004b3250(actor);
    W(0xbcd632u+off)=(uint16_t)slot;W(0xbcd636u+off)=(uint16_t)length;
    W(0xd04e58u+(uint32_t)slot*2u)=(uint16_t)actor;
    return I(result);
}
#undef XY
#undef COORDS
