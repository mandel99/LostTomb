#include "sound_initialization.h"
#include "media_bindings.h"
void FUN_0055ec7c(RestoreSound *self)
{
    int i;
    if(self->disabled)return;
    for(i=0;i<self->sample_count;++i)_AIL_end_sample_4(((uint32_t *)(uintptr_t)self->handles)[i]);
    for(i=0;i<self->count3d;++i)_AIL_end_3D_sample_4(((uint32_t *)(uintptr_t)self->handles3d)[i]);
}
