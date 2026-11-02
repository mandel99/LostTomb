#include "restore_types.h"
#define DAT_006fdd2c (*(undefined4 *)0x006fdd2cu)
#define DAT_006fdd40 (*(undefined4 *)0x006fdd40u)
#define DAT_006ff608 (*(undefined *)0x006ff608u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38eab (*(char *)0x00e38eabu)
#define DAT_00f55c54 (*(undefined *)0x00f55c54u)
#define _DAT_007027a4 (*(undefined4 *)0x007027a4u)
extern undefined4 FUN_004cf620();
extern int FUN_0053e590();

void FUN_00419140(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = (int)DAT_00e38eab;
  if ((((-1 < iVar1) && (iVar1 < DAT_006ff608)) && (iVar1 * 0x40 != -0x6fdd08)) &&
     (iVar3 = 0, 0 < (int)(&DAT_006fdd40)[iVar1 * 0x10])) {
    piVar4 = (int *)(&DAT_006fdd2c + iVar1 * 0x40);
    do {
      iVar6 = *piVar4 + DAT_00e38da4;
      iVar5 = piVar4[-1] + DAT_00e38da8;
      iVar2 = FUN_004cf620(iVar5,iVar6,0x2c,0x2c);
      if (iVar2 == 0) {
        iVar2 = (-(uint)((_DAT_007027a4 & 0xff) - 1 != iVar3) & 0xfffffffe) + 2;
      }
      else {
        iVar2 = 1;
      }
      FUN_0053e590(DAT_00f55c54 + iVar2,iVar5,iVar6);
      iVar3 = iVar3 + 1;
      piVar4 = piVar4 + 3;
    } while (iVar3 < (int)(&DAT_006fdd40)[iVar1 * 0x10]);
  }
  return;
}
