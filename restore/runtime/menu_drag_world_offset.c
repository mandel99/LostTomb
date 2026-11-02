#include "world_offset_model.h"
#include <stdlib.h>
#define SENTRY UINT32_C(0x8e47b123)
void FUN_004405c0(int32_t direction,int32_t *out_x,int32_t *out_y)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=SENTRY;for(i=0;i<8;++i)f.r[i]=0;
 stack[17]=(uint32_t)direction;stack[18]=(uint32_t)(uintptr_t)out_x;stack[19]=(uint32_t)(uintptr_t)out_y;
 f.flags=2;f.boundary=0;f.r[4]=(uint32_t)(uintptr_t)(stack+16);restore_C_004405c0(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=SENTRY||f.r[3]||f.r[5]||f.r[6]||f.r[7])abort();
 for(i=0;i<16;++i)if(stack[i]!=SENTRY)abort();for(i=20;i<32;++i)if(stack[i]!=SENTRY)abort();
}
