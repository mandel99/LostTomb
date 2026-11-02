#include "sprite_auxiliary.h"
#include "sprite_shutdown.h"
/* Whole55DB90. Reload owner/index after releasing the record's allocation. */
void restore_sprite_auxiliary_destroy_record(RestoreSpriteAuxiliaryRecord *object)
{
    volatile RestoreSpriteAuxiliaryRecord *self=object;
    uint32_t allocation=self->allocation;
    if(allocation){
        restore_sprite_shutdown_free((void *)(uintptr_t)allocation);
        self->allocation=0;
    }
    if(self->owner&&self->index>=0)
        (void)FUN_0055d990((RestoreSpriteAuxiliary *)(uintptr_t)self->owner,self->index);
}
void FUN_0055db90(RestoreSpriteAuxiliaryRecord *self){restore_sprite_auxiliary_destroy_record(self);}
