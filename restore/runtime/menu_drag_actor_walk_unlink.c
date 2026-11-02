#include "actor_walking.h"
#include "game_diagnostics.h"
#include <intrin.h>
void FUN_004b5420(int32_t actor)
{
    uint32_t offset=(uint32_t)actor*388u;
    int32_t tile=AW_I(0xbcd618u+offset),previous,next;
    uint16_t successor;
    restore_game_actor_grid_event((uint32_t)(uintptr_t)_ReturnAddress(),actor,0);
    if(tile<0){restore_actor_walking_empty(actor);return;}
    previous=AW_S(0xb350dcu+(uint32_t)tile*2u);
    if(previous==0){restore_actor_walking_empty(actor);return;}
    if(previous==actor){
        successor=AW_W(0xbcd600u+offset);
        AW_W(0xb350dcu+(uint32_t)tile*2u)=successor;
        AW_W(0xbcd600u+offset)=0;
        restore_actor_walking_empty(actor);return;
    }
    next=AW_S(0xbcd600u+(uint32_t)previous*388u);
    while(next!=actor){
        if(previous==0)return;
        previous=next;
        next=AW_S(0xbcd600u+(uint32_t)previous*388u);
    }
    successor=AW_W(0xbcd600u+offset);
    AW_W(0xbcd600u+(uint32_t)previous*388u)=successor;
    AW_W(0xbcd600u+offset)=0;
    restore_actor_walking_empty(actor);restore_actor_walking_empty(previous);
}
