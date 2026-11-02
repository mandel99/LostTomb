#include "actor_profile_model.h"
#include <stdlib.h>
void FUN_004b5af0(int32_t actor)
{
 uint32_t stack[32];struct restore_menu_input_frame f;unsigned i;
 for(i=0;i<32;++i)stack[i]=UINT32_C(0x8e47b123);
 for(i=0;i<8;++i)f.r[i]=0;
 f.flags=2;f.boundary=0;f.r[4]=(uint32_t)(uintptr_t)(stack+16);stack[17]=(uint32_t)actor;
 restore_C_004b5af0(&f);
 if(f.r[4]!=(uint32_t)(uintptr_t)(stack+17)||stack[16]!=UINT32_C(0x8e47b123)||f.r[6])abort();
 for(i=0;i<15;++i)if(stack[i]!=UINT32_C(0x8e47b123))abort();
 for(i=18;i<32;++i)if(stack[i]!=UINT32_C(0x8e47b123))abort();
}
