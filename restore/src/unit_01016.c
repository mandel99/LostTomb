#include "restore_types.h"
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)

void FUN_0046fda0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined4 *puVar4;
  
  iVar1 = DAT_00bca2d4;
  iVar3 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      if ((int)(&DAT_00bc9f24)[iVar3] < (int)(&DAT_00bc9b94)[iVar3]) {
        puVar4 = &DAT_00b7482c + (&DAT_00bc9804)[iVar3];
        for (iVar2 = (&DAT_00bc9b94)[iVar3] - (&DAT_00bc9f24)[iVar3]; iVar2 != 0; iVar2 = iVar2 + -1
            ) {
          *puVar4 = 6;
          puVar4 = puVar4 + 1;
        }
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < iVar1);
  }
  return;
}
