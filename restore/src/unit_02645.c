#include "compression.h"
#include <string.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0055c2d0(uint32_t *work);

/* Whole55C130. Tables603C20/30/40/50 are BYTE arrays,603D50 WORDs.
   Indexing them as DWORD arrays corrupts the callback slots at work+9B4.
   Store actual compiled C callbacks, never original executable addresses. */
uint32_t FUN_0055c130(RestoreCodecRead read,RestoreCodecWrite write,void *work,
    RestoreCodecContext *context,const uint32_t *mode,const uint32_t *dictionary)
{
    uint32_t base=(uint32_t)(uintptr_t)work,index,group,count,item;
    U(base+0x9b4u)=(uint32_t)(uintptr_t)read;
    U(base+0x9b8u)=(uint32_t)(uintptr_t)write;
    U(base+0x18u)=*dictionary;U(base+0x14u)=*mode;
    U(base+0x9b0u)=(uint32_t)(uintptr_t)context;
    U(base+0xcu)=4;U(base+0x10u)=15;
    switch(*dictionary){
    case 0x1000:U(base+0xcu)=U(base+0xcu)+1u;U(base+0x10u)|=0x20u;
        /* fall through */
    case 0x800:U(base+0xcu)=U(base+0xcu)+1u;U(base+0x10u)|=0x10u;
        /* fall through */
    case 0x400:break;
    default:return 1;
    }
    if(*mode==0){
        for(index=0;index<256u;++index){
            B(base+0x9cu+index)=9;
            W(base+0x3a2u+index*2u)=(uint16_t)(index*2u);
        }
    }else if(*mode==1){
        for(index=0;index<256u;++index){
            B(base+0x9cu+index)=(uint8_t)(B(0x603c50u+index)+1u);
            W(base+0x3a2u+index*2u)=(uint16_t)(W(0x603d50u+index*2u)*2u);
        }
    }else return 2;
    for(group=0;group<16u;++group){
        count=1u<<(B(0x603c20u+group)&31u);
        for(item=0;item<count;++item){
            uint32_t shift=(B(0x603c30u+group)+1u)&31u;
            uint16_t word=(uint16_t)((uint32_t)(uint16_t)item<<shift);
            B(base+0x9cu+index)=(uint8_t)(B(0x603c20u+group)+B(0x603c30u+group)+1u);
            word=(uint16_t)(word|((uint16_t)B(0x603c40u+group)*2u)|1u);
            W(base+0x3a2u+index*2u)=word;++index;
        }
    }
    memcpy((uint8_t *)work+0x5c,(const void *)(uintptr_t)0x603be0u,64);
    memcpy((uint8_t *)work+0x1c,(const void *)(uintptr_t)0x603ba0u,64);
    FUN_0055c2d0((uint32_t *)work);
    return 0;
}
