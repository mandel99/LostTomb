#include "restore_types.h"
#define DAT_012666a4 (*(undefined4 *)0x012666a4u)
extern void FUN_005637ee();
extern bool FUN_005687a7();

undefined4 FUN_0056870b(int *param_1)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  
  piVar1 = (int *)*param_1;
  if (((*piVar1 == -0x1f928c9d) && (piVar1[4] == 3)) && (piVar1[5] == 0x19930520)) {
    uVar2 = (FUN_005637ee(), restore_unresolved_return(0x005637eeu));
    return uVar2;
  }
  if ((DAT_012666a4 != (code *)0x0) && (iVar3 = FUN_005687a7(DAT_012666a4), iVar3 != 0)) {
    uVar2 = ((code *)DAT_012666a4)(param_1);
    return uVar2;
  }
  return 0;
}
