#include "actor_combat.h"
void FUN_004b1da0(int32_t id)
{
 uint32_t a=cb_actor((uint32_t)id);CB_B(a+0x83u)=0;CB_B(a+0x30u)=CB_B(a+8u);CB_W(a+0x88u)=CB_W(a+0x84u)=CB_W(a+0x86u)=0;FUN_004b3250(id);
}
