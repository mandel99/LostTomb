#include "restore_types.h"
#define DAT_005da450 (*(undefined4 *)0x005da450u)
#define DAT_00ea4398 (*(undefined4 *)0x00ea4398u)
#define DAT_00ea439c (*(undefined4 *)0x00ea439cu)
#define DAT_00ea43a0 (*(undefined4 *)0x00ea43a0u)
#define DAT_00ea43ac (*(undefined4 *)0x00ea43acu)
#define DAT_00ea4464 (*(undefined4 *)0x00ea4464u)
#define DAT_00ea4468 (*(undefined4 *)0x00ea4468u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea4474 (*(undefined4 *)0x00ea4474u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004cde60();

void FUN_00459400(void)

{
  int *piVar1;
  int iVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int iVar10;
  int local_4;
  
  iVar2 = DAT_00eaac28;
  iVar7 = 0;
  iVar10 = DAT_00eaac28 * 0x49d8;
  iVar8 = 10;
  puVar3 = (undefined4 *)(&DAT_00ea43a0 + iVar10);
  do {
    *puVar3 = 0;
    puVar3 = puVar3 + 5;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  piVar9 = &DAT_00ea439c + iVar2 * 0x1276;
  iVar8 = 10;
  do {
    iVar4 = *piVar9;
    piVar9 = piVar9 + 5;
    iVar7 = iVar7 + iVar4;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  (&DAT_00ea4474)[iVar2 * 0x1276] = 0;
  iVar8 = (&DAT_00ea4398)[iVar2 * 0x1276];
  if (iVar8 < iVar7) {
    iVar7 = 1;
    do {
      iVar4 = 0;
      piVar9 = (int *)(&DAT_00ea43ac + iVar10);
      do {
        if (*piVar9 == iVar7) {
          iVar5 = (&DAT_00ea439c)[iVar4 * 5 + iVar2 * 0x1276];
          if (iVar8 < (int)(&DAT_00ea439c)[iVar4 * 5 + iVar2 * 0x1276]) {
            iVar5 = iVar8;
          }
          *(int *)(&DAT_00ea43a0 + iVar10 + iVar4 * 0x14) = iVar5;
          iVar8 = iVar8 - iVar5;
          break;
        }
        iVar4 = iVar4 + 1;
        piVar9 = piVar9 + 5;
      } while (iVar4 < 9);
    } while ((0 < iVar8) && (iVar7 = iVar7 + 1, iVar7 < 10));
    local_4 = 1;
    if (1 < (int)(&DAT_00ea4398)[iVar2 * 0x1276]) {
      while (0 < iVar8) {
        piVar9 = &DAT_005da450;
        do {
          iVar7 = iVar10 + piVar9[-1] * 0x14;
          if (*(int *)(&DAT_00ea43ac + iVar7) == 0) {
            iVar4 = (&DAT_00ea439c)[piVar9[-1] * 5 + iVar2 * 0x1276] -
                    *(int *)(&DAT_00ea43a0 + iVar7);
            if (0 < iVar4) {
              iVar5 = *piVar9;
              if (iVar8 < *piVar9) {
                iVar5 = iVar8;
              }
              if (iVar4 < iVar5) {
                iVar5 = iVar4;
              }
              iVar8 = iVar8 - iVar5;
              *(int *)(&DAT_00ea43a0 + iVar7) = *(int *)(&DAT_00ea43a0 + iVar7) + iVar5;
              if (iVar8 < 1) goto LAB_00459531;
            }
          }
          piVar9 = piVar9 + 2;
        } while ((int)piVar9 < 0x5da498);
        if ((iVar8 < 1) || (local_4 = local_4 + 1, (int)(&DAT_00ea4398)[iVar2 * 0x1276] <= local_4))
        break;
      }
    }
LAB_00459531: ;
    piVar9 = (int *)(&DAT_00ea43a0 + iVar10);
    (&DAT_00ea4464)[iVar2 * 0x1276] = (&DAT_00ea4398)[iVar2 * 0x1276];
    iVar7 = 9;
    do {
      piVar1 = piVar9 + -1;
      iVar8 = *piVar9;
      piVar9 = piVar9 + 5;
      iVar7 = iVar7 + -1;
      (&DAT_00ea4474)[iVar2 * 0x1276] = (&DAT_00ea4474)[iVar2 * 0x1276] + (*piVar1 - iVar8);
    } while (iVar7 != 0);
  }
  else {
    puVar3 = (undefined4 *)(&DAT_00ea43a0 + iVar10);
    iVar8 = 10;
    do {
      *puVar3 = puVar3[-1];
      puVar3 = puVar3 + 5;
      iVar8 = iVar8 + -1;
    } while (iVar8 != 0);
    (&DAT_00ea4464)[iVar2 * 0x1276] = iVar7;
  }
  (&DAT_00ea4468)[iVar2 * 0x1276] =
       (&DAT_00ea4398)[iVar2 * 0x1276] - (&DAT_00ea4464)[iVar2 * 0x1276];
  uVar6 = FUN_004cde60((&DAT_00ea4398)[iVar2 * 0x1276] - (&DAT_00ea4464)[iVar2 * 0x1276],
                       (&DAT_00ea4398)[iVar2 * 0x1276]);
  (&DAT_00ea446c)[DAT_00eaac28 * 0x1276] = uVar6;
  return;
}
