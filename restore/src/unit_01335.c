#include "actor_callback_496140.h"
#define U(a) (*(uint32_t *)(uintptr_t)(a))
#define B(a) (*(uint8_t *)(uintptr_t)(a))
void FUN_00496060(int32_t id,uint32_t building,uint32_t target)
{
    int32_t slot;uint8_t primary=0;uint32_t a;
    for(slot=0;slot<4;++slot){uint32_t kind=U(0xbcd694u+U(0xd056f8)*388u+(uint32_t)slot*4u);
        if(kind<=7u){FUN_00495cf0(id,building,target,slot);primary=1;}
        else if(kind<=11u)FUN_00496010((uint32_t)id,building,target,slot);
    }
    a=0xbcd5f8u+(uint32_t)id*388u;U(a+172u+(uint32_t)B(a+147u)*4u)=target;B(a+147u)=(uint8_t)(B(a+147u)+1u);
    if(primary)FUN_00495f00(id,(int32_t)building,target);else FUN_00495e10(id,(int32_t)building,target);
}
