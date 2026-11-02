#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004ef450();
extern uint FUN_004f28e0();

void FUN_004f2960(int param_1)

{
  int iVar1;
  
  iVar1 = (int)*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd620 + param_1 * 0x184) * 2);
  FUN_004ef450(iVar1 - (short)(&DAT_007999ea)[iVar1 * 0x84]);
  iVar1 = FUN_004f28e0(iVar1);
  if (0 < iVar1) {
    iVar1 = iVar1 * 0x184;
    if ((&DAT_00bcd602)[iVar1] == 'P') {
      (&DAT_00bcd630)[iVar1] = 0xc;
    }
    else if ((&DAT_00bcd602)[iVar1] == 'Q') {
      (&DAT_00bcd630)[iVar1] = 0xb;
      *(undefined2 *)(&DAT_00bcd62e + iVar1) = 0;
      return;
    }
    *(undefined2 *)(&DAT_00bcd62e + iVar1) = 0;
  }
  return;
}
