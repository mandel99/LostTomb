#include "sound_initialization.h"
#include "media_bindings.h"
#define s_Unable_to_initialize_digital_sou_00604760 (*(char (*)[47])0x00604760u)
int FUN_0055e6c4(RestoreSound *self)
{
    uint32_t platform,major;int i,bytes;
    if(!restore_sound_version(&platform,&major))return 0;
    if(platform==2&&major<5) {self->disabled=1;return 0;}
    _AIL_startup_0();_AIL_set_preference_8(40,0x40000);_AIL_set_preference_8(15,0);
    bytes=self->bits/8;
    self->format.tag=1;self->format.channels=(uint16_t)self->channels;self->format.rate=(uint32_t)self->rate;
    self->format.bytes_per_second=(uint32_t)self->rate*(uint32_t)bytes*(uint32_t)self->channels;
    self->format.block=(uint16_t)((uint32_t)bytes*(uint32_t)self->channels);self->format.bits=(uint16_t)self->bits;
    if(_AIL_waveOutOpen_16((uintptr_t)&self->digital,0,0xffffffffu,(uintptr_t)&self->format)!=0) {
        restore_sound_debug(s_Unable_to_initialize_digital_sou_00604760);return 0;
    }
    if(!FUN_0055dea2(self,5))return 0;
    for(i=0;i<self->sample_count;++i)FUN_0055e5d2(self,i);
    for(i=0;i<self->sample_count;++i) {
        RestoreSoundCache *cache=(RestoreSoundCache *)(uintptr_t)self->cache;
        cache[i].file=-1;cache[i].data=0;cache[i].state=0;
    }
    return 1;
}
