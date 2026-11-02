#include "flood_simulation.h"
void FUN_004be0d0(RestoreWorldRegion *self)
{
    if ((int32_t)self->words[1]<5) {
        self->words[1]++;FUN_004bd850(self,0,1,1);
    }
}
