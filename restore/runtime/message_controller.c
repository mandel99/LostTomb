#include "pending_message.h"
#include "menu_input.h"
#include "actor_order_action.h"
#include "text_selection.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_004cc500(int32_t group);
extern void FUN_0040f8f0(uint32_t *destination,uint32_t bytes,uint8_t value);
extern void FUN_004cbea0(uint32_t source,char *destination,int32_t bytes);
void FUN_0041d310(void)
{
    uint32_t x,y;int32_t hover;
    FUN_004cc500(7);y=U(0xe38da4u);x=U(0xe38da8u);
    hover=FUN_00426450((int32_t)(x+50u),(int32_t)(y+246u),(const RestoreMenuButton *)(uintptr_t)0x57c560u,1);
    y=U(0xe38da4u);U(0x7027e4u)=(uint32_t)hover;x=U(0xe38da8u);
    if(FUN_004261d0((int32_t)(x+50u),(int32_t)(y+246u),(RestoreMenuButton *)(uintptr_t)0x57c560u,1))return;
    if(!U(0xe20998u))return;
    if(!B(0x7027e0u))restore_actor_order_message(217,0,0);
    B(0x7027e0u)=1;FUN_0041d3f0();
}
void FUN_004e2970(int32_t message)
{
    uint32_t slot,record,font,text;int16_t width,length;
    if(!B(0xe38ec1u))return;
    if(message==2&&U(0x605a3cu)==1)return;
    if(!B(0xe38ddau))return;
    for(slot=0;slot<5;++slot)if(!W(0xe94e2eu+slot*120u))break;
    if(slot==5)return;
    record=0xe94e28u+slot*120u;U(0xf53f34u+slot*4u)=0;W(record+6u)=1;
    if(message==1)FUN_004cc110(17,(int32_t)U(0xbca2e0u));
    else FUN_004cc110(19,(int32_t)((uint32_t)message-2u));
    font=U(0xf56f8cu);text=U(0xe20768u);U(0xe94e20u)=1;W(record)=(uint16_t)message;
    W(record+10u)=(uint16_t)FUN_004cbf30((const uint8_t *)(uintptr_t)text,(int32_t)font);
    text=U(0xe20768u);W(record+8u)=(uint16_t)FUN_004cc000((char *)(uintptr_t)text);
    width=(int16_t)W(record+10u);W(record+10u)=width<=100?200u:width<=200?300u:width<=300?400u:460u;
    U(record+12u)=U(0xe92e0cu);FUN_0040f8f0((uint32_t *)(uintptr_t)(record+18u),100,0);
    length=(int16_t)W(record+8u);text=U(0xe20768u);
    /* Native clears100bytes, then copies the signed WORD length without an
       additional100byte cap. Preserve that caller/resource domain exactly. */
    FUN_004cbea0(text,(char *)(uintptr_t)(record+18u),length);
}
