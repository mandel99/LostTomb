#include "monument_clear_model.h"
#include "menu_prepare_integer.h"
#include "terrain_redraw.h"
#include "clear_helpers.h"
extern void FUN_0047a220(int32_t,int32_t,int32_t);
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_0047b590(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00477500(int32_t,int32_t,int32_t,int32_t);
extern void FUN_004777c0(int32_t,int32_t,int32_t,int32_t);
extern void FUN_00475cc0(int32_t,int32_t,int32_t,int32_t,uint32_t);
extern void FUN_004776f0(int32_t,int32_t,int32_t,int32_t);
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];int32_t a=(int32_t)U(sp+4u),b,c,d;(void)site;
 if(target==0x475b50u){f->r[0]=FUN_00475b50((uint32_t)a);f->r[4]=sp+4u;return;}
 b=(int32_t)U(sp+8u);c=(int32_t)U(sp+12u);
 if(target==0x47a220u){FUN_0047a220(a,b,c);f->r[4]=sp+4u;return;}
 d=(int32_t)U(sp+16u);
 switch(target){
 case 0x47b590u:FUN_0047b590(a,b,c,d);break;
 case 0x4767d0u:FUN_004767d0(a,b,c,d,U(sp+20u),U(sp+24u));break;
 case 0x477500u:FUN_00477500(a,b,c,d);break;
 case 0x4777c0u:FUN_004777c0(a,b,c,d);break;
 case 0x475cc0u:FUN_00475cc0(a,b,c,d,U(sp+20u));break;
 case 0x4776f0u:FUN_004776f0(a,b,c,d);break;
 case 0x477890u:FUN_00477890(a,b,c,d);break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
void FUN_004c0920(int32_t building)
{
 uint32_t stack[96];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<96;++i)stack[i]=SENTRY;
 for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+64);stack[65]=(uint32_t)building;
 restore_C_004c0920(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+65)||stack[64]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<28;++i)if(stack[i]!=SENTRY)abort();
 for(i=66;i<96;++i)if(stack[i]!=SENTRY)abort();
}
