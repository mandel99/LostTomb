#include "music_configuration.h"
#include "byte_tokenizer.h"
#include "legacy_format.h"
#include <stdlib.h>
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void *FUN_00561a0a(const char *,const char *);
extern char *FUN_00561e4d(char *,int,void *);
extern int FUN_00561994(void *);
static int token_equal(const char *left,uint32_t right)
{
    uint32_t p=(uint32_t)(uintptr_t)left;
    uint8_t value;
    for(;;){value=B(p);if(value!=B(right))return 0;if(!value)return 1;++p;++right;}
}
void restore_music_load_configuration(RestoreMusicManager *self)
{
    struct {char path[100],line[200];} buffers;
    char delimiters[5],*name,*token,*end;
    uint32_t base=(uint32_t)(uintptr_t)self,row;
    int32_t category=0,delay=0,minimum=1,maximum=100;
    int ambient;
    void *stream=FUN_00561a0a((const char *)(uintptr_t)0x5f9094u,(const char *)(uintptr_t)0x5f90a0u),*test;
    if(!stream)return;
    *(uint32_t *)(void *)delimiters=U(0x5f908cu);delimiters[4]=(char)B(0x5f9090u);
    while(FUN_00561e4d(buffers.line,200,stream)){
        if(buffers.line[0]==';'||!buffers.line[0])continue;
        name=FUN_00561db1(buffers.line,delimiters);if(!name)continue;
        if(token_equal(name,0x5f9084u)){
            row=base+(uint32_t)category*0x1b10u;
            U(row+0x1a40u+U(row+0x1b08u)*4u)=1;continue;
        }
        token=FUN_00561db1(0,delimiters);ambient=token_equal(token,0x5f9080u);
        token=FUN_00561db1(0,delimiters);if(token)category=(int32_t)strtol(token,&end,10);
        token=FUN_00561db1(0,delimiters);if(token)delay=(int32_t)strtol(token,&end,10);
        token=FUN_00561db1(0,delimiters);if(token)minimum=(int32_t)strtol(token,&end,10);
        token=FUN_00561db1(0,delimiters);if(token)maximum=(int32_t)strtol(token,&end,10);
        (void)FUN_00561a36(buffers.path,(const char *)(uintptr_t)0x5f9070u,name);
        test=FUN_00561a0a(buffers.path,(const char *)(uintptr_t)0x5763bcu);
        if(test)(void)FUN_00561994(test);
        /* Original539470..53948C CD fallback, disabled at the user's request.
           Startup uses local files and does not require CD music.
        else{
            uint32_t drive=B(0xe38e62u);
            (void)FUN_00561a36(buffers.path,(const char *)(uintptr_t)0x5f905cu,drive,name);
        }
        */
        (void)FUN_005395e0(RESTORE_MUSIC,buffers.path,category,delay,minimum,maximum,ambient);
    }
    (void)FUN_00561994(stream);
}
