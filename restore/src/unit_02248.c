#include "restore_types.h"
#define DAT_00799ab8 (*(undefined4 *)0x00799ab8u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f53e6c (*(undefined *)0x00f53e6cu)
#define DAT_00f56494 (*(undefined *)0x00f56494u)
extern int FUN_004cde40();
extern int FUN_004cde60();
extern int FUN_004cde80();
extern undefined4 FUN_00502780();
extern int FUN_0053e590();

void FUN_00502840(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  
  iVar2 = FUN_00502780();
  iVar2 = DAT_00f53e6c - iVar2;
  if (0 < iVar2) {
    iVar3 = FUN_00502780(3);
    iVar3 = FUN_004cde80(iVar3 << 2);
    if ((int)(&DAT_00799ab8)[DAT_00eadd6c * 0x42] < 2) {
      uVar4 = 0;
    }
    else {
      iVar1 = (&DAT_00799ab8)[DAT_00eadd6c * 0x42];
      iVar5 = FUN_00502780();
      if (DAT_00f53e6c < iVar5 + iVar1) {
        uVar4 = 100;
      }
      else {
        uVar4 = FUN_004cde60(iVar1 + -1,iVar2);
      }
    }
    iVar2 = FUN_004cde40((iVar3 + -6) * 0x10,uVar4);
    FUN_0053e590(DAT_00f56494 + 0x27,DAT_00eadd90 + 0x19e,iVar2 + 0x6a);
  }
  return;
}
