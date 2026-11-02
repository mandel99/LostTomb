#include "sound_initialization.h"
#include <string.h>
void FUN_0055e5d2(RestoreSound *self,int channel)
{
    static const int32_t defaults[10]={-1,-1,100,64,1,0,-1,0,0,0};
    memcpy((void *)((uintptr_t)self->samples+(uint32_t)channel*40u),defaults,sizeof(defaults));
}
