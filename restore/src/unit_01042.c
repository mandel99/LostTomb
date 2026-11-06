#include "restore_types.h"
#define DAT_00ae8e7c (*(undefined4 *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00f565f4 (*(undefined *)0x00f565f4u)

void FUN_00476750(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  iVar2 = DAT_00f565f4;
  iVar5 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar3 = (&DAT_00bc9804)[iVar5];
      if ((int)(&DAT_00bc9f24)[iVar5] < (int)(&DAT_00bc9b94)[iVar5]) {
        iVar4 = (&DAT_00bc9b94)[iVar5] - (&DAT_00bc9f24)[iVar5];
        do {
          if ((iVar2 <= (int)(&DAT_00b7482c)[iVar3]) && ((int)(&DAT_00b7482c)[iVar3] <= iVar2 + 7))
          {
            bVar1 = (&DAT_00b285cc)[iVar3];
            (&DAT_00af598c)[iVar3] = (&DAT_00af598c)[iVar3] | 0x20;
            (&DAT_00b285cc)[iVar3] = bVar1 & 0x7f;
            (&DAT_00ae8e7c)[iVar3] = 0;
          }
          iVar3 = iVar3 + 1;
          iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < DAT_00bca2d4);
  }
  return;
}
