#include "actor_command.h"
uint32_t FUN_004ec0f0(int32_t x,int32_t y,volatile int32_t *out_x,
 volatile int32_t *out_y,int32_t rings,uint32_t actor,restore_actor_tile_callback callback)
{
 if((uintptr_t)callback==UINT32_C(0x4eae20))callback=restore_actor_find_other_kind_on_tile;
 return restore_actor_spiral_search(x,y,out_x,out_y,rings,actor,callback);
}
