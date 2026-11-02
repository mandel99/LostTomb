#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)

void FUN_004771c0(void)

{
  int iVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  
  iVar1 = DAT_00bca2d4;
  iVar4 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      if ((int)(&DAT_00bc9f24)[iVar4] < (int)(&DAT_00bc9b94)[iVar4]) {
        iVar2 = (&DAT_00bc9b94)[iVar4] - (&DAT_00bc9f24)[iVar4];
        puVar3 = &DAT_00af598c + (&DAT_00bc9804)[iVar4];
        do {
          if ((*puVar3 & 0x10) != 0) {
            *puVar3 = *puVar3 & 0xffffffef | 1;
          }
          puVar3 = puVar3 + 1;
          iVar2 = iVar2 + -1;
        } while (iVar2 != 0);
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < iVar1);
  }
  return;
}
