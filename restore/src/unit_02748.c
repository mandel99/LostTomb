#include "restore_types.h"
extern undefined4 FUN_0056073c();
extern undefined4 FUN_00560855();
extern uint FUN_005608cc();

int FUN_0056090c(int param_1,int param_2,byte param_3)

{
  uint uVar1;
  int iVar2;
  int in_ECX;
  int iVar3;
  
  iVar3 = -1;
  if ((param_3 & 1) != 0) {
    FUN_005608cc();
    if (param_2 == 0) {
      iVar3 = *(int *)(in_ECX + 0x24);
    }
    else if (param_2 == 1) {
      iVar3 = *(int *)(in_ECX + 0x28);
    }
    else {
      if (param_2 != 2) {
        return -1;
      }
      iVar3 = *(int *)(in_ECX + 0x2c);
    }
    uVar1 = iVar3 + param_1;
    iVar3 = uVar1 - *(uint *)(in_ECX + 0x24);
    if (uVar1 < *(uint *)(in_ECX + 0x24)) {
      return -1;
    }
    if (*(uint *)(in_ECX + 0x2c) < uVar1) {
      return -1;
    }
    if (*(uint *)(in_ECX + 0x2c) != 0) {
      *(uint *)(in_ECX + 0x28) = uVar1;
    }
  }
  if ((param_3 & 2) == 0) {
    return iVar3;
  }
  if ((*(int *)(in_ECX + 0x20) == 0) && (iVar3 = FUN_00560855(0xffffffff), iVar3 == -1)) {
    return -1;
  }
  if (param_2 == 0) {
    iVar3 = *(int *)(in_ECX + 0x18);
  }
  else if (param_2 == 1) {
    iVar3 = *(int *)(in_ECX + 0x1c);
  }
  else {
    if (param_2 != 2) {
      return -1;
    }
    iVar3 = *(int *)(in_ECX + 0x20);
  }
  uVar1 = iVar3 + param_1;
  iVar3 = uVar1 - *(uint *)(in_ECX + 0x18);
  if (*(uint *)(in_ECX + 0x18) <= uVar1) {
    if (uVar1 <= *(uint *)(in_ECX + 0x20)) {
LAB_005609c8: ;
      if (*(int *)(in_ECX + 0x20) != 0) {
        *(uint *)(in_ECX + 0x1c) = uVar1;
        return iVar3;
      }
      return iVar3;
    }
    if (*(int *)(in_ECX + 0x30) != 0) {
      iVar2 = *(int *)(in_ECX + 0x34);
      if (*(int *)(in_ECX + 0x34) <= iVar3) {
        iVar2 = iVar3;
      }
      *(int *)(in_ECX + 0x34) = iVar2;
      iVar2 = FUN_0056073c();
      if (iVar2 != -1) goto LAB_005609c8;
    }
  }
  return -1;
}
