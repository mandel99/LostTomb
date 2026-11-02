#include "redraw_dispatch_model.h"
#include "surface_passes.h"
#include "map_gates.h"
#include "canal_region_model.h"
#include "road_pack.h"
#include "road_refresh.h"
#include "ground.h"
#include "terrain_redraw.h"
#include "water_refresh.h"
#include "clear_helpers.h"
#include <stdlib.h>
extern void FUN_0051f780(void),FUN_00477210(void),FUN_0047b570(void);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];int32_t a,b,c,d;(void)site;
 switch(target){
 case 0x51f780u:FUN_0051f780();goto done;
 case 0x475dc0u:FUN_00475dc0();goto done;
 case 0x476530u:FUN_00476530();goto done;
 case 0x477210u:FUN_00477210();goto done;
 case 0x47b570u:FUN_0047b570();goto done;
 case 0x47b1a0u:FUN_0047b1a0();goto done;
 case 0x47d870u:FUN_0047d870(U(sp+4u));goto done;
 default:break;
 }
 a=(int32_t)U(sp+4u);b=(int32_t)U(sp+8u);c=(int32_t)U(sp+12u);d=(int32_t)U(sp+16u);
 switch(target){
 case 0x4770f0u:FUN_004770f0(a,b,c,d);break;
 case 0x475bf0u:FUN_00475bf0(a,b,c,d);break;
 case 0x4767d0u:FUN_004767d0(a,b,c,d,U(sp+20u),U(sp+24u));break;
 case 0x4777c0u:FUN_004777c0(a,b,c,d);break;
 case 0x477500u:FUN_00477500(a,b,c,d);break;
 case 0x4776f0u:FUN_004776f0(a,b,c,d);break;
 case 0x475cc0u:FUN_00475cc0(a,b,c,d,U(sp+20u));break;
 case 0x477890u:FUN_00477890(a,b,c,d);break;
 case 0x475720u:FUN_00475720(a,b,c,d,U(sp+20u));break;
 case 0x47bae0u:FUN_0047bae0(a,b,c,d,U(sp+20u));break;
 default:abort();
 }
 done:f->r[4]=sp+4u;
}
void FUN_004dea50(void)
{
 uint32_t stack[64];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<64;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+32);
 restore_C_004dea50(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+33)||stack[32]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<13;++i)if(stack[i]!=SENTRY)abort();for(i=33;i<64;++i)if(stack[i]!=SENTRY)abort();
}
