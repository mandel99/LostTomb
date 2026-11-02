#include "restore_types.h"
#define DAT_00a1126c (*(undefined4 *)0x00a1126cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)

void FUN_00485830(void)

{
  int iVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = DAT_00bca2d4;
  iVar5 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      if ((int)(&DAT_00bc9f24)[iVar5] < (int)(&DAT_00bc9b94)[iVar5]) {
        psVar3 = (short *)(&DAT_00b4e6fc + (&DAT_00bc9804)[iVar5] * 2);
        iVar4 = (&DAT_00bc9804)[iVar5] << 2;
        iVar2 = (&DAT_00bc9b94)[iVar5] - (&DAT_00bc9f24)[iVar5];
        do {
          if (*psVar3 == 0) {
            *(undefined4 *)((int)&DAT_00b7482c + iVar4) = *(undefined4 *)(&DAT_00a1126c + iVar4);
          }
          psVar3 = psVar3 + 1;
          iVar4 = iVar4 + 4;
          iVar2 = iVar2 + -1;
        } while (iVar2 != 0);
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < iVar1);
  }
  return;
}
