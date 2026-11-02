#include "sprite_auxiliary.h"
#include "sprite_shutdown.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))

/* Whole55D8B0: hold each old allocation across callbacks, but reload the
   next owner field. Vector entries are destroyed backwards in24-byte steps. */
void restore_sprite_auxiliary_destroy(RestoreSpriteAuxiliary *object)
{
    volatile RestoreSpriteAuxiliary *self=object;
    uint32_t saved,cursor,count;
    if(self->child){
        restore_sprite_shutdown_delete((void *)(uintptr_t)self->child,3);
        self->child=0;
    }
    saved=self->first_list;
    if(saved){
        restore_sprite_shutdown_free((void *)(uintptr_t)U(saved+8u));
        restore_sprite_shutdown_free((void *)(uintptr_t)saved);
        self->first_list=0;
    }
    saved=self->records;
    if(saved){
        count=U(saved-4u);cursor=saved+count*24u;
        for(;;){
            --count;
            if((int32_t)count<0)break;
            cursor-=24u;
            FUN_0055db90((RestoreSpriteAuxiliaryRecord *)(uintptr_t)cursor);
        }
        restore_sprite_shutdown_free((void *)(uintptr_t)(saved-4u));
        self->records=0;
    }
    saved=self->free_list;
    if(saved){
        restore_sprite_shutdown_free((void *)(uintptr_t)U(saved+8u));
        restore_sprite_shutdown_free((void *)(uintptr_t)saved);
        self->free_list=0;
    }
    saved=U(0x12652f8u);
    if(saved){
        restore_sprite_shutdown_free((void *)(uintptr_t)saved);
        U(0x12652f8u)=0;U(0x12652f4u)=0;
    }
}
void FUN_0055d8b0(RestoreSpriteAuxiliary *self){restore_sprite_auxiliary_destroy(self);}
