#include "sound_allocation.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(void *allocation);
int32_t restore_sound_resize_samples(RestoreSound *self,int32_t count)
{
    static const uint32_t offsets[4]={0x19c,0x1a0,0x1a8,0x1ac};
    static const uint32_t strides[4]={4,4,40,12};
    uint32_t base=(uint32_t)(uintptr_t)self,i,value;
    if(count<1)return 0;
    if(U(base+0x19cu)){
        FUN_0055ec7c(self);
        for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
            FUN_0055e5d2(self,(int32_t)i);
            (void)_AIL_release_sample_handle_4(U(U(base+0x19cu)+i*4u));
            if(U(U(base+0x1acu)+i*12u+4u)){
                value=U(U(base+0x1acu)+i*12u+4u);
                (void)_AIL_mem_free_lock_4(value);
                U(U(base+0x1acu)+i*12u+4u)=0;
            }
        }
    }
    for(i=0;i<4u;++i){
        if(U(base+offsets[i])){
            value=U(base+offsets[i]);FUN_00561a2b((void *)(uintptr_t)value);
            U(base+offsets[i])=0;
        }
    }
    for(i=0;i<4u;++i){
        value=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*strides[i]);
        U(base+offsets[i])=value;
        /* Native rereads the published field; no rollback or count reset. */
        if(!U(base+offsets[i]))return 0;
    }
    U(base+0x198u)=(uint32_t)count;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
        value=(uint32_t)_AIL_allocate_sample_handle_4(U(base+0x178u));
        U(U(base+0x19cu)+i*4u)=value;
        if(!U(U(base+0x19cu)+i*4u))break;
    }
    return 1;
}
