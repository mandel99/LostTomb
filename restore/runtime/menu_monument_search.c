/* Pinned Pharaoh fde7cd6542ab00e3952d0366c0c5c52dde59a70160280f172e24d41406d33c8d:
 * complete 004f5d90..004f5e6b (70 instructions),004f6000..004f6013 (6).
 * Both read one stack DWORD, have no incoming register data input, and RET0. */
#include "menu_monument_search.h"
#include "editor_brush_validation.h"
#define MQ_U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define MQ_W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define MQ_B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
uint32_t FUN_004f5d90(uint32_t building)
{
 uint32_t candidate=1;
 MQ_U(0xeaae88u)=building;
 do{
  uint32_t offset=candidate*264u;
  if(MQ_B(0x7999d8u+offset)&&MQ_W(0x7999e8u+offset)==0xb7u){
   uint32_t tool=MQ_B(0x799a2cu+offset),version,step;
   if(FUN_004ecea0((int32_t)tool)){
    uint32_t delta=MQ_U(0x5f5d08u+tool*4u);
    delta+=MQ_U(0x7999e4u+offset);delta-=MQ_U(0xbca2d8u);
    FUN_004ed360((int32_t)delta%228,(int32_t)delta/228,6,0);
    if(MQ_B(0xeaae84u))return candidate;
   }
   version=MQ_U(0x5e4ef4u);tool=MQ_B(0x799a2cu+offset);
   step=MQ_U((version==5u?0x56f7ecu:0x56f7d8u)+tool*4u);
   candidate+=step-1u;
  }
  ++candidate;
 }while((int32_t)candidate<4000);
 return building;
}
int32_t FUN_004f6000(uint32_t building)
{return (int32_t)(int16_t)MQ_W(0x7999eau+building*264u);}
