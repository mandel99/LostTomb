#include "paving_region_model.h"
#include "surface_passes.h"
#include "route_seed.h"
#include "map_commit_state.h"
#include "map_commit_proximity.h"
#include <stdlib.h>
extern void FUN_0040f860(const void *,void *,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];int32_t a,b,c,d;(void)site;
 if(target==0x485830u){FUN_00485830();goto done;}
 if(target==0x476530u){FUN_00476530();goto done;}
 a=(int32_t)U(sp+4u);
 if(target==0x47b8e0u){f->r[0]=FUN_0047b8e0((uint32_t)a);goto done;}
 b=(int32_t)U(sp+8u);c=(int32_t)U(sp+12u);
 if(target==0x40f860u){FUN_0040f860((const void *)(uintptr_t)(uint32_t)a,(void *)(uintptr_t)(uint32_t)b,c);goto done;}
 d=(int32_t)U(sp+16u);
 if(target==0x470900u){f->r[0]=FUN_00470900(a,b,c,d);goto done;}
 if(target==0x475720u){FUN_00475720(a,b,c,d,U(sp+20u));goto done;}
 abort();
 done:f->r[4]=sp+4u;
}
static void execute(void (*body)(struct restore_menu_input_frame *),uint32_t unused,int32_t left,int32_t top,int32_t right,int32_t bottom)
{
 uint32_t stack[64];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<64;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[33]=unused;stack[34]=(uint32_t)left;stack[35]=(uint32_t)top;stack[36]=(uint32_t)right;stack[37]=(uint32_t)bottom;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+32);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+33)||stack[32]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<14;++i)if(stack[i]!=SENTRY)abort();for(i=38;i<64;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_004754a0(uint32_t unused,int32_t left,int32_t top,int32_t right,int32_t bottom)
{execute(restore_C_004754a0,unused,left,top,right,bottom);}
void FUN_004759e0(uint32_t unused,int32_t left,int32_t top,int32_t right,int32_t bottom)
{execute(restore_C_004759e0,unused,left,top,right,bottom);}
