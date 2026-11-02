#include "connection_plan_model.h"
#include "route_walk_model.h"
#include "route_footprint.h"
#include "route_seed.h"
#include "terrain_redraw.h"
#include "footprint.h"
#include "map_commit_state.h"
#include <stdlib.h>
extern void FUN_0040f860(const void *,void *,int32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4],a,b,c,d;(void)site;
 if(target==0x485830u){FUN_00485830();goto done;}
 a=U(sp+4u);
 if(target==0x47f190u){f->r[0]=FUN_0047f190(a);goto done;}
 b=U(sp+8u);c=U(sp+12u);
 if(target==0x40f860u){FUN_0040f860((const void *)(uintptr_t)a,(void *)(uintptr_t)b,(int32_t)c);goto done;}
 d=U(sp+16u);
 switch(target){
 case 0x51b680u:f->r[0]=FUN_0051b680(a,(int32_t)b,(int32_t)c,(int32_t)d);break;
 case 0x47ca50u:FUN_0047ca50(a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20u));break;
 case 0x47d360u:FUN_0047d360((int32_t)a,(int32_t)b,(int32_t)c,d,(const void *)(uintptr_t)U(sp+20u),(uint8_t)U(sp+24u));break;
 case 0x51c930u:f->r[0]=FUN_0051c930(a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20u),(int32_t)U(sp+24u));break;
 case 0x47c880u:f->r[0]=(uint32_t)FUN_0047c880((int32_t)a,(int32_t)b,(int32_t)c,(int32_t)d,(int32_t)U(sp+20u),(int32_t)U(sp+24u),(int32_t)U(sp+28u));break;
 default:abort();
 }
 done:f->r[4]=sp+4u;
}
uint32_t FUN_0047eda0(uint32_t preview,int32_t x1,int32_t y1,int32_t x2,int32_t y2)
{
 uint32_t stack[64];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<64;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[33]=preview;stack[34]=(uint32_t)x1;stack[35]=(uint32_t)y1;stack[36]=(uint32_t)x2;stack[37]=(uint32_t)y2;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+32);restore_C_0047eda0(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+33)||stack[32]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<10;++i)if(stack[i]!=SENTRY)abort();for(i=38;i<64;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
