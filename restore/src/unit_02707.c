#include "sound_streams.h"
#include "media_bindings.h"
uint32_t FUN_0055f449(RestoreSound *self,const char *filename,int channel,uint32_t loops)
{
    uint32_t stream,volume;
    if(channel<0||channel>=3) return UINT32_MAX;
    if(self->disabled) return 1;
    if(self->streams[channel]) {
        _AIL_close_stream_4(self->streams[channel]);
        self->streams[channel]=0;
    }
    stream=(uint32_t)_AIL_open_stream_12(self->digital,(uintptr_t)filename,0);
    self->streams[channel]=stream;
    if(stream) {
        volume=(uint32_t)self->volumes[channel==1?2:channel==2?3:0];
        _AIL_set_stream_loop_count_8(stream,loops);
        _AIL_set_stream_volume_8(stream,volume);
        _AIL_start_stream_4(stream);
    }
    return 1;
}
