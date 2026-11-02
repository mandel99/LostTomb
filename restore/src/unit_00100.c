#include "restore_types.h"
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)

void FUN_0040d7f0(int param_1)

{
  int iVar1;
  int *in_ECX;
  undefined4 *puVar2;
  
  iVar1 = param_1 + 0x19;
  if (param_1 < iVar1) {
    puVar2 = (undefined4 *)(&DAT_00799a38 + param_1 * 0x108);
    do {
      puVar2[-2] = 0x15;
      *puVar2 = 1;
      *(undefined1 *)(puVar2 + 2) = 1;
      *(undefined2 *)((int)puVar2 + -0x4a) = 0xffff;
      (**(code **)(*in_ECX + 0x18))(param_1);
      param_1 = param_1 + 1;
      puVar2 = puVar2 + 0x42;
    } while (param_1 < iVar1);
  }
  return;
}
