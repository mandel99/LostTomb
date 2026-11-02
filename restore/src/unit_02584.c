#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a6e (*(undefined4 *)0x00799a6eu)

void FUN_00547ad0(int param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int *in_ECX;
  int iVar3;
  
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  iVar1 = (**(code **)(*in_ECX + 0x3c))();
  iVar1 = iVar1 + param_1;
  *(undefined4 *)((int)&DAT_00799a6e + param_1 * 0x108 + 2) = 400;
  (&DAT_00799a30)[param_1 * 0x42] = 1;
  if (param_1 < iVar1) {
    puVar2 = (undefined4 *)(&DAT_00799a38 + param_1 * 0x108);
    iVar3 = iVar1 - param_1;
    do {
      if (puVar2[2] != 0x11) {
        *puVar2 = 4;
      }
      *(undefined2 *)((int)puVar2 + -0x4a) = 0xffff;
      *(undefined1 *)((int)puVar2 + 0x3d) = 1;
      puVar2 = puVar2 + 0x42;
      iVar3 = iVar3 + -1;
    } while (iVar3 != 0);
    for (; param_1 < iVar1; param_1 = param_1 + 1) {
      (**(code **)(*in_ECX + 0x18))(param_1);
    }
  }
  return;
}
