#include "building_release_model.h"
#include "building_cleanup.h"
#include "group_clear.h"
#include "monument_unlink.h"
#include "monument_clear_model.h"
#include "redraw_dispatch_model.h"
#include "menu_prepare_integer.h"
#include <stdlib.h>
extern void FUN_004a36b0(int32_t);
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0047bd50(int32_t,int32_t,int32_t,int32_t);
extern void FUN_0040f8f0(uint32_t *,uint32_t,uint8_t);
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];int32_t a;(void)site;
 if(target==0x4dea50u){FUN_004dea50();f->r[4]=sp+4u;return;}
 a=(int32_t)U(sp+4u);
 switch(target){
 case 0x46a2e0u:FUN_0046a2e0(a);break;
 case 0x4a36b0u:FUN_004a36b0(a);break;
 case 0x4b82e0u:FUN_004b82e0(a);break;
 case 0x4f4ae0u:FUN_004f4ae0(a);break;
 case 0x4e5830u:FUN_004e5830(a);break;
 case 0x4c0920u:FUN_004c0920(a);break;
 case 0x475b50u:f->r[0]=FUN_00475b50((uint32_t)a);break;
 case 0x47b590u:FUN_0047b590(a,(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u));break;
 case 0x47bd50u:FUN_0047bd50(a,(int32_t)U(sp+8u),(int32_t)U(sp+12u),(int32_t)U(sp+16u));break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
void FUN_0046ac50(int32_t building)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+16);stack[17]=(uint32_t)building;
 restore_C_0046ac50(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<3;++i)if(stack[i]!=SENTRY)abort();for(i=18;i<32;++i)if(stack[i]!=SENTRY)abort();
}
void FUN_0046ac10(int32_t building)
{
 FUN_0046ac50(building);
 FUN_0040f8f0((uint32_t *)(uintptr_t)(0x7999d8u+(uint32_t)building*264u),264u,0);
 if(U(0xbc9690u)==9u)FUN_00485200();
}
