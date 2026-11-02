#include "sound_3d.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Whole55FCA4. Cache slot and output channel are distinct. Preserve request
   reloads and the seven ordered field copies, including overlapping input. */
int32_t restore_sound_load_3d_file(RestoreSound *sound,const int32_t *request,int32_t channel)
{
    static const uint8_t fields[7]={0,1,2,4,7,8,9};
    volatile RestoreSound *self=sound;const volatile int32_t *r=request;
    uint32_t name=U(self->files.entries+(uint32_t)r[1]*8u),field,data;
    int32_t slot;
    if(!name||!B(name)||self->disabled)return -1;
    for(slot=0;slot<self->count3d;++slot)
        if(!U(self->cache3d+(uint32_t)slot*12u+8u))break;
    if(slot>=self->count3d)return -1;
    if(U(self->cache3d+(uint32_t)slot*12u+4u)){
        (void)_AIL_mem_free_lock_4(U(self->cache3d+(uint32_t)slot*12u+4u));
        FUN_0055fa1c(sound,channel);
    }
    for(field=0;field<7u;++field){
        uint32_t offset=(uint32_t)fields[field]*4u;
        U(self->samples3d+(uint32_t)channel*40u+offset)=(uint32_t)r[fields[field]];
    }
    data=(uint32_t)_AIL_file_read_8(U(self->files.entries+(uint32_t)r[1]*8u),0);
    U(self->cache3d+(uint32_t)slot*12u+4u)=data;
    if(!U(self->cache3d+(uint32_t)slot*12u+4u))return -1;
    U(self->cache3d+(uint32_t)slot*12u)=(uint32_t)r[1];
    (void)_AIL_set_3D_sample_file_8(U(self->handles3d+(uint32_t)channel*4u),U(self->cache3d+(uint32_t)slot*12u+4u));
    return slot;
}
int32_t FUN_0055fca4(RestoreSound *self,const int32_t *request,int32_t channel){return restore_sound_load_3d_file(self,request,channel);}
