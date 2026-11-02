#include "restore_types.h"
#define DAT_00978d9c (*(undefined4 *)0x00978d9cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca344 (*(undefined *)0x00bca344u)

void FUN_0047e290(void)

{
  int iVar1;
  uint uVar2;
  byte bVar3;
  uint uVar4;
  int iVar5;
  uint *puVar6;
  int local_4;
  
  DAT_00bca344 = DAT_00bca344 + 1;
  if (0x13 < DAT_00bca344) {
    DAT_00bca344 = 0;
    local_4 = 0;
    if (0 < DAT_00bca2d4) {
      do {
        if ((int)(&DAT_00bc9f24)[local_4] < (int)(&DAT_00bc9b94)[local_4]) {
          iVar1 = (&DAT_00bc9b94)[local_4] - (&DAT_00bc9f24)[local_4];
          puVar6 = &DAT_00978d9c + (&DAT_00bc9804)[local_4];
          do {
            uVar4 = *puVar6;
            if (uVar4 != 0) {
              bVar3 = 0;
              iVar5 = 10;
              do {
                uVar2 = uVar4 >> (bVar3 & 0x1f) & 7;
                if (uVar2 != 0) {
                  uVar4 = (7 << (bVar3 & 0x1f) ^ 0xffffU) & uVar4 | uVar2 - 1 << (bVar3 & 0x1f);
                }
                bVar3 = bVar3 + 3;
                iVar5 = iVar5 + -1;
              } while (iVar5 != 0);
            }
            *puVar6 = uVar4;
            puVar6 = puVar6 + 1;
            iVar1 = iVar1 + -1;
          } while (iVar1 != 0);
        }
        local_4 = local_4 + 1;
      } while (local_4 < DAT_00bca2d4);
    }
  }
  return;
}
