#include "restore_types.h"
extern uint FUN_0056880a();
extern uint FUN_0056889c();

uint FUN_005687bf(uint param_1,uint param_2)

{
  uint uVar1;
  
  uVar1 = FUN_0056880a();
  uVar1 = uVar1 & ~param_2 | param_1 & param_2;
  FUN_0056889c(uVar1);
  return uVar1;
}
