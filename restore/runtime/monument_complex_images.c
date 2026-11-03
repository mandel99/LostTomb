#include "monument_complex_images.h"
#include "monument_progress.h"
#include <stdlib.h>
#define U(a) (*(volatile uint32_t *)(uintptr_t)(uint32_t)(a))
#define W(a) (*(volatile uint16_t *)(uintptr_t)(uint32_t)(a))
#define B(a) (*(volatile uint8_t *)(uintptr_t)(uint32_t)(a))
#define H(a) ((uint32_t)(int32_t)(int16_t)W(a))
extern uint32_t FUN_004ea930(void);
struct image_rule {uint32_t cold,hot;uint8_t cold_phase,cold_orientation,hot_phase,hot_orientation;};
static const struct image_rule rules[]={
 {0xf5746cu,0xf57474u,1,1,0,1},
 {0xf5747cu,0xf57484u,1,1,0,1},
 {0xf5748cu,0xf57494u,1,1,0,1},
 {0xf5749cu,0xf574a4u,1,1,0,1},
 {0x0u,0x0u,0,0,0,0},
 {0xf574acu,0xf574b4u,1,1,0,1},
 {0xf574bcu,0xf574c4u,1,1,0,1},
 {0xf574ccu,0xf574d4u,1,1,0,1},
 {0xf574dcu,0x0u,1,0,0,0},
 {0xf574ecu,0x0u,0,1,0,0},
 {0xf574f4u,0x0u,0,1,0,0},
 {0xf574fcu,0x0u,0,1,0,0},
 {0xf57504u,0xf5750cu,1,1,0,1},
 {0xf57594u,0xf5759cu,1,1,0,1},
 {0xf575a4u,0xf575acu,1,1,0,1},
 {0xf575b4u,0xf575bcu,1,1,0,1},
 {0xf575c4u,0xf575ccu,1,1,0,1},
 {0xf575d4u,0xf575dcu,1,1,0,1},
 {0xf57614u,0x0u,0,0,0,0},
 {0xf57624u,0x0u,0,1,0,0},
 {0xf575e4u,0xf575ecu,1,1,0,1},
 {0xf575f4u,0xf575fcu,1,1,0,1},
 {0xf57604u,0xf5760cu,1,1,0,1},
 {0xf5762cu,0x0u,0,1,0,0},
 {0xf57634u,0x0u,0,1,0,0},
 {0xf5763cu,0xf57644u,1,1,0,1},
 {0xf5764cu,0xf57654u,1,1,0,1},
 {0xf5765cu,0xf57664u,1,1,0,1},
 {0xf5766cu,0xf57674u,1,1,0,1},
 {0xf5767cu,0xf57684u,1,1,0,1},
 {0xf5768cu,0xf57694u,1,1,0,1},
 {0xf5769cu,0xf576a4u,1,1,0,1},
 {0xf576acu,0xf576b4u,1,1,0,1},
 {0xf576bcu,0xf576c4u,1,1,0,1},
 {0xf576ccu,0xf576d4u,1,1,0,1},
 {0xf576fcu,0x0u,0,0,0,0},
 {0xf5770cu,0x0u,0,1,0,0},
 {0xf576dcu,0xf576e4u,1,1,0,1},
 {0xf576ecu,0xf576f4u,1,1,0,1},
 {0xf57714u,0x0u,0,1,0,0},
 {0xf5771cu,0x0u,0,1,0,0},
 {0xf57724u,0xf5772cu,1,1,0,1},
 {0xf57734u,0xf5773cu,1,1,0,1},
 {0xf57744u,0xf5774cu,1,1,0,1},
 {0xf57754u,0xf5775cu,1,1,0,1},
 {0xf57764u,0xf5776cu,1,1,0,1},
 {0xf57774u,0xf5777cu,1,1,0,1},
 {0xf57784u,0xf5778cu,1,1,0,1},
 {0xf57794u,0xf5779cu,1,1,0,1},
 {0xf577a4u,0xf577acu,1,1,0,1},
 {0xf577b4u,0xf577bcu,1,1,0,1},
 {0xf577c4u,0xf577ccu,1,1,0,1},
 {0xf577d4u,0xf577dcu,1,1,0,1},
 {0xf57814u,0x0u,0,0,0,0},
 {0xf57824u,0x0u,0,1,0,0},
 {0xf577e4u,0xf577ecu,1,1,0,1},
 {0xf577f4u,0xf577fcu,1,1,0,1},
 {0xf57804u,0xf5780cu,1,1,0,1},
 {0xf5782cu,0x0u,0,1,0,0},
 {0xf57834u,0x0u,0,1,0,0},
 {0xf5783cu,0xf57844u,1,1,0,1},
};
static const uint8_t shapes_00545760[]={0,1,2,3,4,4,5,4,4,4,4,4,4,4,4,6,7,8,9,4,4,4,4,4,4,4,4,10,11,12};
static const uint8_t shapes_00558c00[]={13,14,15,4,4,4,4,4,4,16,17,4,4,4,4,4,4,18,19,20,21,22,4,4,4,4,4,23,24,25};
static const uint8_t shapes_00555ea0[]={26,27,28,4,29,30,31,32,33,4,4,4,34,4,4,4,4,35,36,4,4,4,37,38,4,4,4,39,40,41};
static const uint8_t shapes_0054f490[]={42,43,44,4,45,4,46,47,48,4,4,49,50,51,52,4,4,53,54,4,4,4,4,4,55,56,57,58,59,60};

