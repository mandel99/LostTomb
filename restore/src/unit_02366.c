#include "advisor_shell.h"
#include "sprite_render.h"
extern void FUN_00425730(int32_t x,int32_t y,int16_t *buttons,int32_t count);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((int32_t)*(volatile int16_t *)(uintptr_t)(uint32_t)(a))
#define I(v) ((int32_t)(uint32_t)(v))
/* Whole520F40: full clip/origin/image fields, original13 availability writes,
  14-button draw and signed WORD selected-tab coordinates. */
void FUN_00520f40(void)
{
    uint32_t first=U(0xe38db0u),second=U(0xe38dacu),x,y,p,selected,offset,image;
    U(0xd3b56cu)=(uint32_t)(I(1024u-first)/2);
    U(0xd3b574u)=first;
    U(0xd3b568u)=(uint32_t)(I(768u-second)/2);
    U(0xd3b570u)=second;
    if(B(0xe38dcau)==1)(void)FUN_0053e590(I(U(0xf55c0cu)),0,0);
    x=U(0xe38da8u);y=U(0xe38da4u)+432u;
    (void)FUN_0053e590(I(U(0xf5691cu)),I(x),I(y));
    for(p=0x5f8338u;p<0x5f850cu;p+=36u)B(p)=B(0x6fdc88u+U(p+4u))!=0;
    FUN_00425730(I(x),I(y),(int16_t *)(uintptr_t)0x5f8320u,14);
    selected=U(0x605a44u);
    if(I(selected)>=3){
        offset=selected*36u-72u;
        image=U(0xf56914u)+selected*4u-6u;
        (void)FUN_0053e590(I(image),I(x+(uint32_t)H(0x5f8320u+offset)),
                         I(y+(uint32_t)H(0x5f8322u+offset)));
    }else{
        offset=selected*36u;
        image=U(0xf56914u)+selected*4u-2u;
        (void)FUN_0053e590(I(image),I(x+(uint32_t)H(0x5f82fcu+offset)),
                         I(y+(uint32_t)H(0x5f82feu+offset)));
    }
}
