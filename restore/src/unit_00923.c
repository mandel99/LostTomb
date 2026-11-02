#include "restore_types.h"
#define DAT_005da544 (*(undefined *)0x005da544u)
#define DAT_005da554 (*(undefined4 *)0x005da554u)
#define DAT_0070573c (*(undefined4 *)0x0070573cu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
extern void FUN_00461b20();
extern int FUN_00475b50();

void FUN_00461950(int param_1)

{
  int *piVar1;
  int iVar2;
  bool bVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  int *piVar9;
  short *local_28;
  int local_24;
  int local_20 [4];
  int local_10 [4];
  
  iVar5 = 0;
  do {
    if (iVar5 == 0) {
      local_20[0] = (DAT_005da544 * param_1) / 100;
    }
    else {
      local_20[iVar5] = local_20[iVar5] + ((&DAT_005da544)[iVar5] * param_1) / 100;
    }
    local_20[iVar5 + 4] = 0;
    iVar6 = (&DAT_005da554)[iVar5 * 0xc];
    if (iVar6 != 0) {
      piVar4 = &DAT_005da554 + iVar5 * 0xc;
      do {
        piVar4 = piVar4 + 1;
        local_20[iVar5 + 4] = local_20[iVar5 + 4] + (&DAT_0070573c)[iVar6];
        iVar6 = *piVar4;
      } while (iVar6 != 0);
    }
    if (iVar5 != 3) {
      local_20[iVar5 + 1] = 0;
    }
    iVar6 = local_20[iVar5 + 4];
    iVar2 = local_20[iVar5];
    if (iVar2 < iVar6) {
      local_20[iVar5 + 4] = iVar2;
    }
    else if ((iVar6 < iVar2) && (iVar5 != 3)) {
      local_20[iVar5 + 1] = iVar2 - iVar6;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 4);
  local_24 = 0;
  if (-1 < DAT_0078d790) {
    local_28 = &DAT_007999e8;
    do {
      if ((char)local_28[-8] == '\x01') {
        iVar5 = FUN_00475b50(local_24);
        if ((((&DAT_00799a18)[iVar5 * 0x84] != 0) ||
            (*(char *)((int)&DAT_007999dc + iVar5 * 0x108 + 1) != '\0')) &&
           ((*(char *)((int)&DAT_007999dc + iVar5 * 0x108 + 1) == '\0' ||
            ((&DAT_007999f4)[iVar5 * 0x84] != 0)))) {
          piVar7 = &DAT_005da554;
          bVar3 = true;
          piVar4 = local_20 + 4;
          piVar8 = &DAT_005da554;
          do {
            if (*piVar4 != 0) {
              bVar3 = false;
              iVar5 = *piVar8;
              piVar9 = piVar7;
              piVar1 = piVar8;
              while (iVar5 != 0) {
                if ((int)*local_28 == *piVar1) {
                  *piVar4 = *piVar4 + -1;
                  FUN_00461b20(local_24);
                }
                if (*piVar4 == 0) break;
                piVar1 = piVar9 + 1;
                piVar9 = piVar9 + 1;
                iVar5 = *piVar1;
                piVar1 = piVar9;
              }
            }
            piVar8 = piVar8 + 0xc;
            piVar7 = piVar7 + 0xc;
            piVar4 = piVar4 + 1;
          } while ((int)piVar8 < 0x5da614);
          if (bVar3) {
            return;
          }
        }
      }
      local_24 = local_24 + 1;
      local_28 = local_28 + 0x84;
    } while (local_24 <= DAT_0078d790);
  }
  return;
}
