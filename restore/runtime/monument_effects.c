#include "monument_effects.h"
#include "pyramid_summary.h"
#include "monument_rating.h"
#include "actor_walking.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

static uint32_t footprint(uint32_t mode,uint32_t stage)
{return U((mode==5u?0x56f7ecu:0x56f7d8u)+stage*4u);}

/* Whole 4F5340..4F53CD, 58 instructions, nullary cdecl/full EAX.
   Count is a DWORD captured once; the native upper bound is inclusive. */
uint32_t restore_monument_effect_level_004f5340(void)
{
 uint32_t building=1,best=0,address=0x799b34u;
 int32_t maximum=-1,count,progress;
 int32_t summary[28];
 if(!B(0xe38ec3u))return 0;
 count=(int32_t)U(0x78d790u);
 if(count<1)return 0;
 do{
  if(B(address-0x54u)==1u&&W(address-0x44u)==183u){
   progress=B(address);
   if(progress>maximum){maximum=progress;best=building;}
  }
  ++building;address+=264u;
 }while((int32_t)building<=count);
 if(maximum<0)return 0;
 (void)FUN_004f1310(best,summary);
 /* Native reads ESP+48 before dropping eight outgoing argument bytes:
    buffer starts at the pre-call ESP+0C, so this is DWORD13. */
 progress=summary[13];
 if(progress<50)return 2;
 return progress<75?1u:0u;
}

/* Whole 4F3AA0..4F3C8B, 142 instructions, cdecl1/RET0. This body uses
   DWORD building counts, byte state/stage, and signed WORD actor links.
   The native outer bound is strict and reloads the count after each group. */
void restore_monument_effect_004f3aa0(int32_t amount)
{
 uint32_t building=1,minimum=999999u;
 int32_t summary[28];
 if((int32_t)U(0x78d790u)<=1)return;
 do{
  uint32_t record=building*264u;
  if(B(0x7999d8u+record)==1u&&W(0x7999e8u+record)==183u){
   uint32_t actor=1,address=0xbcd786u,candidate=999999u;
   do{
    if(B(address+4u)){
     uint8_t kind=B(address);
     int eligible=kind==0x55u?
       (int32_t)(int16_t)W(address+0x54u)>=(int32_t)building:
       kind==0x51u||kind==0x4fu||kind==0x50u||kind==0x60u;
     if(eligible){
      uint32_t mode=U(0x5e4ef4u),stage=B(0x799a2cu+record);
      uint32_t end=footprint(mode,stage)+building;
      if((int32_t)(int16_t)W(address+0x54u)<(int32_t)end){
       B(address+0x2eu)=4;
       FUN_004b5420((int32_t)actor);
      }
     }
    }
    address+=388u;++actor;
   }while((int32_t)address<0xc8ad42);
   if(B(0x799a2du+record)>1u){
    uint32_t mode,stage,bound;
    (void)FUN_004f1310(building,summary);
    candidate=(uint32_t)summary[17]-(uint32_t)summary[16]+1u;
    mode=U(0x5e4ef4u);stage=B(0x799a2cu+record);
    bound=footprint(mode,stage)*(uint32_t)amount*2u;
    if((int32_t)bound<(int32_t)candidate){
     /* Original re-reads the chosen table in this branch, with captured
        mode/stage rather than another global lookup. */
     candidate=footprint(mode,stage)*(uint32_t)amount*2u;
    }
   }else if(!FUN_004f53d0(building)){
    uint32_t mode=U(0x5e4ef4u),stage=B(0x799a2cu+record);
    candidate=footprint(mode,stage)*(uint32_t)amount;
   }
   if((int32_t)candidate<(int32_t)minimum)minimum=candidate;
   {
    uint32_t mode=U(0x5e4ef4u),stage=B(0x799a2cu+record);
    /* Native adds span-1 inside the group and then increments once. */
    building+=footprint(mode,stage);
   }
  }else ++building;
 }while((int32_t)building<(int32_t)U(0x78d790u));
 if(minimum!=999999u){B(0xeaaf5cu)=1;U(0x78d7a4u)=minimum;}
}
uint32_t FUN_004f5340(void){return restore_monument_effect_level_004f5340();}
void FUN_004f3aa0(int32_t amount){restore_monument_effect_004f3aa0(amount);}
