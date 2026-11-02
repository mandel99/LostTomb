#include "graphics_session.h"
void FUN_004d05f0(RestoreSurfaceContext *self,int buffer,int steps,int brightness,uint8_t randomize,int effect)
{
    self->brightness=brightness;
    self->steps=FUN_004d0630(steps,0,15);
    self->randomize=randomize;
    self->effect=effect;
    FUN_004d0650(self,buffer);
}
