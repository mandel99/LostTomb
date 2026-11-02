#include "restore_types.h"
uint FUN_00542110(uint param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char cVar5;
  bool bVar6;
  
  iVar1 = *(int *)(0x00799a34u + param_1 * 0x108u);
  if (iVar1 == 0) {
    return param_1 & 0xffffff00;
  }
  iVar2 = iVar1 * 0x184;
  if (*(undefined1 *)(0x00bcd606u + (uint)iVar2) == '\0') {
    *(undefined4 *)(0x00799a34u + param_1 * 0x108u) = 0;
    return ((iVar1 * 0x61 & 0x3fffffffU) >> 6) << 8;
  }
  cVar5 = *(char *)(0x00bcd630u + (uint)iVar2);
  if ((cVar5 == '\x06') || (cVar5 == '\a')) {
    cVar5 = *(char *)(0x00bcd608u + (uint)iVar2);
  }
  uVar3 = (uint)*(byte *)(0x00bcd602u + (uint)iVar2);
  if (uVar3 == 0x4f) {
    uVar4 = 0;
    if ((cVar5 != '\t') && (uVar3 = 0, cVar5 != '\n')) goto LAB_005421ba;
  }
  else {
    if (uVar3 == 0x51) {
      uVar4 = 0;
      if ((cVar5 == '\t') || (cVar5 == '\v')) goto LAB_0054219d;
      bVar6 = cVar5 == '\n';
      uVar3 = uVar4;
    }
    else {
      uVar3 = uVar3 - 0x55;
      if (uVar3 != 0) goto LAB_005421ba;
      uVar4 = 0;
      if ((((cVar5 == '\x0f') || (cVar5 == '\x10')) || (cVar5 == '\x17')) || (cVar5 == '\x18'))
      goto LAB_0054219d;
      bVar6 = cVar5 == '\x19';
    }
    uVar4 = uVar3;
    if (!bVar6) {
LAB_005421ba: ;
      *(undefined4 *)(0x00799a34u + param_1 * 0x108u) = 0;
      return uVar3 & 0xffffff00;
    }
  }
LAB_0054219d: ;
  return CONCAT31((int3)(uVar4 >> 8),1);
}
