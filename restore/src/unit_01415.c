#include "actor_combat.h"
uint32_t FUN_004b1d10(int32_t direction,int32_t other)
{
 int32_t adjacent;if(direction==other)return 1;
 adjacent=(int32_t)((uint32_t)other-1u);if(adjacent<0)adjacent=7;if(direction==adjacent)return 1;
 adjacent=(int32_t)((uint32_t)other+1u);if(adjacent>=8)adjacent=0;return direction==adjacent?1u:0u;
}
