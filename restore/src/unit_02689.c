#include "sound_initialization.h"
#include <string.h>
RestoreSound *FUN_0055e38b(RestoreSound *self)
{
    static const uint32_t zero_slots[]={0x200,0x19c,0x1a0,0x1a8,0x1ac,0x1f0,0x1f4,0x1f8,0x1ec,0x1fc};
    unsigned int i;
    *(uint32_t *)0x12663b0u+=12u;
    self->files.capacity=0;self->files.count=0;self->files.entries=0;
    for(i=0;i<sizeof(zero_slots)/sizeof(zero_slots[0]);++i)memset((uint8_t *)self+zero_slots[i],0,4);
    self->rate=22050;self->bits=16;self->channels=2;
    memset(self->settings,255,sizeof(self->settings));
    for(i=0;i<4;++i)self->volumes[i]=100;
    memset(self->streams,0,sizeof(self->streams));
    memcpy(self->providers,(const void *)0x604628u,sizeof(self->providers));
    self->selected=-1;self->enabled=1;self->reserved168=0;
    {uint32_t factors[]={0x3f000000,0x3d75c28f,0x3f000000};memcpy(self->factors,factors,sizeof(factors));}
    self->provider=5;*(RestoreSound **)0x12663acu=self;return self;
}
