#include "restore_types.h"
void FUN_004ba040(int param_1)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  
  iVar3 = 0x10;
  psVar2 = (short *)(0x00bcb9d0u + (uint)param_1 * 0x90u);
  do {
    if (0 < *psVar2) {
      iVar1 = *psVar2 * 0x184;
      if ((*(undefined1 *)(0x00bcd630u + (uint)iVar1) != '\x05') &&
          (*(undefined1 *)(0x00bcd630u + (uint)iVar1) != '\x04')) {
        *(undefined1 *)(0x00bcd630u + (uint)iVar1) = 9;
        *(undefined2 *)(0x00bcd62eu + (uint)iVar1) = 0;
      }
    }
    psVar2 = psVar2 + 1;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return;
}
