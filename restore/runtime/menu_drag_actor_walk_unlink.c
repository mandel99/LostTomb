#include "actor_walking.h"
#include "game_diagnostics.h"
#include <intrin.h>
static uint32_t actor_unlink_grid_core(int32_t actor,uint32_t caller_site)
{
    uint32_t offset=(uint32_t)actor*388u;
    int32_t tile=AW_I(0xbcd618u+offset),previous,next;
    uint16_t successor;
    restore_game_actor_grid_event(caller_site,actor,0);
    if(tile<0){restore_actor_walking_empty(actor);return offset;}
    previous=AW_S(0xb350dcu+(uint32_t)tile*2u);
    if(previous==0){restore_actor_walking_empty(actor);return offset;}
    if(previous==actor){
        successor=AW_W(0xbcd600u+offset);
        AW_W(0xb350dcu+(uint32_t)tile*2u)=successor;
        AW_W(0xbcd600u+offset)=0;
        restore_actor_walking_empty(actor);return offset;
    }
    next=AW_S(0xbcd600u+(uint32_t)previous*388u);
    while(next!=actor){
        if(previous==0)return offset;
        previous=next;
        next=AW_S(0xbcd600u+(uint32_t)previous*388u);
    }
    successor=AW_W(0xbcd600u+offset);
    AW_W(0xbcd600u+(uint32_t)previous*388u)=successor;
    AW_W(0xbcd600u+offset)=0;
    restore_actor_walking_empty(actor);restore_actor_walking_empty(previous);
    return offset;
}

__declspec(noinline) uint32_t restore_actor_unlink_from_grid(int32_t actor)
{
    return actor_unlink_grid_core(actor,(uint32_t)(uintptr_t)_ReturnAddress());
}

__declspec(noinline) void FUN_004b5420(int32_t actor)
{
    (void)actor_unlink_grid_core(actor,(uint32_t)(uintptr_t)_ReturnAddress());
}
