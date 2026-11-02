#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)

void FUN_0040ecf0(int param_1)

{
  int iVar1;
  int *in_ECX;
  int *piVar2;
  int iVar3;
  
  iVar3 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  iVar1 = (&DAT_00799a30)[iVar3 * 0x42];
  piVar2 = &DAT_00799a30 + iVar3 * 0x42;
  if ((iVar1 < 0x15) && (iVar3 < iVar3 + 0x19)) {
    do {
      *piVar2 = iVar1 + 1;
      (**(code **)(*in_ECX + 0x18))(iVar3);
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 0x42;
    } while (iVar3 < (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) + 0x19);
  }
  return;
}
