#include "three_way_button.h"
#include "message_panel_input.h"
#include "sprite_render.h"
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
uint8_t FUN_00425840(void)
{
    uint32_t saved=U(0x7056b8u),mode;int32_t width,left,cursor;
    U(0x705698u)=0;
    if(!FUN_00425990(0,0,(const void *)(uintptr_t)0x5c79c8u,1)){
        U(0x7056b8u)=saved;return 0;
    }
    width=(int16_t)W(0x5c79ccu);left=(int16_t)W(0x5c79c8u);cursor=(int32_t)U(0xe38e34u);
    U(0x7056b8u)=20;
    if(cursor<left+width/3)mode=1;
    else mode=cursor>=left+(width*2)/3?2u:0u;
    U(0xd3b198u)=mode;U(0x5c79e4u)=mode;B(0xe38dc9u)=1;
    FUN_00425900();
    U(0x705698u)=U(0xd3b198u)+1u;
    return 1;
}
void FUN_00425900(void)
{
    int32_t index=(int16_t)W(0x5c79d2u),x=(int16_t)W(0x5c79c8u),y;
    uint8_t pressed=B(0x5c79e1u);uint32_t image=U(0xf55bb4u+(uint32_t)index*8u);
    U(0xd3b5e0u)=(uint32_t)x;y=(int16_t)W(0x5c79cau);
    U(0xd3b5dcu)=(uint32_t)y;U(0xd3b5c0u)=image;
    if(pressed){
        if(pressed==20)B(0xe38dc8u)=1;
        image+=U(0x5c79e4u)+4u;U(0xd3b5c0u)=image;
    }else if(B(0x5c79e2u)){
        image+=U(0x5c79e4u)+1u;U(0xd3b5c0u)=image;
    }
    /* Actual global-manager C facade. The finite renderer is test-only. */
    (void)FUN_0053e590((int32_t)image,x,y);
}
