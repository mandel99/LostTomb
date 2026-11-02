#include "sprite_auxiliary.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
/* Whole55D990: push a record pointer onto the owner's descending free stack. */
uint32_t restore_sprite_auxiliary_return_record(RestoreSpriteAuxiliary *object,int32_t index)
{
    volatile RestoreSpriteAuxiliary *self=object;
    if(index>=0){
        uint32_t record=self->records+(uint32_t)index*24u;
        uint32_t list=self->free_list;
        uint32_t cursor=U(list)-4u;
        U(list)=cursor;U(cursor)=record;
    }
    return 0;
}
uint32_t FUN_0055d990(RestoreSpriteAuxiliary *self,int32_t index)
{return restore_sprite_auxiliary_return_record(self,index);}
