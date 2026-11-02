#include "restore_types.h"
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea5e40 (*(undefined4 *)0x00ea5e40u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e0cf0();

void FUN_004b9f90(void)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar4 = 0;
  iVar2 = DAT_00eaac28 * 0x1276;
  if (0 < (int)(&DAT_00ea5e40)[DAT_00eaac28 * 0x1276]) {
    pbVar3 = &DAT_00bcd786;
    iVar5 = (&DAT_00ea5e40)[DAT_00eaac28 * 0x1276];
    do {
      iVar6 = iVar5;
      if (pbVar3[4] == 1) {
        if (iVar5 < 1) break;
        bVar1 = *pbVar3;
        if ((bVar1 < 0x2b) || (0x35 < bVar1)) {
          if (bVar1 == 0x39) {
            pbVar3[0x2e] = 4;
          }
          else {
            if ((((bVar1 != 99) && (bVar1 != 0x36)) && (bVar1 != 0x37)) && (bVar1 != 0x38))
            goto LAB_004ba00b;
            pbVar3[0x2e] = 4;
          }
        }
        else {
          pbVar3[0x2e] = 4;
        }
        iVar6 = iVar5 + -1;
        if (iVar4 == 0) {
          iVar4 = *(int *)(pbVar3 + 0x16);
          iVar6 = iVar5 + -1;
        }
      }
LAB_004ba00b: ;
      pbVar3 = pbVar3 + 0x184;
      iVar5 = iVar6;
    } while ((int)pbVar3 < 0xc8ad42);
    (&DAT_00ea5e40)[iVar2] = 0;
    DAT_00e950dc = 1;
    FUN_004e0cf0(0x130,0,iVar4,0,0);
  }
  return;
}
