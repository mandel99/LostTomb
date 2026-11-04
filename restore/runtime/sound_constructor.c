#include "audio_global_lifecycle.h"
#include "sound_file_lookup.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void *FUN_00563240(void *destination,int value,size_t size);
RestoreSound *restore_sound_construct(RestoreSound *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,i;
    (void)FUN_0055dbc0(&self->files);
    U(base+0x200u)=0;U(base+0x19cu)=0;U(base+0x1a0u)=0;U(base+0x1a8u)=0;U(base+0x1acu)=0;
    U(base+0x1f0u)=0;U(base+0x1f4u)=0;U(base+0x1f8u)=0;
    U(base+0x18cu)=22050;U(base+0x190u)=16;U(base+0x194u)=2;
    (void)FUN_00563240((void *)(uintptr_t)(base+0x1b0u),-1,40);
    U(base+0x14cu)=100;U(base+0x150u)=100;U(base+0x154u)=100;U(base+0x158u)=100;
    U(base+0x1ecu)=0;U(base+0x1fcu)=0;
    for(i=0;i<3u;++i)U(base+0x1d8u+i*4u)=0;
    for(i=0;i<26u;++i){
        U(base+12u+i*12u)=U(0x604628u+i*12u);
        U(base+16u+i*12u)=U(0x60462cu+i*12u);
        U(base+20u+i*12u)=U(0x604630u+i*12u);
    }
    U(base+0x148u)=UINT32_MAX;U(base+0x144u)=1;U(base+0x168u)=0;
    U(base+0x16cu)=0x3f000000;U(base+0x170u)=0x3d75c28f;U(base+0x174u)=0x3f000000;
    U(base+0x164u)=5;U(0x12663acu)=base;return self;
}
