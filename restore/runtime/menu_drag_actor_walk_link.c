#include "actor_walking.h"
#include "game_diagnostics.h"
#include <intrin.h>
void FUN_004b52f0(int32_t actor)
{
    uint32_t offset=(uint32_t)actor*388u;
    int32_t tile=AW_I(0xbcd618u+offset),tail,next;
    restore_game_actor_grid_event((uint32_t)(uintptr_t)_ReturnAddress(),actor,1);
    if(tile>=0){
        tail=AW_S(0xb350dcu+(uint32_t)tile*2u);
        AW_B(0xbcd682u+offset)=0;
        if(tail==0){
            AW_W(0xb350dcu+(uint32_t)AW_I(0xbcd618u+offset)*2u)=(uint16_t)actor;
            restore_actor_walking_empty(actor);return;
        }
        AW_B(0xbcd682u+offset)=1;
        next=AW_S(0xbcd600u+(uint32_t)tail*388u);
        while(next!=0){
            tail=next;
            if(tail==actor)goto complete;
            AW_B(0xbcd682u+offset)=(uint8_t)(AW_B(0xbcd682u+offset)+1u);
            next=AW_S(0xbcd600u+(uint32_t)tail*388u);
        }
        if(AW_B(0xbcd682u+offset)>20)AW_B(0xbcd682u+offset)=20;
        AW_W(0xbcd600u+(uint32_t)tail*388u)=(uint16_t)actor;
    }
complete:
    restore_actor_walking_empty(actor);
}
