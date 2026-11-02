#include "road_scan_model.h"
#include <stdlib.h>
#define SENTRY UINT32_C(0x8e47b123)
static uint32_t execute(void (*body)(struct restore_menu_input_frame *),const uint32_t *args,unsigned count)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 for(i=0;i<count;++i)stack[17+i]=args[i];
 f.flags=2;f.boundary=0;f.r[4]=(uint32_t)(uintptr_t)(stack+16);body(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<4;++i)if(stack[i]!=SENTRY)abort();for(i=17+count;i<32;++i)if(stack[i]!=SENTRY)abort();
 return f.r[0];
}
uint32_t FUN_0043f250(uint32_t x,int32_t y,int32_t origin_x,int32_t origin_y,int32_t extent)
{uint32_t args[5]={x,(uint32_t)y,(uint32_t)origin_x,(uint32_t)origin_y,(uint32_t)extent};return execute(restore_C_0043f250,args,5);}
uint32_t FUN_0043f3a0(int32_t x,int32_t y,int32_t extent)
{uint32_t args[3]={(uint32_t)x,(uint32_t)y,(uint32_t)extent};return execute(restore_C_0043f3a0,args,3);}
uint32_t FUN_00441040(int32_t extent,int32_t rows,int32_t columns,uint32_t *count,int32_t *locations)
{uint32_t args[5]={(uint32_t)extent,(uint32_t)rows,(uint32_t)columns,(uint32_t)(uintptr_t)count,(uint32_t)(uintptr_t)locations};return execute(restore_C_00441040,args,5);}
void FUN_004413b0(int32_t x,int32_t y,int32_t kind)
{uint32_t args[3]={(uint32_t)x,(uint32_t)y,(uint32_t)kind};(void)execute(restore_C_004413b0,args,3);}
