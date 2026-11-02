#include "compression.h"
#include <windows.h>
extern void FUN_004ce6e0(const char *,int,int);
/* Whole55BF10. True52-byte callback context and explicit input/output
   callbacks; the native workspace allocation and capacity stay unchanged. */
uint32_t FUN_0055bf10(uint32_t *checksum,uint32_t *encoded_size,const void *input,
                     void *output,uint32_t size)
{
    HGLOBAL memory=GlobalAlloc(0x42,0x8dd8);
    void *work;uint32_t result=1;
    RestoreCodecContext context={0};
    if(!memory)return 0;
    work=GlobalLock(memory);if(!work){GlobalFree(memory);return 0;}
    context.encode_input.data=(uint8_t *)input;context.encode_input.capacity=size;
    context.encoded.data=output;context.encoded.capacity=size;
    context.mode=1;context.crc=UINT32_MAX;
    if(FUN_0055c130(FUN_0055bdb0,FUN_0055be50,work,&context,
            (const uint32_t *)(uintptr_t)0x126519cu,
            (const uint32_t *)(uintptr_t)0x603650u)==0&&!context.error){
        *checksum=~context.crc;*encoded_size=context.encoded_size;
    }else{
        FUN_004ce6e0((const char *)(uintptr_t)0x603688u,0,0);result=0;
    }
    GlobalUnlock(memory);GlobalFree(memory);return result;
}
