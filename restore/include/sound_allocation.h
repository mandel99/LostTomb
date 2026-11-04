#ifndef RESTORE_SOUND_ALLOCATION_H
#define RESTORE_SOUND_ALLOCATION_H
#include "sound_initialization.h"
/* Native 55DEA2: partial allocations and a failed individual handle persist. */
int32_t restore_sound_resize_samples(RestoreSound *self,int32_t count);
#endif
