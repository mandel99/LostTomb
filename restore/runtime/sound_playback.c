#include "sound_playback.h"
#include "media_bindings.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
/* Native offsets: channel count198, handles19C, records1A8, cache1AC.
   Cache rows are {file ID,data pointer,active references}, 12 bytes each. */
static uint32_t sample_handle(uint32_t base,int32_t channel)
{return U(U(base+0x19cu)+(uint32_t)channel*4u);}

void restore_sound_refresh_cache_references(RestoreSound *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,i,index,cache,value;
    if(U(base+0x200u))return;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i)U(U(base+0x1acu)+i*12u+8u)=0;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
        if(_AIL_sample_status_4(sample_handle(base,(int32_t)i))!=4)continue;
        index=U(U(base+0x1a8u)+i*40u+24u);cache=U(base+0x1acu);
        value=U(cache+index*12u+8u)+1u;
        index=U(U(base+0x1a8u)+i*40u+24u);cache=U(base+0x1acu);
        U(cache+index*12u+8u)=value;
    }
}
int32_t restore_sound_find_available_channel(RestoreSound *self)
{
    uint32_t base=(uint32_t)(uintptr_t)self,i;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i)
        if(_AIL_sample_status_4(sample_handle(base,(int32_t)i))!=4)return (int32_t)i;
    return -1;
}
int32_t restore_sound_find_cached_sample(RestoreSound *self,const RestoreSoundRequest *request)
{
    uint32_t base=(uint32_t)(uintptr_t)self,req=(uint32_t)(uintptr_t)request,i,cache,file;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
        cache=U(base+0x1acu);file=U(cache+i*12u);
        if(file==U(req+4u)&&U(U(base+0x1acu)+i*12u+4u))return (int32_t)i;
    }
    return -1;
}
int32_t restore_sound_load_sample_cache(RestoreSound *self,const RestoreSoundRequest *request,int32_t channel)
{
    uint32_t base=(uint32_t)(uintptr_t)self,req=(uint32_t)(uintptr_t)request;
    uint32_t file,entries,name,i,slot=UINT32_MAX,cache,destination,value,data,handle;
    if(U(base+0x200u))return 1;
    file=U(req+4u);entries=U(base+8u);name=U(entries+file*8u);
    if(!name)return -1;
    file=U(req+4u);entries=U(base+8u);name=U(entries+file*8u);
    if(!B(name))return -1;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
        if(!U(U(base+0x1acu)+i*12u+8u)){slot=i;break;}
    }
    if(slot==UINT32_MAX)return -1;
    if(U(U(base+0x1acu)+slot*12u+4u)){
        data=U(U(base+0x1acu)+slot*12u+4u);
        (void)_AIL_mem_free_lock_4(data);
        FUN_0055e5d2(self,channel);
    }
    for(i=0;i<5u;++i){
        destination=U(base+0x1a8u);value=U(req+i*4u);
        U(destination+(uint32_t)channel*40u+i*4u)=value;
    }
    file=U(req+4u);entries=U(base+8u);name=U(entries+file*8u);
    data=(uint32_t)_AIL_file_read_8(name,0);
    U(U(base+0x1acu)+slot*12u+4u)=data;
    if(!U(U(base+0x1acu)+slot*12u+4u))return -1;
    cache=U(base+0x1acu);file=U(req+4u);U(cache+slot*12u)=file;
    (void)_AIL_init_sample_4(sample_handle(base,channel));
    data=U(U(base+0x1acu)+slot*12u+4u);handle=sample_handle(base,channel);
    (void)_AIL_set_sample_file_12(handle,data,0);
    return (int32_t)slot;
}
int32_t restore_sound_play_sample(RestoreSound *self,const RestoreSoundRequest *request)
{
    uint32_t base=(uint32_t)(uintptr_t)self,req=(uint32_t)(uintptr_t)request;
    uint32_t i,limit,samples,value,data,handle,reverb0,reverb1,reverb2;
    int32_t channel,cache_index,result;
    if(U(base+0x200u))return -1;
    if((int32_t)U(req+4u)<0)return -1;
    limit=U(base+4u)-1u;
    if((int32_t)U(req+4u)>(int32_t)limit)return -1;
    for(i=0;(int32_t)i<(int32_t)U(base+0x198u);++i){
        if(_AIL_sample_status_4(sample_handle(base,(int32_t)i))!=4)continue;
        samples=U(base+0x1a8u);value=U(req);
        if(value!=U(samples+i*40u))continue;
        samples=U(base+0x1a8u);value=U(req+4u);
        if(value==U(samples+i*40u+4u))return -1;
    }
    /* The original stores AL, then MOVSX: keep both signed-byte conversions. */
    channel=(int32_t)(int8_t)(uint8_t)FUN_0055f02d((uintptr_t)self);
    if(channel==-1)return -1;
    FUN_0055ecf3((uintptr_t)self);
    cache_index=(int32_t)(int8_t)(uint8_t)FUN_0055f07f((uintptr_t)self,(const int32_t *)request);
    if(cache_index==-1){
        result=FUN_0055edd0((uintptr_t)self,(const int32_t *)request,channel);
        if(result==-1)return -1;
        cache_index=(int32_t)(int8_t)(uint8_t)result;
    }else{
        data=U(U(base+0x1acu)+(uint32_t)cache_index*12u+4u);handle=sample_handle(base,channel);
        (void)_AIL_set_sample_file_12(handle,data,0);
    }
    value=U(req+16u);handle=sample_handle(base,channel);(void)_AIL_set_sample_loop_count_8(handle,value);
    value=U(req+12u);handle=sample_handle(base,channel);(void)_AIL_set_sample_pan_8(handle,value);
    value=U(req+8u);handle=sample_handle(base,channel);(void)_AIL_set_sample_volume_8(handle,value);
    U(U(base+0x1a8u)+(uint32_t)channel*40u+24u)=(uint32_t)cache_index;
    if(U(base+0x168u)){
        reverb2=U(base+0x174u);reverb1=U(base+0x170u);reverb0=U(base+0x16cu);
        handle=sample_handle(base,channel);(void)_AIL_set_sample_reverb_16(handle,reverb0,reverb1,reverb2);
    }else (void)_AIL_set_sample_reverb_16(sample_handle(base,channel),0,0,0);
    (void)_AIL_start_sample_4(sample_handle(base,channel));
    return channel;
}
