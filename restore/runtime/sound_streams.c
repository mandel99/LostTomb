#include "sound_streams.h"
#include "media_bindings.h"
/* Whole55F32F/30 instructions, native ECX+self +one DWORD/RET4.
   Preserve the volume publication and the fresh stream reload after status.
   The normalized global and settings aliases share this implementation. */
void restore_sound_music_volume_0055f32f(RestoreSound *self,int32_t volume)
{
    uint32_t base=(uint32_t)(uintptr_t)self;
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
    U(base+0x154u)=(uint32_t)volume;
    if(U(base+0x200u)==0u&&U(base+0x1dcu)!=0u){
        uint32_t stream=U(base+0x1dcu);
        if(_AIL_stream_status_4(stream)==4)
            (void)_AIL_set_stream_volume_8(U(base+0x1dcu),(uint32_t)volume);
    }
#undef U
}
/* Native 0055f561: object in ECX, one channel argument, ret 4. The C
   interface makes the receiver explicit and preserves null-handle calls. */
void restore_sound_close_stream(void *controller,uint32_t channel)
{
    unsigned char *sound=(unsigned char *)controller;
    uintptr_t *stream=(uintptr_t *)(sound+0x1d8+channel*4u);
    if(*(uint32_t *)(sound+0x200)==0) {
        _AIL_close_stream_4(*stream);
        *stream=0;
    }
}
