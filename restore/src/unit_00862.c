#include "restore_types.h"
#define DAT_00784ddc (*(undefined *)0x00784ddcu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea44e4 (*(undefined4 *)0x00ea44e4u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004c5120();
extern int FUN_004cde40();

void FUN_0045a350(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  
  uVar3 = DAT_00784ddc;
  if (((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) &&
     (iVar1 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]), iVar1 == 1)) {
    uVar3 = ((int)(uVar3 - 5) < 0) - 1 & uVar3 - 5;
  }
  if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < 0) {
    iVar2 = FUN_004cde40(-*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8),uVar3);
    iVar1 = DAT_00eaac28;
    *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) =
         *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) - iVar2 / 0xc;
    (&DAT_00ea44e4)[iVar1 * 0x1276] = (&DAT_00ea44e4)[iVar1 * 0x1276] + iVar2 / 0xc;
  }
  return;
}
