#include "actor_order_action.h"
#include "text_selection.h"
#include "menu_text_glyph.h"
#define U(a) (*(uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(int16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(uint32_t)(a))
void FUN_0040f8f0(uint32_t *destination,uint32_t bytes,uint8_t value);
void FUN_004cbea0(int32_t source,uint8_t *destination,int32_t bytes);
int32_t FUN_004cc000(char *text);
void FUN_004e2970(int32_t message)
{
    uint32_t slot=0,record,text,font;int16_t width;
    if(!B(0xe38ec1))return;
    if(message==2&&U(0x605a3c)==1)return;
    if(!B(0xe38dda))return;
    while(W(0xe94e2eu+slot*120u)!=0){if(++slot>=5)return;}
    record=0xe94e28u+slot*120u;U(0xf53f34u+slot*4u)=0;W(record+6)=1;
    if(message==1)FUN_004cc110(17,(int32_t)U(0xbca2e0));
    else FUN_004cc110(19,(int32_t)((uint32_t)message-2u));
    font=U(0xf56f8c);text=U(0xe20768);U(0xe94e20)=1;W(record)=(int16_t)message;
    W(record+10)=(int16_t)FUN_004cbf30((const uint8_t *)(uintptr_t)text,(int32_t)font);
    W(record+8)=(int16_t)FUN_004cc000((char *)(uintptr_t)U(0xe20768));width=W(record+10);
    W(record+10)=width<=100?200:width<=200?300:width<=300?400:460;
    U(record+12)=U(0xe92e0c);FUN_0040f8f0((uint32_t *)(uintptr_t)(record+18),100,0);
    FUN_004cbea0((int32_t)U(0xe20768),(uint8_t *)(uintptr_t)(record+18),W(record+8));
}
