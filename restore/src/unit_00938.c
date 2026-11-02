#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
#define DAT_00f56b84 (*(undefined *)0x00f56b84u)

void FUN_00468790(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int iVar6;
  int local_8;
  
  iVar1 = DAT_00bca2d0;
  iVar3 = DAT_00f56b84 + 0x30;
  if (0 < DAT_00bca2d4) {
    local_8 = DAT_00bca2d4;
    iVar2 = DAT_00bca2d8;
    do {
      if (0 < iVar1) {
        psVar5 = (short *)(&DAT_00b4e6fc + iVar2 * 2);
        iVar6 = iVar1;
        do {
          if ((((((&DAT_00af598c)[iVar2] & 4) == 0) && (((&DAT_00af598c)[iVar2] & 0x100) != 0)) &&
              ((iVar4 = (int)*psVar5, iVar4 == 0 || ((&DAT_007999e8)[iVar4 * 0x84] == 0x8a)))) &&
             ((((&DAT_00ae8e7c)[iVar2] = 0, ((&DAT_00b285cc)[iVar2] & 0x80) == 0 && (iVar4 == 0)) &&
              ((int)(&DAT_00b7482c)[iVar2] < iVar3)))) {
            (&DAT_00b7482c)[iVar2] = (&DAT_00b7482c)[iVar2] + 0x30;
          }
          iVar2 = iVar2 + 1;
          psVar5 = psVar5 + 1;
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
      }
      iVar2 = iVar2 + DAT_00bca2dc;
      local_8 = local_8 + -1;
    } while (local_8 != 0);
  }
  return;
}
