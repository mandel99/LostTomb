#include "route_query_model.h"
#include "map_commit_links.h"
#include "map_commit_paths.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define SENTRY UINT32_C(0x8e47b123)
static void boundary(uint32_t site,uint32_t target,struct restore_menu_input_frame *f)
{
 uint32_t sp=f->r[4];int32_t a=(int32_t)U(sp+4u),b=(int32_t)U(sp+8u),c=(int32_t)U(sp+12u);(void)site;
 switch(target){
 case 0x4806c0u:f->r[0]=FUN_004806c0(a,b,(uint32_t)c);break;
 case 0x51de70u:f->r[0]=FUN_0051de70(a,b,c,(int32_t)U(sp+16u));break;
 case 0x51e720u:f->r[0]=(uint32_t)FUN_0051e720(a,b,c,(int32_t)U(sp+16u),(int32_t)U(sp+20u),(int32_t)U(sp+24u),(uint32_t *)(uintptr_t)U(sp+28u));break;
 default:abort();
 }
 f->r[4]=sp+4u;
}
uint32_t FUN_00486a30(int32_t start_x,int32_t start_y,int32_t end_x,int32_t end_y,uint32_t layout,int32_t orientation,int32_t wanted_orientation,uint32_t *path,int32_t *count)
{
 uint32_t stack[64];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<64;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[33]=(uint32_t)start_x;stack[34]=(uint32_t)start_y;stack[35]=(uint32_t)end_x;stack[36]=(uint32_t)end_y;stack[37]=layout;
 stack[38]=(uint32_t)orientation;stack[39]=(uint32_t)wanted_orientation;stack[40]=(uint32_t)(uintptr_t)path;stack[41]=(uint32_t)(uintptr_t)count;
 f.flags=2;f.boundary=boundary;f.r[4]=(uint32_t)(uintptr_t)(stack+32);
 restore_C_00486a30(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+33)||stack[32]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<14;++i)if(stack[i]!=SENTRY)abort();for(i=42;i<64;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
