#include "restore_types.h"
#define DAT_005763bc (*(undefined4 *)0x005763bcu)
extern undefined4 FUN_00561994();
extern undefined4 * FUN_00561a0a();
extern uint FUN_00561ca1();
extern int FUN_0056b1b6();
extern undefined4 FUN_0056b22e();

undefined4 FUN_004ceb10(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = FUN_00561a0a(param_1,&DAT_005763bc);
  if (iVar1 == 0) {
    return 0;
  }
  uVar2 = FUN_0056b22e(iVar1);
  uVar2 = FUN_0056b1b6(uVar2);
  FUN_00561ca1(param_2,1,uVar2,iVar1);
  FUN_00561994(iVar1);
  return 1;
}
