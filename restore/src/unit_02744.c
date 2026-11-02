#include "restore_types.h"
extern void FUN_005610d4();
extern void * FUN_00561a1d();
extern void FUN_00561a2b();
extern undefined4 * FUN_00563400();

undefined4 FUN_0056073c(void)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  void *pvVar4;
  undefined4 uVar5;
  uint uVar6;
  int in_ECX;
  int iVar7;
  int iVar8;
  uint uVar9;
  uint local_8;
  
  local_8 = *(uint *)(in_ECX + 0x34);
  uVar9 = 1;
  if (1 < (int)local_8) {
    uVar9 = local_8;
  }
  uVar6 = *(uint *)(in_ECX + 0x14);
  uVar1 = *(uint *)(in_ECX + 0x10);
  if (uVar1 < uVar6) {
    iVar3 = uVar6 - uVar1;
  }
  else {
    iVar3 = 0;
  }
  if ((int)local_8 <= (int)(iVar3 + uVar9)) {
    if ((int)local_8 < 2) {
      local_8 = 1;
    }
    if (uVar1 < uVar6) {
      iVar3 = uVar6 - uVar1;
    }
    else {
      iVar3 = 0;
    }
    local_8 = iVar3 + local_8;
  }
  iVar3 = 0;
  if (*(code **)(in_ECX + 0x40) == (code *)0x0) {
    pvVar4 = FUN_00561a1d(local_8);
  }
  else {
    pvVar4 = (void *)(**(code **)(in_ECX + 0x40))();
  }
  if (pvVar4 == (void *)0x0) {
    uVar5 = 0xffffffff;
  }
  else {
    uVar9 = *(uint *)(in_ECX + 0x14);
    uVar6 = *(uint *)(in_ECX + 0x10);
    if (uVar6 < uVar9) {
      iVar8 = uVar9 - uVar6;
    }
    else {
      iVar8 = 0;
    }
    if (iVar8 != 0) {
      if (uVar6 < uVar9) {
        iVar3 = uVar9 - uVar6;
      }
      else {
        iVar3 = 0;
      }
      FUN_00563400(pvVar4,uVar6,iVar3);
      uVar6 = *(uint *)(in_ECX + 0x10);
      iVar3 = (int)pvVar4 - uVar6;
    }
    if (*(code **)(in_ECX + 0x44) == (code *)0x0) {
      FUN_00561a2b(uVar6);
    }
    else {
      (**(code **)(in_ECX + 0x44))();
    }
    FUN_005610d4(pvVar4,local_8 + (int)pvVar4,0);
    if ((iVar3 != 0) && (*(int *)(in_ECX + 0x2c) != 0)) {
      *(int *)(in_ECX + 0x24) = *(int *)(in_ECX + 0x24) + iVar3;
      *(int *)(in_ECX + 0x28) = *(int *)(in_ECX + 0x28) + iVar3;
      *(undefined4 *)(in_ECX + 0xc) = 0xffffffff;
      *(int *)(in_ECX + 0x2c) = *(int *)(in_ECX + 0x2c) + iVar3;
    }
    if (*(int *)(in_ECX + 0x20) != 0) {
      iVar8 = *(int *)(in_ECX + 0x18);
      iVar2 = *(int *)(in_ECX + 0x1c);
      iVar7 = *(int *)(in_ECX + 0x20) + iVar3;
      iVar3 = iVar8 + iVar3;
      *(int *)(in_ECX + 0x20) = iVar7;
      *(int *)(in_ECX + 0x18) = iVar3;
      *(int *)(in_ECX + 0x1c) = iVar3;
      if (iVar7 != 0) {
        *(int *)(in_ECX + 0x1c) = (iVar3 - iVar8) + iVar2;
      }
    }
    uVar5 = 1;
  }
  return uVar5;
}
