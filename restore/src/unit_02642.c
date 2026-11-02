#include "compression.h"
#include <windows.h>
#define s_ERR_COMP_Error_uncompressing__006036b4 (*(char (*)[30])0x006036b4u)
extern uint8_t FUN_0055cbd0(uint32_t,uint32_t,uint32_t,uint32_t);
extern void FUN_004ce6e0(const char *,int,int);
uint32_t FUN_0055c000(uint32_t *checksum,const void *input,uint32_t size,void *output,uint32_t capacity)
{
    HGLOBAL memory=GlobalAlloc(0x42,0x8dd8);void *work;uint32_t result=1;
    RestoreCodecContext context={0};
    if(!memory)return 0;
    work=GlobalLock(memory);if(!work){GlobalFree(memory);return 0;}
    context.encoded.data=(uint8_t *)input;context.encoded.capacity=size;
    context.decoded.data=output;context.decoded.capacity=capacity;
    context.mode=2;context.crc=0xffffffffu;
    if(FUN_0055cbd0((uint32_t)(uintptr_t)FUN_0055bdb0,(uint32_t)(uintptr_t)FUN_0055be50,(uint32_t)(uintptr_t)work,(uint32_t)(uintptr_t)&context)==0&&!context.error)
        *checksum=~context.crc;
    else {FUN_004ce6e0(s_ERR_COMP_Error_uncompressing__006036b4,0,0);result=0;}
    GlobalUnlock(memory);GlobalFree(memory);return result;
}
