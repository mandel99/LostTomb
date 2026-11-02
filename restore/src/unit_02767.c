#include "restore_types.h"
extern void FUN_005610d4();
extern void * FUN_00561a1d();

undefined4 FUN_005610a7(void)

{
  void *pvVar1;
  
  pvVar1 = FUN_00561a1d(0x200);
  if (pvVar1 == (void *)0x0) {
    return 0xffffffff;
  }
  FUN_005610d4(pvVar1,(int)pvVar1 + 0x200,1);
  return 1;
}
