#include "invasion_commands.h"
#include <stdint.h>
#include <stdlib.h>

/* The selected cdecl children define their live scratch registers from
   explicit arguments. Keep complete RAW implementations for these edges. */
static uint32_t city_raw_call(uint32_t entry,const uint32_t *arguments,uint32_t count)
{
    uint32_t stack[8192],registers[8]={0},flags=2,i;
    uint32_t sp=(uint32_t)(uintptr_t)(stack+8000);
    stack[8000]=0;
    for(i=0;i<count;++i)stack[8001+i]=arguments[i];
    registers[4]=sp;
    restore_invasion_raw(entry,registers,&flags);
    if(registers[4]!=sp+4u)abort();
    return registers[0];
}

uint32_t restore_actor_tile_heat_overflow_raw(int32_t x,int32_t y,int32_t tile)
{
    const uint32_t arguments[]={(uint32_t)x,(uint32_t)y,(uint32_t)tile};
    return city_raw_call(0x469140u,arguments,3);
}

void restore_city_disaster_actors_004b7510(int32_t x,int32_t y)
{
    const uint32_t arguments[]={(uint32_t)x,(uint32_t)y};
    (void)city_raw_call(0x4b7510u,arguments,2);
}

void restore_city_disaster_refresh_00497b90(void)
{
    (void)city_raw_call(0x497b90u,0,0);
}
