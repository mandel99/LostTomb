#include "restore_types.h"
extern undefined4 FUN_0056073c();

undefined4 FUN_00560855(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  int in_ECX;
  
  if (*(uint *)(in_ECX + 0x1c) < *(uint *)(in_ECX + 0x20)) {
LAB_005608ae: ;
    if ((param_1 != -1) &&
       (**(undefined1 **)(in_ECX + 0x1c) = (char)param_1, *(int *)(in_ECX + 0x20) != 0)) {
      *(int *)(in_ECX + 0x1c) = *(int *)(in_ECX + 0x1c) + 1;
    }
    uVar2 = 1;
  }
  else {
    if (*(int *)(in_ECX + 0x30) != 0) {
      iVar1 = FUN_0056073c();
      if (iVar1 != -1) {
        if (*(int *)(in_ECX + 0x20) == 0) {
          iVar1 = (*(int *)(in_ECX + 0x2c) - *(int *)(in_ECX + 0x24)) + *(int *)(in_ECX + 0x10);
          *(int *)(in_ECX + 0x18) = iVar1;
          *(int *)(in_ECX + 0x1c) = iVar1;
          *(undefined4 *)(in_ECX + 0x20) = *(undefined4 *)(in_ECX + 0x14);
        }
        else {
          uVar2 = *(undefined4 *)(in_ECX + 0x1c);
          *(undefined4 *)(in_ECX + 0x1c) = *(undefined4 *)(in_ECX + 0x18);
          *(int *)(in_ECX + 0x20) = *(int *)(in_ECX + 0x14);
          if (*(int *)(in_ECX + 0x14) != 0) {
            *(undefined4 *)(in_ECX + 0x1c) = uVar2;
          }
        }
        goto LAB_005608ae;
      }
    }
    uVar2 = 0xffffffff;
  }
  return uVar2;
}
