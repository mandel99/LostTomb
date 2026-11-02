#include "restore_types.h"
extern void * FUN_00561b68();
extern size_t FUN_00563380();
extern uint * FUN_00566750();

undefined4 FUN_0056b177(char *param_1)

{
  size_t sVar1;
  void *pvVar2;
  undefined4 uVar3;
  
  if (param_1 != (char *)0x0) {
    sVar1 = FUN_00563380(param_1);
    pvVar2 = FUN_00561b68(sVar1 + 1);
    if (pvVar2 != (void *)0x0) {
      uVar3 = FUN_00566750(pvVar2,param_1);
      return uVar3;
    }
  }
  return 0;
}
