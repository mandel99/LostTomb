#include "video_open.h"
#include "map_file_load.h"
#include "media_bindings.h"
#include "graphics_session.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern uint32_t FUN_004ce450(uint32_t path);
extern uint32_t FUN_004cff20(void);
extern void FUN_0051f780(void); /* Authenticated one-byte RET; extras 0/1 discarded. */
uint32_t FUN_00538dc0(const char *path,uint32_t x,uint32_t y,uint32_t flags,uint32_t return_mode)
{
    uint32_t kind;
    if(!U(0xf54118u))return 0;
    U(0xf53ffcu)=return_mode;
    if(!FUN_00538e30(path,x,y,flags)){
        kind=U(0xf54118u);B(0xe38ddau)=(uint8_t)return_mode;
        if(kind==2)B(0xe38dcau)=1;
        return 0;
    }
    B(0xe38ddau)=19;return 1;
}
uint32_t FUN_00538e30(const char *path,uint32_t x,uint32_t y,uint32_t flags)
{
    uint32_t initialized=U(0xf54050u),format,sound=0,manager,driver,handle,available;
    U(0xf54054u)=0;
    if(!initialized){
        manager=U(0x12663acu);driver=U(manager+0x178u);
        (void)_SmackSoundUseMSS_4((uintptr_t)driver);U(0xf54050u)=1;
    }
    FUN_004ce6e0((char *)(uintptr_t)0x5f9014u,(int32_t)(uintptr_t)path,0);
    U(0xf54008u)=x;format=U(0xe3914cu);
    U(0xf54114u)=0;U(0xf54004u)=y;U(0xf54000u)=flags;
    U(0xf53ff4u)=format==0x565u?0xc0000000u:0x80000000u;
    FUN_004cbea0((uint32_t)(uintptr_t)path,(char *)(uintptr_t)0xf5400cu,64);
    handle=FUN_004ce450(0xf5400cu);U(0xf5404cu)=handle;
    if(!handle)(void)FUN_004ce100((char *)(uintptr_t)0xf5400cu);
    manager=U(0x12663acu);driver=U(manager+0x178u);
    if(driver){
        sound=U(0xf53ffcu);
        if(sound==65u||sound==60u)sound=0x2000u;
        else sound=B(0xe38e63u)?0x2000u:0;
    }
    handle=(uint32_t)_SmackOpen_12((uintptr_t)0xf5400cu,sound,UINT32_MAX);U(0xf54114u)=handle;
    if(!handle){
        FUN_004ce2d0();FUN_004ce6e0((char *)(uintptr_t)0x5f8ffcu,0xf5400c,0);
        available=FUN_004cff20();FUN_004ce6e0((char *)(uintptr_t)0x5f8fe4u,0,(int32_t)(available+1u));return 0;
    }
    FUN_0051f780();FUN_0051f780();FUN_004d0280(1);U(0xf54054u)=1;
    (void)FUN_00538fa0(); /* Native pushes an unread extra 1; callee is nullary. */
    FUN_004ce2d0();return 1;
}