uint32_t restore_complex_image_query(uint32_t target,uint32_t self,uint32_t building,uint32_t output)
{
 const uint8_t *shapes=0;const struct image_rule *rule;
 uint32_t count=0,terminal_reset=1,r=building*264u,step,shape,relative,delta,address,phase,orientation;
 switch(target){
 case 0x545760u:shapes=shapes_00545760;count=9;terminal_reset=0;break;
 case 0x558c00u:shapes=shapes_00558c00;count=10;break;
 case 0x555ea0u:shapes=shapes_00555ea0;count=21;break;
 case 0x54f490u:shapes=shapes_0054f490;count=31;break;
 default:abort();
 }
 U(output)=0;step=U(0x799a38u+r);
 if(!step){
  shape=U(0x799a40u+r);
  if(shape!=18u&&shape!=27u&&shape!=28u){
   U(output)=1;shape=B(0x799a74u+r);return shape+U(0xf574dcu);
  }
 }
 relative=U(0x799a68u+r)-(uint32_t)((int32_t)U(0xbca2e0u)/2);
 if((int32_t)relative<0)relative+=4u; /* One signed correction, not modulo4. */
 delta=step==3u?4u:step==4u?8u:step==5u?12u:0u;
 if(terminal_reset&&H(0x7999eau+r)==U(self+0x50u))delta=0;
 /* Genuine material predicate and full x87/worker progress provider execute. */
 if((uint8_t)FUN_004ea930()&&B(0x799a6cu+r)){
  if(restore_monument_progress((void *)(uintptr_t)self,building,(int32_t)count,0)==100){delta+=4u;goto select_image;}
 }
 if(W(0x7999eeu+r)==0xffffu&&B(0x799a6cu+r))delta+=4u;
select_image:
 shape=U(0x799a40u+r);if(shape>29u)return 0;
 rule=rules+shapes[shape];address=rule->cold;phase=rule->cold_phase;orientation=rule->cold_orientation;
 if(rule->hot&&U(self+0x54u)){address=rule->hot;phase=rule->hot_phase;orientation=rule->hot_orientation;}
 if(!address)return 0;
 return U(address)+delta*phase+relative*orientation;
}
#define ENTRY(token) \
uint32_t restore_complex_image_##token(uint32_t self,uint32_t building,uint32_t output) \
{return restore_complex_image_query(0x##token##u,self,building,output);}
ENTRY(00545760)
ENTRY(00558c00)
ENTRY(00555ea0)
ENTRY(0054f490)
#define ALIAS(token) \
__declspec(naked) uint32_t FUN_##token(void) { \
 __asm { push dword ptr[esp+8] } __asm { push dword ptr[esp+8] } \
 __asm { push ecx } __asm { call restore_complex_image_##token } \
 __asm { add esp,12 } __asm { ret } }
ALIAS(00545760)
ALIAS(00558c00)
ALIAS(00555ea0)
ALIAS(0054f490)
