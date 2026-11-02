#include "mission_schema.h"
#include "chunk_io.h"
#include "game_diagnostics.h"
#include <stdint.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
extern void FUN_00486e70(void);
static uint32_t recover_terrain;

static void descriptor(uint32_t index,uint32_t codec,uint32_t at,int32_t size,int32_t count)
{
    volatile RestoreChunkDescriptor *d=(volatile RestoreChunkDescriptor *)(uintptr_t)(0x5e09a0u+index*20u);
    d->codec=codec;d->destination=at;d->size=size;d->count=count;d->migration=0;
}

/* Whole4D1350,177 instructions. Native CRT slot576068 calls the4D1340
   thunk before main. The static PE image alone leaves descriptors147..180
   unfinished, including campaign mode169 and grass growth170. */
void FUN_004d1350(void)
{
    static const RestoreChunkDescriptor tail[]={
        {0,0x7027b8u,1,26,0},{0,0x6fdc88u,1,15,0},
        {1,0xa9010cu,1,51984,0},{0,0x7027b4u,1,1,0},
        {1,0xa509bcu,1,51984,0},{0,0x5e45b0u,24,10,0},
        {0,0x5e46a0u,24,18,0},{0,0x78d798u,4,1,0},
        {0,0xe39080u,4,1,0},{0,0x6fdce0u,12,1,0},
        {0,0x5c1f80u,24,27,0},{0,0x5c1488u,24,27,0},
        {0,0x5c1e18u,24,15,0},{0,0x5c2208u,24,56,0},
        {0,0x5c1710u,24,75,0}
    };
    uint32_t i;
    /* The codec of147 is statically initialized, and is not written here. */
    U(0x5e1520u)=0xd38360u;U(0x5e1524u)=36;U(0x5e1528u)=1;U(0x5e152cu)=0;
    descriptor(148,1,0x978d9cu,4,51984);
    descriptor(149,0,0x5c1350u,24,13);
    for(i=0;i<16u;++i)descriptor(150u+i,0,0xd0568cu-i*4u,4,1);
    for(i=0;i<15u;++i)descriptor(166u+i,tail[i].codec,tail[i].destination,tail[i].size,tail[i].count);
}
void FUN_004d1340(void){FUN_004d1350();}

/* Whole4D4A60. Native CRT57606C ->4D4A50 finishes map descriptors21..23.
   Keep the statically initialized codec of21 untouched, as in the original. */
void FUN_004d4a60(void)
{
    U(0x5e1980u)=0xd38360u;U(0x5e1984u)=36;U(0x5e1988u)=1;U(0x5e198cu)=0;
    U(0x5e1990u)=0;U(0x5e1994u)=0x5d2c48u;U(0x5e1998u)=8;U(0x5e199cu)=36;U(0x5e19a0u)=0;
    U(0x5e19a4u)=1;U(0x5e19a8u)=0xa509bcu;U(0x5e19acu)=1;U(0x5e19b0u)=51984;U(0x5e19b4u)=0;
}
void FUN_004d4a50(void){FUN_004d4a60();}

/* Early C-only builds serialized exactly this empty147..180 suffix. Do not
   infer missing data for valid native saves or other malformed layouts. The
   family record supplies a checkable campaign identity; custom maps retain
   their mode. Missing growth is regenerated later by the native terrain path,
   after post-load geometry and map indices have been prepared. */
void restore_mission_schema_recover_loaded(void)
{
    uint32_t i,at;int32_t selected;
    recover_terrain=0;
    if(U(0xe945c0u)!=160u||U(0xe945c4u)!=181u)return;
    for(i=147;i<181u;++i){
        at=0xe92e50u+i*20u;
        if(U(at)!=(i==147u?1u:0u)||U(at+4u)||U(at+8u)||U(at+12u)||U(at+16u))return;
    }
    selected=(int8_t)B(0xe38eabu);
    if(!U(0xe38ebcu)&&!B(0x7027b4u)&&selected>=0&&
       selected<(int32_t)U(0x6ff608u)&&
       B(0x6fdd08u+(uint32_t)selected*64u+12u)==B(0xe38ea8u)){
        B(0x7027b4u)=1;
    }
    recover_terrain=1;
    restore_game_trace("legacy C save missing descriptors147..180: campaign=%u grass regeneration pending\n",(uint32_t)B(0x7027b4u));
}
void restore_mission_schema_recover_terrain(void)
{
    if(!recover_terrain)return;
    recover_terrain=0;
    B(0xbca30cu)=1;FUN_00486e70();
    restore_game_trace("legacy C save grass regenerated from loaded terrain\n");
}
