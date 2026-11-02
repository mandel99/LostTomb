#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd632 (*(undefined4 *)0x00bcd632u)
#define DAT_00bcd634 (*(undefined4 *)0x00bcd634u)
#define DAT_00bcd636 (*(undefined4 *)0x00bcd636u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)

undefined4 FUN_004f2ba0(int param_1)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  short *psVar6;
  
  param_1 = param_1 * 0x184;
  if (((0 < *(short *)(&DAT_00bcd632 + param_1)) &&
      (iVar1 = (int)*(short *)(&DAT_00b4e6fc + *(int *)(&DAT_00bcd620 + param_1) * 2), iVar1 != 0))
     && ((&DAT_007999e8)[iVar1 * 0x84] == 0xb7)) {
    iVar1 = iVar1 - (short)(&DAT_007999ea)[iVar1 * 0x84];
    psVar6 = &DAT_00799a3c + iVar1 * 0x84;
    iVar5 = iVar1;
    while( true ) {
      if (DAT_005e4ef4 == 5) {
        iVar2 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar1 * 0x84) * 4);
      }
      else {
        iVar2 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar1 * 0x84) * 4);
      }
      if (iVar2 + iVar1 <= iVar5) break;
      iVar2 = *psVar6 * 0x184;
      if ((((((&DAT_00bcd602)[iVar2] == 'Q') && ((&DAT_00bcd630)[iVar2] == '\n')) ||
           (((&DAT_00bcd602)[iVar2] == 'P' && ((&DAT_00bcd630)[iVar2] == '\v')))) &&
          ((*(short *)(&DAT_00bcd660 + iVar2) != 0 &&
           (iVar2 = *(short *)(&DAT_00bcd660 + iVar2) * 0x184, (&DAT_00bcd602)[iVar2] == 'U')))) &&
         (((&DAT_00bcd630)[iVar2] == '\x11' &&
          ((0 < *(short *)(&DAT_00bcd632 + iVar2) &&
           (uVar3 = ((int)*(short *)(&DAT_00bcd636 + iVar2) - (int)*(short *)(&DAT_00bcd634 + iVar2)
                    ) - ((int)*(short *)(&DAT_00bcd636 + param_1) -
                        (int)*(short *)(&DAT_00bcd634 + param_1)), uVar4 = (int)uVar3 >> 0x1f,
           (int)((uVar3 ^ uVar4) - uVar4) < 7)))))) {
        return 0;
      }
      iVar5 = iVar5 + 1;
      psVar6 = psVar6 + 0x84;
    }
  }
  return 1;
}
