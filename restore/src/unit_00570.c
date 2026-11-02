#include "placement_preview.h"
#include "footprint.h"
#include "building_footprint.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern uint32_t FUN_004893c0(int32_t,int32_t,int32_t,int32_t,uint32_t);

/* Whole43DD70: shoreline building preview. The selection table has20-byte
   records; offsets are byte addresses, not offsets into a DWORD pointer. */
void FUN_0043dd70(uint32_t first,uint32_t allow_existing)
{
    int32_t sx=0,sy=0,result;
    uint32_t offset,image,y,x,table;
    (void)first;
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&sx,&sy);
    sy=I((uint32_t)sy+U(0x70e44cu));
    offset=U(0x6059e8u)*20u;
    image=U(0xf55bb4u+U(0x5d06e4u+offset)*8u)+U(0x5d06e8u+offset);
    result=FUN_0047cc70(I(U(0xbca304u)),I(U(0xbca308u)),1,(uint8_t)allow_existing);
    if(I(U(0xea187cu+U(0xeaac28u)*0x49d8u))<=-5000)result=999;
    else if(result==0){
        uint32_t base=U(0xf56cccu);
        if(I(image)>=I(base)&&I(image)<=I(base+11u)){
            B(0xbca32cu)=(uint8_t)FUN_004893c0(I(U(0xbca304u)),I(U(0xbca308u)),2,2,0x10000);
            image=FUN_00487d30(I(U(0xbc96dcu)),0);
        }else image+=U(0xbc96dcu);
        y=(uint32_t)sy-U(0x70e44cu);x=(uint32_t)sx;
        W(0x70e120u)=0x1fe3u;
        (void)FUN_0053e590(I(image),I(x),I(y));
        (void)FUN_0053f250(I(image),I(x),I(y));
        W(0x70e120u)=0;return;
    }
    for(table=0x5cfcf4u;table<0x5cfd14u;table+=8u){
        x=U(table-4u)+(uint32_t)sx;y=U(table)-U(0x70e44cu)+(uint32_t)sy;
        (void)FUN_0053f4f0(I(U(0xf56be4u)),I(x),I(y),result?0xfffff863u:0x1fe3u);
    }
}
