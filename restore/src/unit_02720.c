#include "sound_3d.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole55FF20. Channel and cache results are narrowed to signed BYTEs exactly
   where the native stack locals do so;3D positions retain raw DWORD bits. */
int32_t restore_sound_play_3d(RestoreSound *sound,const int32_t *request)
{
    volatile RestoreSound *self=sound;const volatile int32_t *r=request;
    int32_t i,loaded;int8_t channel,cache;
    if(self->disabled)return -1;
    if(r[1]<0||r[1]>(int32_t)((uint32_t)self->files.count-1u))return -1;
    for(i=0;i<self->count3d;++i){
        if(_AIL_3D_sample_status_4(U(self->handles3d+(uint32_t)i*4u))==4&&
           r[0]==(int32_t)U(self->samples3d+(uint32_t)i*40u)&&
           r[1]==(int32_t)U(self->samples3d+(uint32_t)i*40u+4u))return -1;
    }
    channel=(int8_t)FUN_0055faf7(sound);
    if(channel==-1)return -1;
    FUN_0055fbd8(sound);
    cache=(int8_t)FUN_0055fb6f(sound,request);
    if(cache==-1){
        loaded=FUN_0055fca4(sound,request,channel);
        if(loaded==-1)return -1;
        cache=(int8_t)loaded;
    }else{
        (void)_AIL_set_3D_sample_file_8(U(self->handles3d+(uint32_t)(int32_t)channel*4u),
            U(self->cache3d+(uint32_t)(int32_t)cache*12u+4u));
    }
    (void)_AIL_set_3D_sample_loop_count_8(U(self->handles3d+(uint32_t)(int32_t)channel*4u),(uint32_t)r[4]);
    (void)_AIL_set_3D_sample_volume_8(U(self->handles3d+(uint32_t)(int32_t)channel*4u),(uint32_t)r[2]);
    (void)_AIL_set_3D_position_16(U(self->handles3d+(uint32_t)(int32_t)channel*4u),(uint32_t)r[7],(uint32_t)r[8],(uint32_t)r[9]);
    U(self->samples3d+(uint32_t)(int32_t)channel*40u+24u)=(uint32_t)(int32_t)cache;
    (void)_AIL_start_3D_sample_4(U(self->handles3d+(uint32_t)(int32_t)channel*4u));
    return channel;
}
int32_t FUN_0055ff20(RestoreSound *self,const int32_t *request){return restore_sound_play_3d(self,request);}
