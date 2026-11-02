#include "sound_streams.h"
#include "media_bindings.h"
uint32_t FUN_0055f5f4(RestoreSound *self,int channel)
{
    if(channel<0||channel>=3||self->disabled||!self->streams[channel]) return 0;
    return (uint32_t)_AIL_stream_volume_4(self->streams[channel]);
}
