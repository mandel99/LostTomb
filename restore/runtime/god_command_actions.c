#include "god_command_actions.h"
#include "god_effects.h"
#include "pending_message.h"
#include <string.h>
/* Twenty whole six-instruction native wrappers share one parameterized
   C body. Every row authenticates PUSH effect/PUSH1/PUSH god/CALL/ADD/RET.
   Their implicit God-local prestate is explicit rather than host garbage. */
struct GodCommand {uint32_t token;int32_t god,effect;};
static const struct GodCommand commands[]={
 {0x0041ca80u,0,1},
 {0x0041ca90u,0,2},
 {0x0041caa0u,0,3},
 {0x0041cab0u,0,4},
 {0x0041cac0u,1,1},
 {0x0041cad0u,1,2},
 {0x0041cae0u,1,3},
 {0x0041caf0u,1,4},
 {0x0041cb00u,2,1},
 {0x0041cb10u,2,2},
 {0x0041cb20u,2,3},
 {0x0041cb30u,2,4},
 {0x0041cb40u,3,1},
 {0x0041cb50u,3,2},
 {0x0041cb60u,3,3},
 {0x0041cb70u,3,4},
 {0x0041cb80u,4,1},
 {0x0041cb90u,4,2},
 {0x0041cba0u,4,3},
 {0x0041cbb0u,4,4},
};
int restore_god_command_action(uint32_t token)
{
 uint32_t i;
 for(i=0;i<sizeof(commands)/sizeof(commands[0]);++i){
  if(commands[i].token==token){
   const uint8_t *initial=restore_command_take_native_payload();
   uint8_t scratch[30];
   if(!initial)return 0;
   memcpy(scratch,initial,sizeof(scratch));
   restore_god_effect_core_004c47a0(commands[i].god,1u,commands[i].effect,scratch);
   return 1;
  }
 }
 return 0;
}
