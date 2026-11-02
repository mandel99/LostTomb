#include "restore_types.h"
extern undefined4 FUN_0046f930();

bool FUN_0047e830(int param_1)

{
  char cVar1;
  
  cVar1 = FUN_0046f930(param_1 % 0xe4,param_1 / 0xe4);
  return cVar1 == '\0';
}
