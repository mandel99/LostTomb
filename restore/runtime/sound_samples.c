#include "sound_samples.h"
#include "sound_file_lookup.h"
#include "legacy_format.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Opaque legacy FILE values: these functions own their original stream layout. */
extern void *FUN_00561a0a(const char *,const char *);
extern uint32_t FUN_00561994(void *);

int32_t restore_sound_load_file_id(const char *filename)
{
    char fallback[260];void *stream;int32_t result,copy=0;
    const char *path=filename;
    stream=FUN_00561a0a(filename,(const char *)(uintptr_t)0x5763bcu);
    if(!stream){
        /* Native sprintf has no capacity argument. Preserve its ABI/behavior. */
        (void)FUN_00561a36(fallback,(const char *)(uintptr_t)0x5763b4u,(int)B(0xe38e62u),filename);
        stream=FUN_00561a0a(fallback,(const char *)(uintptr_t)0x5763bcu);
        if(!stream)return -1;
        path=fallback;copy=1;
    }
    result=FUN_0055dd3f(&RESTORE_SOUND->files,path,copy);
    (void)FUN_00561994(stream);
    return result;
}
void restore_sound_load_ambient_sample(int32_t bank,const char *filename,int32_t mode)
{
    uint32_t row=(uint32_t)bank*48u;int32_t id;
    if(mode==0){
        id=FUN_00539c60(filename);
        U(0xf54578u+row)=(uint32_t)id;
    }else if(mode==1&&(int32_t)U(0xf5457cu+row)<10){
        id=FUN_00539c60(filename);
        U(0xf54550u+row+U(0xf5457cu+row)*4u)=(uint32_t)id;
        U(0xf5457cu+row)=U(0xf5457cu+row)+1u;
    }
}

void restore_sound_initialize_ambient_samples(void)
{
    static const uint32_t filenames[6][8]={
        {0x5fb154u,0x5fb13cu,0x5fb124u,0x5fb10cu,0x5fb0f4u,0x5fb0dcu,0x5fb0c4u,0x5fb0acu},
        {0x5fb094u,0x5fb07cu,0x5fb064u,0x5fb04cu,0x5fb034u,0x5fb01cu,0x5fb004u,0x5fafecu},
        {0x5fafd4u,0x5fafbcu,0x5fafa4u,0x5faf8cu,0x5faf74u,0x5faf5cu,0x5faf44u,0x5faf2cu},
        {0x5faf14u,0x5faefcu,0x5faee4u,0x5faeccu,0x5faeb4u,0x5fae9cu,0x5fae84u,0x5fae6cu},
        {0x5fae54u,0x5fae3cu,0x5fae24u,0x5fae0cu,0x5fadf4u,0x5faddcu,0x5fadc4u,0x5fadacu},
        {0x5fad94u,0x5fad7cu,0x5fad64u,0x5fad4cu,0x5fad34u,0x5fad1cu,0x5fad04u,0x5facecu},
    };
    uint32_t bank,i,row;
    for(bank=0;bank<6u;++bank){
        row=0xf54550u+bank*48u;
        for(i=0;i<10u;++i)U(row+i*4u)=UINT32_MAX;
        U(row+40u)=UINT32_MAX;U(row+44u)=0;
    }
    for(bank=0;bank<6u;++bank){
        FUN_0053bc80((int32_t)bank,(const char *)(uintptr_t)filenames[bank][0],0);
        for(i=1;i<8u;++i)FUN_0053bc80((int32_t)bank,(const char *)(uintptr_t)filenames[bank][i],1);
    }
}
