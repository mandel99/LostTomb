#include "sound_streams.h"
#include "media_bindings.h"
void FUN_0055f639(RestoreSound *self,int channel,int32_t volume)
{
    if(channel>=0&&channel<3&&!self->disabled&&self->streams[channel])
        _AIL_set_stream_volume_8(self->streams[channel],(uint32_t)volume);
}
