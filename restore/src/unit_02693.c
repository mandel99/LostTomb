#include "sound_initialization.h"
#include "media_bindings.h"
extern void FUN_00561a2b(void *allocation);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole55E92B. Keep native release order and field reloads, including writes
   after the callbacks. Only the four2D allocation fields are freed here. */
uint32_t restore_sound_shutdown(RestoreSound *sound)
{
    volatile RestoreSound *self=sound;
    int32_t i;
    if(self->disabled)return 1;
    for(i=0;i<self->sample_count;++i){
        FUN_0055e5d2(sound,i);
        (void)_AIL_release_sample_handle_4(U(self->handles+(uint32_t)i*4u));
        if(U(self->cache+(uint32_t)i*12u+4u)){
            (void)_AIL_mem_free_lock_4(U(self->cache+(uint32_t)i*12u+4u));
            U(self->cache+(uint32_t)i*12u+4u)=0;
        }
    }
    for(i=0;i<3;++i){
        if(self->streams[i]){
            (void)_AIL_close_stream_4(self->streams[i]);
            self->streams[i]=0;
        }
    }
    if(self->provider!=5){
        for(i=0;i<self->count3d;++i){
            (void)_AIL_release_3D_sample_handle_4(U(self->handles3d+(uint32_t)i*4u));
            if(U(self->cache3d+(uint32_t)i*12u+4u)){
                (void)_AIL_mem_free_lock_4(U(self->cache3d+(uint32_t)i*12u+4u));
                U(self->cache3d+(uint32_t)i*12u+4u)=0;
            }
        }
        (void)_AIL_close_3D_provider_4(self->device3d);
    }
    (void)_AIL_shutdown_0();
    if(self->cache){FUN_00561a2b((void *)(uintptr_t)self->cache);self->cache=0;}
    if(self->samples){FUN_00561a2b((void *)(uintptr_t)self->samples);self->samples=0;}
    if(self->auxiliary){FUN_00561a2b((void *)(uintptr_t)self->auxiliary);self->auxiliary=0;}
    if(self->handles){FUN_00561a2b((void *)(uintptr_t)self->handles);self->handles=0;}
    return 1;
}
uint32_t FUN_0055e92b(RestoreSound *self){return restore_sound_shutdown(self);}
