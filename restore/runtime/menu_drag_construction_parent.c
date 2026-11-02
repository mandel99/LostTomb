#include "road_check_model.h"
#include "road_scan_model.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4],a=U(sp+4),b=U(sp+8),c=U(sp+12);(void)site;
 switch(target){
 case 0x43f250u:f->r[0]=FUN_0043f250(a,(int32_t)b,(int32_t)c,(int32_t)U(sp+16),(int32_t)U(sp+20));break;
 case 0x43f3a0u:f->r[0]=FUN_0043f3a0((int32_t)a,(int32_t)b,(int32_t)c);break;
 case 0x441040u:f->r[0]=FUN_00441040((int32_t)a,(int32_t)b,(int32_t)c,(uint32_t *)(uintptr_t)U(sp+16),(int32_t *)(uintptr_t)U(sp+20));break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
static uint32_t execute(void (*body)(struct restore_menu_input_frame *),const uint32_t *args,unsigned count)
{
 uint32_t stack[128];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<128;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 for(i=0;i<count;++i)stack[65+i]=args[i];
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+64);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+65)||stack[64]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<18;++i)if(stack[i]!=SENTRY)abort();for(i=65+count;i<128;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
uint32_t FUN_0043f410(int32_t kind)
{uint32_t args[1]={(uint32_t)kind};return execute(restore_C_0043f410,args,1);}
uint32_t FUN_00441130(int32_t x,int32_t y,int32_t *out_x,int32_t *out_y,int32_t primary_kind,int32_t secondary_kind)
{uint32_t args[6]={(uint32_t)x,(uint32_t)y,(uint32_t)(uintptr_t)out_x,(uint32_t)(uintptr_t)out_y,(uint32_t)primary_kind,(uint32_t)secondary_kind};return execute(restore_C_00441130,args,6);}
