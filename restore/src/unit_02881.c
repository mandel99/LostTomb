#include "restore_types.h"
extern undefined4 * FUN_00562880();
extern size_t FUN_00563380();

void FUN_0056457b(char *param_1,int param_2)

{
  size_t sVar1;
  
  if (param_2 != 0) {
    sVar1 = FUN_00563380(param_1);
    FUN_00562880(param_1 + param_2,param_1,sVar1 + 1);
  }
  return;
}
