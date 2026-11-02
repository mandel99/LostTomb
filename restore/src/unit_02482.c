#include "restore_types.h"
#include "sound_streams.h"
#include "legacy_format.h"

#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern undefined4 *FUN_00561a0a(undefined4 path,undefined4 mode);
extern uint32_t FUN_00561994(restore_legacy_FILE *stream);

static uint32_t terminated_bytes(uint32_t source)
{
    uint32_t size=0;
    do{++size;}while(B(source+size-1u));
    return size;
}
static void copy_words(uint32_t target,uint32_t source,uint32_t size)
{
    uint32_t index;
    for(index=0;index<(size&~3u);index+=4u)U(target+index)=U(source+index);
    for(;index<size;++index)B(target+index)=B(source+index);
}

/* Whole539B20,108 instructions, cdecl1/RET0 and full EAX result.
   Native300-byte scratch, three NUL-inclusive DWORD/remainder copies,
   and the original relative-path then configured-drive file lookup.
   All sound members receive their genuine explicit E92BF8 receiver.
   The four actual41A850 callers discard volatile GPRs and flags. */
uint32_t FUN_00539b20(char *name)
{
    uint32_t scratch[75],local=(uint32_t)(uintptr_t)scratch,size;
    undefined4 *stream;
    if(!U(0xf55b88u)||!B(0xe38e65u))return 0;
    size=terminated_bytes(0x5f90b8u);
    copy_words(local,0x5f90b8u,size);
    copy_words(local+size-1u,(uint32_t)(uintptr_t)name,
               terminated_bytes((uint32_t)(uintptr_t)name));
    copy_words(0xf558f4u,local,terminated_bytes(local));
    stream=FUN_00561a0a(local,0x5763bcu);
    if(!stream){
        /* The failed fopen leaves EAX=0 before native MOV AL,drive,
           so the sprintf %c argument is precisely zero-extended BYTE. */
        (void)FUN_00561a36((char *)(uintptr_t)0xf558f4u,
            (const char *)(uintptr_t)0x5763b4u,(int)B(0xe38e62u),(const char *)scratch);
        stream=FUN_00561a0a(0xf558f4u,0x5763bcu);
        if(!stream)return UINT32_MAX;
    }
    (void)FUN_00561994((restore_legacy_FILE *)stream);
    FUN_0055f639(RESTORE_SOUND,1,(int32_t)U(0xe38eccu));
    FUN_0055f32f(RESTORE_SOUND,(int32_t)U(0xe38eccu));
    return FUN_0055f449(RESTORE_SOUND,(const char *)(uintptr_t)0xf558f4u,1,1);
}
