#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd608 (*(undefined4 *)0x00bcd608u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)

uint FUN_004e0530(int param_1)

{
  int iVar1;
  uint3 uVar5;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char cVar6;
  int iVar7;
  bool bVar8;
  
  iVar7 = param_1 * 0x108;
  uVar5 = (uint3)((uint)param_1 >> 8);
  if ((&DAT_007999e8)[param_1 * 0x84] != 0xde) {
    return (uint)uVar5 << 8;
  }
  iVar1 = *(int *)(&DAT_00799a34 + iVar7);
  if (iVar1 == 0) {
    return (uint)uVar5 << 8;
  }
  iVar2 = iVar1 * 0x184;
  if ((&DAT_00bcd606)[iVar2] == '\0') {
    *(undefined4 *)(&DAT_00799a34 + iVar7) = 0;
    return ((iVar1 * 0x61 & 0x3fffffffU) >> 6) << 8;
  }
  cVar6 = (&DAT_00bcd630)[iVar2];
  if ((cVar6 == '\x06') || (cVar6 == '\a')) {
    cVar6 = (&DAT_00bcd608)[iVar2];
  }
  uVar3 = (uint)(byte)(&DAT_00bcd602)[iVar2];
  if (uVar3 == 0x4f) {
    uVar4 = 0;
    if ((cVar6 != '\t') && (uVar3 = 0, cVar6 != '\n')) goto LAB_004e05e6;
  }
  else {
    if (uVar3 == 0x51) {
      uVar4 = 0;
      if ((cVar6 == '\t') || (cVar6 == '\v')) goto LAB_004e05c9;
      bVar8 = cVar6 == '\n';
      uVar3 = uVar4;
    }
    else {
      uVar3 = uVar3 - 0x55;
      if (uVar3 != 0) goto LAB_004e05e6;
      uVar4 = 0;
      if ((((cVar6 == '\x0f') || (cVar6 == '\x10')) || (cVar6 == '\x1a')) || (cVar6 == '\x1b'))
      goto LAB_004e05c9;
      bVar8 = cVar6 == '\x1c';
    }
    uVar4 = uVar3;
    if (!bVar8) {
LAB_004e05e6: ;
      *(undefined4 *)(&DAT_00799a34 + iVar7) = 0;
      return uVar3 & 0xffffff00;
    }
  }
LAB_004e05c9: ;
  return CONCAT31((int3)(uVar4 >> 8),1);
}
