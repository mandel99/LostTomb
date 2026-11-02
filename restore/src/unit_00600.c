#include "placement_preview.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))

/* Whole441F80: statue preview. Size counts, orientation offsets, player and
   image identifiers are DWORDs; actor grid entries and tint are WORDs. */
void FUN_00441f80(void)
{
    int32_t sx=0,sy=0,size,count,variant,frame,rotation;
    uint32_t base,image,table,tile,pointer,flags,i,x,y;
    int blocked=0,all_invalid=0;
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&sx,&sy);
    sy=I((uint32_t)sy+U(0x70e44cu));
    size=I(U(0x5d06dcu+U(0x6059e8u)*20u));base=U(0xbca300u);
    if(GetAsyncKeyState(0x52)==-32767)U(0xbc9714u)=U(0xbc9714u)+1u;
    if(I(U(0xbc9714u))>15)U(0xbc9714u)=0;
    variant=I(U(0xbc9714u))%4;frame=I(U(0xbc9714u)+4u)/4;if(frame>3)frame=0;
    if(variant<2)image=U(0xf55bb4u+U(0x5d06e4u+U(0x6059e8u)*20u)*8u)+(uint32_t)variant*4u;
    else image=U(size==1?0xf5730cu:size==2?0xf57304u:0xf572fcu)-8u+(uint32_t)variant*4u;
    image+=(uint32_t)frame;
    if(U(0x605a3cu)==0&&(I(U(0xbca304u))<3||I(U(0xbca304u)+(uint32_t)size-1u)>=I(U(0xbca2d0u)-3u)||
            I(U(0xbca308u))<3||I(U(0xbca308u)+(uint32_t)size-1u)>=I(U(0xbca2d4u)-3u))){blocked=1;all_invalid=1;}
    count=I(U(0x5cfcd4u+(uint32_t)size*4u));rotation=I(U(0xbca2e0u))/2;
    table=0x5cfe10u+(uint32_t)rotation*144u;
    for(i=0;I(i)<count;++i){
        tile=base+U(table+i*4u);flags=U(0xaf598cu+tile*4u);
        if(flags&0x10000u){blocked=1;all_invalid=1;}
        else{pointer=U(0xd05720u+tile*4u);if(pointer&&B(pointer+24u)==0){blocked=1;all_invalid=1;}}
        if((flags&0xeefed77fu)||W(0xb350dcu+tile*2u))blocked=1;
    }
    if(I(U(0xea187cu+U(0xeaac28u)*0x49d8u))<=-5000)all_invalid=1;
    else if(!blocked){
        x=(uint32_t)sx;y=(uint32_t)sy-U(0x70e44cu);W(0x70e120u)=0x1fe3u;
        (void)FUN_0053e590(I(image),I(x),I(y));(void)FUN_0053f250(I(image),I(x),I(y));
        W(0x70e120u)=0;return;
    }
    for(i=0;I(i)<count;++i){
        x=U(0x5cfcf0u+i*8u)+(uint32_t)sx;y=U(0x5cfcf4u+i*8u)-U(0x70e44cu)+(uint32_t)sy;
        tile=base+U(table+i*4u);flags=U(0xaf598cu+tile*4u)&0xeefed77fu;
        if(W(0xb350dcu+tile*2u))flags=1;
        (void)FUN_0053f4f0(I(U(0xf56be4u)),I(x),I(y),(all_invalid||flags)?0xfffff863u:0x1fe3u);
    }
}
