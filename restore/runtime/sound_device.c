#include "sound_device.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
int32_t restore_sound_initialize_digital(RestoreSound *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,platform,major,i,value;
    int32_t bytes;
    if(!restore_sound_version(&platform,&major))return 0;
    if(platform==2u&&major<5u){U(base+0x200u)=1;return 0;}
    (void)_AIL_startup_0();
    (void)_AIL_set_preference_8(40,0x40000);
    (void)_AIL_set_preference_8(15,0);
    W(base+0x17cu)=1;W(base+0x17eu)=W(base+0x194u);
    U(base+0x180u)=U(base+0x18cu);
    bytes=(int32_t)U(base+0x190u)/8;
    value=U(base+0x18cu)*(uint32_t)bytes;value*=U(base+0x194u);U(base+0x184u)=value;
    bytes=(int32_t)U(base+0x190u)/8;
    value=(uint32_t)bytes*U(base+0x194u);W(base+0x188u)=(uint16_t)value;
    W(base+0x18au)=W(base+0x190u);
    if(_AIL_waveOutOpen_16(base+0x178u,0,UINT32_MAX,base+0x17cu)!=0){
        restore_sound_debug((const char *)(uintptr_t)0x604760u);return 0;
    }
    if(!FUN_0055dea2(self,5))return 0;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i)FUN_0055e5d2(self,(int32_t)i);
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
        U(U(base+0x1acu)+i*12u)=UINT32_MAX;
        U(U(base+0x1acu)+i*12u+4u)=0;
        U(U(base+0x1acu)+i*12u+8u)=0;
    }
    return 1;
}
