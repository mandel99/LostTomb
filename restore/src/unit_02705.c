#include "restore_types.h"
#include "sound_streams.h"
void FUN_0055f38d(RestoreSound *self,int32_t volume)
{
    self->volumes[0] = volume;
    if (!self->disabled && self->streams[0] &&
        _AIL_stream_status_4(self->streams[0]) == 4)
        _AIL_set_stream_volume_8(self->streams[0],volume);
}
