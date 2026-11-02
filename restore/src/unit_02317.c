#include "restore_types.h"
undefined4 FUN_0051aff0(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  
  uVar1 = 1;
  iVar3 = (int)*(short *)(0x00b350dcu + (uint)param_1 * 2u);
  if (0 < *(short *)(0x00b350dcu + (uint)param_1 * 2u)) {
    while ((iVar2 = iVar3 * 0x184,
            *(undefined1 *)(0x00bcd605u + (uint)iVar2) != '\x01' ||
            (*(undefined1 *)(0x00bcd630u + (uint)iVar2) != '\x05'))) {
      iVar2 = (int)*(short *)(0x00bcd600u + (uint)iVar2);
      if (iVar3 == iVar2) {
        return 1;
      }
      iVar3 = iVar2;
      if (iVar2 == 0) {
        return uVar1;
      }
    }
    uVar1 = 0;
  }
  return uVar1;
}
