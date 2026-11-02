#include "restore_types.h"
#define DAT_005da2f4 (*(undefined4 *)0x005da2f4u)
#define DAT_0078d590 (*(undefined4 *)0x0078d590u)
#define DAT_0078d76c (*(undefined4 *)0x0078d76cu)
#define DAT_0078d770 (*(undefined4 *)0x0078d770u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

int FUN_0046b9e0(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  short *psVar5;
  undefined4 *puVar6;
  int iVar7;
  
  iVar7 = DAT_0078d770;
  puVar6 = (undefined4 *)&DAT_0078d590;
  for (iVar1 = 0xc; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar6 = 0;
    puVar6 = puVar6 + 1;
  }
  iVar1 = 0;
  iVar7 = iVar7 + DAT_0078d76c * 0xe4 + DAT_00bca2d8;
  if (0 < param_2) {
    piVar4 = &DAT_005da2f4;
    do {
      iVar2 = *piVar4 + iVar7;
      if ((((*(byte *)(&DAT_00af598c + iVar2) & 8) != 0) &&
          (iVar2 = (int)*(short *)(&DAT_00b4e6fc + iVar2 * 2), iVar2 != param_1)) &&
         (*(char *)((int)&DAT_007999dc + iVar2 * 0x108 + 1) != '\0')) {
        iVar1 = iVar1 + (short)(&DAT_007999f4)[iVar2 * 0x84];
        piVar3 = (int *)&DAT_0078d590;
        psVar5 = &DAT_00799a2c + iVar2 * 0x84;
        do {
          *piVar3 = *piVar3 + (int)*psVar5;
          piVar3 = piVar3 + 1;
          psVar5 = psVar5 + 1;
        } while ((int)piVar3 < 0x78d5c0);
        (&DAT_007999f4)[iVar2 * 0x84] = 0;
        (&DAT_007999d8)[iVar2 * 0x108] = 5;
      }
      piVar4 = piVar4 + 1;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
    return iVar1;
  }
  return 0;
}
