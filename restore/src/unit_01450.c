#include "actor_combat.h"
#include "actor_constable.h"

void FUN_004b5210(int param_1)

{
  switch(CB_B(cb_actor((uint32_t)param_1)+2u)) {
  case 0xb: ;
  case 0x2b: ;
  case 0x37: ;
    FUN_0053b4a0(param_1,0xb,2);
    return;
  case 0xc: ;
  case 0x2c: ;
  case 0x36: ;
    FUN_0053b4a0(param_1,0xc,2);
    return;
  case 0xd: ;
  case 0x2d: ;
  case 0x38: ;
  case 99: ;
    FUN_0053b4a0(param_1,0xd,2);
    return;
  case 0x4e: ;
  case 0x5d: ;
  case 100: ;
    FUN_0053b4a0(param_1,0x11,2);
  }
  return;
}
