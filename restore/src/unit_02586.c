#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a6e (*(undefined4 *)0x00799a6eu)

undefined4 FUN_00547ba0(int param_1)

{
  char cVar1;
  int iVar2;
  int *in_ECX;
  int iVar3;
  
  if (0 < *(int *)((int)&DAT_00799a6e +
                  (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108 + 2)) {
    return 0;
  }
  iVar2 = (**(code **)(*in_ECX + 0x38))();
  iVar3 = 0;
  if (0 < iVar2) {
    do {
      cVar1 = (**(code **)(*in_ECX + 0x24))(iVar3 + param_1);
      if (cVar1 != '\0') {
        return 0;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < iVar2);
  }
  return 1;
}
