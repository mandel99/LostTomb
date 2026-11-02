#include "placement_preview.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
extern uint32_t FUN_0047d350(void);

/* Whole43E090: single building preview. Native47D350 is the shared zero
   leaf: its extra evaluated inputs do not form a consumed C argument ABI. */
void FUN_0043e090(void)
{
    int32_t sx=0,sy=0;
    uint32_t offset,image,result,table,x,y;
    FUN_004f0ba0(I(U(0xbca304u)),I(U(0xbca308u)),&sx,&sy);
    sy=I((uint32_t)sy+U(0x70e44cu));offset=U(0x6059e8u)*20u;
    image=U(0xf55bb4u+U(0x5d06e4u+offset)*8u)+U(0x5d06e8u+offset);
    (void)U(0xbca308u);(void)U(0xbca304u);result=FUN_0047d350();
    if(result==0){
        image+=U(0xbc96d4u);x=(uint32_t)sx;y=(uint32_t)sy-U(0x70e44cu);
        W(0x70e120u)=0x1fe3u;
        (void)FUN_0053e590(I(image),I(x),I(y));
        (void)FUN_0053f250(I(image),I(x),I(y));
        W(0x70e120u)=0;return;
    }
    for(table=0x5cfcf0u;table<0x5cfd10u;table+=8u){
        y=U(table+4u)-U(0x70e44cu)+(uint32_t)sy;x=U(table)+(uint32_t)sx;
        (void)FUN_0053f4f0(I(U(0xf56be4u)),I(x),I(y),0xfffff863u);
    }
}
