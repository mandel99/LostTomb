#include "sound_initialization.h"
#include "media_bindings.h"
extern void *FUN_00561a1d(uint32_t bytes);
extern void FUN_00561a2b(void *allocation);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole55E14F. Native ECX is explicit, RET4 becomes ordinary C arguments.
   Every callback-sensitive pointer/count is reloaded. The original succeeds
   after a failed individual sample handle; only array-allocation failure
   returns0. No invented zeroing or rollback is added. */
int restore_sound_resize_3d(RestoreSound *sound,int32_t count)
{
    volatile RestoreSound *self=sound;
    int32_t i;
    if(count<1)return 0;
    if(self->handles3d){
        FUN_0055ec7c(sound);
        for(i=0;i<self->count3d;++i){
            (void)_AIL_release_3D_sample_handle_4(U(self->handles3d+(uint32_t)i*4u));
            if(U(self->cache3d+(uint32_t)i*12u+4u)){
                (void)_AIL_mem_free_lock_4(U(self->cache3d+(uint32_t)i*12u+4u));
                U(self->cache3d+(uint32_t)i*12u+4u)=0;
            }
        }
    }
    if(self->handles3d){FUN_00561a2b((void *)(uintptr_t)self->handles3d);self->handles3d=0;}
    if(self->samples3d){FUN_00561a2b((void *)(uintptr_t)self->samples3d);self->samples3d=0;}
    if(self->cache3d){FUN_00561a2b((void *)(uintptr_t)self->cache3d);self->cache3d=0;}
    self->handles3d=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*4u);
    if(!self->handles3d)return 0;
    self->samples3d=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*40u);
    if(!self->samples3d)return 0;
    self->cache3d=(uint32_t)(uintptr_t)FUN_00561a1d((uint32_t)count*12u);
    if(!self->cache3d)return 0;
    self->count3d=count;
    for(i=0;i<self->count3d;++i){
        uint32_t sample=(uint32_t)_AIL_allocate_3D_sample_handle_4(self->device3d);
        U(self->handles3d+(uint32_t)i*4u)=sample;
        if(!U(self->handles3d+(uint32_t)i*4u))break;
    }
    return 1;
}
int FUN_0055e14f(RestoreSound *self,int32_t count)
{return restore_sound_resize_3d(self,count);}
