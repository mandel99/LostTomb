#include "restore_types.h"
undefined4 FUN_0051afa0(int param_1)

{
  int iVar1;
  
  iVar1 = (int)*(short *)(0x00b350dcu + (uint)param_1 * 2u);
  if (0 < iVar1) {
    do {
      iVar1 = iVar1 * 0x184;
      if ((*(undefined1 *)(0x00bcd605u + (uint)iVar1) == '\0') &&
          (*(undefined1 *)(0x00bcd630u + (uint)iVar1) == '\x05')) {
        return 0;
      }
      iVar1 = (int)*(short *)(0x00bcd600u + (uint)iVar1);
    } while (iVar1 != 0);
  }
  return 1;
}
