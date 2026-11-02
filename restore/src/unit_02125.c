#include "restore_types.h"
uint FUN_004f26a0(int param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  uVar1 = CONCAT31((int3)((uint)param_1 >> 8),*(undefined1 *)(0x00bcd606u + (uint)iVar2));
  if ((*(undefined1 *)(0x00bcd606u + (uint)iVar2) != '\0') &&
      (*(undefined1 *)(0x00bcd602u + (uint)iVar2) == 'U')) {
    uVar1 = (uint)*(byte *)(0x00bcd630u + (uint)iVar2);
    if ((uVar1 == 6) || (uVar1 == 7)) {
      uVar1 = (uint)*(byte *)(0x00bcd608u + (uint)iVar2);
    }
    if (((((uVar1 == 0xf) || (uVar1 == 0x10)) || (uVar1 == 0x11)) ||
        ((uVar1 == 0x1e || (uVar1 == 0x12)))) ||
       ((uVar1 == 0x13 || ((uVar1 == 0x1d || (uVar1 == 0x14)))))) {
      uVar1 = (uint)*(short *)(0x00bcd660u + (uint)iVar2);
      if (uVar1 == param_2) {
        return CONCAT31((int3)(char)((ushort)*(short *)(0x00bcd660u + (uint)iVar2) >> 8),1);
      }
    }
  }
  return uVar1 & 0xffffff00;
}
