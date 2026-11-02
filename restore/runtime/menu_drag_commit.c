#include "drag_commit_model.h"
#include "building_release_model.h"
#include "actor_spawn.h"
#include "drag_leaves.h"
#include "terrain_redraw.h"
#include "editor_validation.h"
#include "clear_footprint.h"
#include "refresh_dispatch.h"
#include "refresh_wrappers.h"
#include "flood_edges.h"
#include "growth_update.h"
#include <stdlib.h>
extern void FUN_0051f780(void);
extern uint8_t FUN_00477f60(void);
extern void FUN_00488e90(int32_t,int32_t,int32_t,int32_t,char);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];int32_t a,b,c,d;(void)site;
 if(target==0x51f780u){FUN_0051f780();goto done;}
 if(target==0x477f60u){f->r[0]=(f->r[0]&UINT32_C(0xffffff00))|FUN_00477f60();goto done;}
 a=(int32_t)U(sp+4u);
 if(target==0x46ac10u){FUN_0046ac10(a);goto done;}
 if(target==0x47ed60u){f->r[0]=FUN_0047ed60((uint32_t)a);goto done;}
 b=(int32_t)U(sp+8u);
 switch(target){
 case 0x4ec410u:f->r[0]=FUN_004ec410((uint32_t)a,(char)b);goto done;
 case 0x477d40u:f->r[0]=FUN_00477d40(a,b);goto done;
 case 0x477f70u:f->r[0]=FUN_00477f70(a,b);goto done;
 case 0x478090u:f->r[0]=FUN_00478090(a,b);goto done;
 case 0x478270u:f->r[0]=FUN_00478270(a,b);goto done;
 case 0x477960u:FUN_00477960(a,b);goto done;
 case 0x4779f0u:FUN_004779f0(a,b);goto done;
 case 0x477ab0u:FUN_00477ab0(a,b);goto done;
 case 0x477b10u:FUN_00477b10(a,b);goto done;
 case 0x477b70u:FUN_00477b70(a,b);goto done;
 case 0x477bd0u:FUN_00477bd0(a,b);goto done;
 case 0x477c20u:FUN_00477c20(a,b);goto done;
 case 0x477c80u:FUN_00477c80(a,b);goto done;
 case 0x477ce0u:FUN_00477ce0(a,b);goto done;
 case 0x4793e0u:FUN_004793e0(a,b);goto done;
 case 0x479460u:FUN_00479460(a,b);goto done;
 default:break;
 }
 c=(int32_t)U(sp+12u);
 switch(target){
 case 0x46f9a0u:f->r[0]=FUN_0046f9a0(a,b,c);goto done;
 case 0x4bbd40u:FUN_004bbd40(a,b,c);goto done;
 case 0x47c480u:FUN_0047c480(a,b,c);goto done;
 case 0x477a40u:FUN_00477a40(a,b,(uint32_t)c);goto done;
 case 0x477dd0u:FUN_00477dd0(a,b,c);goto done;
 default:break;
 }
 d=(int32_t)U(sp+16u);
 switch(target){
 case 0x487450u:FUN_00487450(a,b,c,d);break;
 case 0x478d40u:FUN_00478d40(a,b,c,d);break;
 case 0x478e10u:FUN_00478e10(a,b,c,d);break;
 case 0x478ea0u:FUN_00478ea0(a,b,c,d);break;
 case 0x478f40u:FUN_00478f40(a,b,c,d);break;
 case 0x478fd0u:FUN_00478fd0(a,b,c,d);break;
 case 0x479060u:FUN_00479060(a,b,c,d);break;
 case 0x4790e0u:FUN_004790e0(a,b,c,d);break;
 case 0x479170u:FUN_00479170(a,b,c,d);break;
 case 0x479200u:FUN_00479200(a,b,c,d);break;
 case 0x4792b0u:FUN_004792b0(a,b,c,d);break;
 case 0x479340u:FUN_00479340(a,b,c,d);break;
 case 0x479510u:FUN_00479510(a,b,c,d);break;
 case 0x488e90u:FUN_00488e90(a,b,c,d,(char)U(sp+20u));f->r[4]=sp+24u;return;
 default:abort();
 }
 done:f->r[4]=sp+4u;
}
void FUN_00478440(int32_t x,int32_t y)
{
 uint32_t stack[64];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<64;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+32);stack[33]=(uint32_t)x;stack[34]=(uint32_t)y;
 restore_C_00478440(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+33)||stack[32]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<17;++i)if(stack[i]!=SENTRY)abort();for(i=35;i<64;++i)if(stack[i]!=SENTRY)abort();
}
