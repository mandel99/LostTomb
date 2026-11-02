#include "basic_command_actions.h"
#include "monument_effects.h"
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))

/* Original command-table rows0,1,2; each nullary cdecl/RET0 without ECX.
   Whole bodies are respectively4,11,5 PE instructions, with no callees. */
void FUN_0041cfc0(void)
{
 B(0xe38dd8u)=1;
 B(0xe38dd7u)=1;
}
void FUN_0041cfd0(void)
{
 uint32_t scene=U(0xeaac28u)*0x49d8u,cash=U(0xea187cu+scene);
 if((int32_t)cash<15000){
  /* Native tests before adding; values14999->15999 are not clamped. */
  U(0xea187cu+scene)=cash+1000u;
  U(0xea5d10u+scene)=U(0xea5d10u+scene)+1000u;
 }
}
void FUN_0041ccf0(void)
{
 uint8_t value=B(0x7027e1u);
 B(0x7027e1u)=value==0;
}
/* Whole 41CC80..41CC91, seven instructions. The query only gates the
   action: native always pushes amount TWO, rather than its query result. */
void FUN_0041cc80(void)
{
 if(restore_monument_effect_level_004f5340()!=0)
  restore_monument_effect_004f3aa0(2);
}
int restore_basic_command_action(uint32_t token)
{
 switch(token){
 case 0x41cc80u:FUN_0041cc80();return 1;
 case 0x41cfc0u:FUN_0041cfc0();return 1;
 case 0x41cfd0u:FUN_0041cfd0();return 1;
 case 0x41ccf0u:FUN_0041ccf0();return 1;
 default:return 0;
 }
}
