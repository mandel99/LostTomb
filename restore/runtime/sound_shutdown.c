#include "sound_initialization.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00561a2b(void *allocation);
uint32_t restore_sound_shutdown(RestoreSound *self)
{
    static const uint32_t fields[4]={0x1ac,0x1a8,0x1a0,0x19c};
    uint32_t base=(uint32_t)(uintptr_t)self,i,value;
    if(U(base+0x200u))return 1;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
        FUN_0055e5d2(self,(int32_t)i);
        (void)_AIL_release_sample_handle_4(U(U(base+0x19cu)+i*4u));
        if(U(U(base+0x1acu)+i*12u+4u)){
            value=U(U(base+0x1acu)+i*12u+4u);(void)_AIL_mem_free_lock_4(value);
            U(U(base+0x1acu)+i*12u+4u)=0;
        }
    }
    for(i=0;i<3u;++i){
        if(U(base+0x1d8u+i*4u)){
            value=U(base+0x1d8u+i*4u);(void)_AIL_close_stream_4(value);
            U(base+0x1d8u+i*4u)=0;
        }
    }
    if(U(base+0x164u)!=5u){
        for(i=0;(int32_t)i<(int32_t)U(base+0x1e4u);++i){
            (void)_AIL_release_3D_sample_handle_4(U(U(base+0x1f0u)+i*4u));
            if(U(U(base+0x1f8u)+i*12u+4u)){
                value=U(U(base+0x1f8u)+i*12u+4u);(void)_AIL_mem_free_lock_4(value);
                U(U(base+0x1f8u)+i*12u+4u)=0;
            }
        }
        (void)_AIL_close_3D_provider_4(U(base+0x1e8u));
    }
    (void)_AIL_shutdown_0();
    for(i=0;i<4u;++i)if(U(base+fields[i])){
        value=U(base+fields[i]);FUN_00561a2b((void *)(uintptr_t)value);U(base+fields[i])=0;
    }
    return 1;
}
