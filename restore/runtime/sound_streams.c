#include "sound_streams.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#ifndef RESTORE_SOUND_STREAM_PUBLISH
#define RESTORE_SOUND_STREAM_PUBLISH(base,field,value) (U((base)+(field))=(uint32_t)(value))
#endif
static uint32_t stream_slot(RestoreSound *self,uint32_t channel)
{return (uint32_t)(uintptr_t)self+0x1d8u+channel*4u;}

/* Native55F449 rejects signed channel>2 only. A negative channel retains
   the original wrapped address calculation, rather than a C array subscript. */
uint32_t restore_sound_open_stream(RestoreSound *self,const char *filename,int32_t channel,uint32_t loops)
{
    uint32_t base=(uint32_t)(uintptr_t)self,slot,volume;
    if(channel>2)return UINT32_MAX;
    if(U(base+0x200u))return 1;
    slot=stream_slot(self,(uint32_t)channel);
    if(U(slot)){
        (void)_AIL_close_stream_4(U(slot));
        U(slot)=0;
    }
    U(slot)=(uint32_t)_AIL_open_stream_12(U(base+0x178u),(uintptr_t)filename,0);
    if(U(slot)){
        volume=U(base+(channel==1?0x154u:channel==2?0x158u:0x14cu));
        (void)_AIL_set_stream_loop_count_8(U(slot),loops);
        (void)_AIL_set_stream_volume_8(U(slot),volume);
        (void)_AIL_start_stream_4(U(slot));
    }
    /* Original reports1 even when the backend failed to open a stream. */
    return 1;
}
void restore_sound_set_stream_volume(RestoreSound *self,int32_t channel,int32_t volume)
{
    uint32_t base=(uint32_t)(uintptr_t)self,slot;
    if(U(base+0x200u))return;
    slot=stream_slot(self,(uint32_t)channel);
    if(U(slot))(void)_AIL_set_stream_volume_8(U(slot),(uint32_t)volume);
}
void restore_sound_close_stream(void *controller,uint32_t channel)
{
    RestoreSound *self=(RestoreSound *)controller;uint32_t base=(uint32_t)(uintptr_t)self;
    if(!U(base+0x200u)){
        uint32_t slot=stream_slot(self,channel);
        /* Closing a null handle is also an original backend call. */
        (void)_AIL_close_stream_4(U(slot));U(slot)=0;
    }
}
uint32_t restore_sound_stream_is_playing(RestoreSound *self,int32_t channel)
{
    uint32_t base=(uint32_t)(uintptr_t)self,slot;
    if(U(base+0x200u))return 0;
    slot=stream_slot(self,(uint32_t)channel);
    if(!U(slot))return 0;
    return _AIL_stream_status_4(U(slot))==4;
}
uint32_t restore_sound_get_stream_volume(RestoreSound *self,int32_t channel)
{
    uint32_t base=(uint32_t)(uintptr_t)self,slot;
    if(U(base+0x200u))return 0;
    slot=stream_slot(self,(uint32_t)channel);
    if(!U(slot))return 0;
    return (uint32_t)_AIL_stream_volume_4(U(slot));
}
static void publish_volume(RestoreSound *self,int32_t volume,uint32_t field,uint32_t channel)
{
    uint32_t base=(uint32_t)(uintptr_t)self,slot;
    RESTORE_SOUND_STREAM_PUBLISH(base,field,(uint32_t)volume);
    if(U(base+0x200u))return;
    slot=stream_slot(self,channel);
    if(!U(slot))return;
    if(_AIL_stream_status_4(U(slot))==4)
        (void)_AIL_set_stream_volume_8(U(slot),(uint32_t)volume);
}
void restore_sound_set_music_volume(RestoreSound *self,int32_t volume)
{publish_volume(self,volume,0x14cu,0);}
void restore_sound_set_voice_volume(RestoreSound *self,int32_t volume)
{publish_volume(self,volume,0x154u,1);}
void restore_sound_set_ambient_volume(RestoreSound *self,int32_t volume)
{publish_volume(self,volume,0x158u,2);}
void restore_sound_set_sample_volume(RestoreSound *self,int32_t volume)
{U((uint32_t)(uintptr_t)self+0x150u)=(uint32_t)volume;}
/* Retained historical name:55F32F actually publishes channel1's voice volume.
   Existing figure/settings callers and their evidence keep this C alias. */
void restore_sound_music_volume_0055f32f(RestoreSound *self,int32_t volume)
{restore_sound_set_voice_volume(self,volume);}
