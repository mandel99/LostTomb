#include "sound_streams.h"
#include "media_bindings.h"
uint32_t FUN_0055f5a1(RestoreSound *self,int channel)
{
    if(channel<0||channel>=3||self->disabled||!self->streams[channel]) return 0;
    return _AIL_stream_status_4(self->streams[channel])==4;
}
