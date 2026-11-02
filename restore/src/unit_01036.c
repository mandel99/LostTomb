#include "restore_types.h"
#define DAT_00acf85c (*(uint8_t *)0x00acf85cu)
#define DAT_00b285cc (*(uint8_t *)0x00b285ccu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00f56b54 (*(uint32_t *)0x00f56b54u)

void FUN_00475b80(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  
  iVar2 = DAT_00bca2d4;
  iVar5 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar3 = (&DAT_00bc9804)[iVar5];
      if ((int)(&DAT_00bc9f24)[iVar5] < (int)(&DAT_00bc9b94)[iVar5]) {
        iVar4 = (&DAT_00bc9b94)[iVar5] - (&DAT_00bc9f24)[iVar5];
        piVar6 = &DAT_00b7482c + iVar3;
        do {
          bVar1 = (&DAT_00acf85c)[iVar3];
          *piVar6 = (bVar1 & 7) + DAT_00f56b54;
          if ((bVar1 & 1) != 0) {
            (&DAT_00b285cc)[iVar3] = (&DAT_00b285cc)[iVar3] | 0x20;
          }
          piVar6 = piVar6 + 1;
          iVar3 = iVar3 + 1;
          iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < iVar2);
  }
  return;
}
