#include "monument_layout_queries.h"
#include "actor_laborer.h"
uint32_t restore_monument_dispatch_work(uint32_t x,uint32_t y,int32_t actor)
{
 if (MW_U(0x5e4ef4u)==5u) return 0;
 return (uint32_t)FUN_004efe30(x,y,actor,1);
}
uint32_t FUN_004f0190(uint32_t x,uint32_t y,int32_t actor)
{ return restore_monument_dispatch_work(x,y,actor); }
