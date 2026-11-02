#include "sound_streams.h"
void FUN_0055f561(RestoreSound *self,int channel)
{
    if(channel>=0&&channel<3) restore_sound_close_stream(self,(uint32_t)channel);
}
