#include "restore_types.h"
#define DAT_005d2078 (*(undefined4 *)0x005d2078u)
#define DAT_005d2088 (*(undefined4 *)0x005d2088u)
#define DAT_005d2098 (*(undefined4 *)0x005d2098u)
#define DAT_005d20a8 (*(undefined4 *)0x005d20a8u)
#define DAT_005d20b8 (*(undefined4 *)0x005d20b8u)
#define DAT_005d20dc (*(undefined4 *)0x005d20dcu)
#define DAT_005d2100 (*(undefined4 *)0x005d2100u)
#define DAT_005d2124 (*(undefined4 *)0x005d2124u)
#define DAT_005d2148 (*(undefined4 *)0x005d2148u)
#define DAT_005d2188 (*(undefined4 *)0x005d2188u)
#define DAT_005d21c8 (*(undefined4 *)0x005d21c8u)
#define DAT_005d2208 (*(undefined4 *)0x005d2208u)
#define DAT_005d2248 (*(undefined4 *)0x005d2248u)
#define DAT_005d22ac (*(undefined4 *)0x005d22acu)
#define DAT_005d2310 (*(undefined4 *)0x005d2310u)
#define DAT_005d2374 (*(undefined4 *)0x005d2374u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00ee0c50 (*(undefined4 *)0x00ee0c50u)
#define DAT_00f5659c (*(undefined *)0x00f5659cu)
#define DAT_00f565a4 (*(undefined *)0x00f565a4u)
#define DAT_00f565ac (*(undefined *)0x00f565acu)
#define DAT_00f565ec (*(undefined *)0x00f565ecu)
#define DAT_00f565f4 (*(undefined *)0x00f565f4u)
#define DAT_00f566fc (*(undefined *)0x00f566fcu)
#define DAT_00f5679c (*(undefined *)0x00f5679cu)
#define DAT_00f567ac (*(undefined *)0x00f567acu)
#define DAT_00f5695c (*(undefined *)0x00f5695cu)

void FUN_00440060(undefined2 *param_1,int param_2,undefined2 param_3)

{
  byte bVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_14;
  int local_10;
  int local_c;
  undefined2 *local_8;
  int local_4;
  
  local_c = 1;
  if (param_2 == 0x1f) {
    local_c = 2;
  }
  else if (param_2 == 0x1e) {
    local_c = 3;
  }
  else if (param_2 == 0x21) {
    local_c = 4;
  }
  else if (param_2 == 0xd1) {
    local_c = 5;
  }
  iVar8 = 0;
  if (local_c != 0) {
    local_10 = 0;
    iVar7 = (int)param_1 * 4;
    local_8 = (undefined2 *)(&DAT_00b4e6fc + (int)param_1 * 2);
    local_14 = 0;
    iVar6 = 0;
    iVar9 = (int)param_1;
    param_1 = local_8;
    local_4 = iVar7;
    while( true ) {
      do {
        if ((param_2 == 0xd1) ||
           (((((iVar2 = *(int *)((int)&DAT_00b7482c + iVar7), iVar2 != DAT_00f567ac &&
               (iVar2 != DAT_00f566fc)) && (iVar2 != DAT_00f566fc + 1)) &&
             ((iVar2 != DAT_00f566fc + 2 && (iVar2 != DAT_00f566fc + 3)))) &&
            (iVar2 != DAT_00f5659c)))) {
          uVar3 = *(uint *)((int)&DAT_00af598c + iVar7);
          uVar4 = uVar3 & 0x3052ec0;
          *param_1 = param_3;
          bVar1 = (&DAT_00b285cc)[iVar6 + iVar9];
          uVar5 = uVar4 | 8;
          *(uint *)((int)&DAT_00af598c + iVar7) = uVar5;
          (&DAT_00b285cc)[iVar6 + iVar9] = bVar1 & 0x60;
          iVar2 = DAT_00f565f4;
          if (((uVar3 & 0x40) == 0) && (param_2 != 0xd1)) {
            *(uint *)((int)&DAT_00af598c + iVar7) = CONCAT31((int3)(uVar4 >> 8),(char)uVar5) | 0x20;
            *(uint *)((int)&DAT_00b7482c + iVar7) =
                 ((byte)(&DAT_00acf85c)[iVar6 + iVar9] & 3) + iVar2;
          }
          else {
            switch((int)(param_2)) {
            case 0x1e: ;
              if (DAT_00bca2e0 == 0) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565ec + *(int *)(&DAT_005d20b8 + (local_14 + iVar6) * 4);
              }
              else if (DAT_00bca2e0 == 2) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565ec + *(int *)(&DAT_005d20dc + (local_14 + iVar6) * 4);
              }
              else if (DAT_00bca2e0 == 4) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565ec + *(int *)(&DAT_005d2100 + (local_14 + iVar6) * 4);
              }
              else {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565ec + *(int *)(&DAT_005d2124 + (local_14 + iVar6) * 4);
              }
              break;
            case 0x1f: ;
              if (DAT_00bca2e0 == 0) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f5679c + (&DAT_005d2078)[iVar6 + iVar8 * 2];
              }
              else if (DAT_00bca2e0 == 2) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f5679c + (&DAT_005d2088)[iVar6 + iVar8 * 2];
              }
              else {
                iVar2 = iVar6 + iVar8 * 2;
                if (DAT_00bca2e0 == 4) {
                  *(int *)((int)&DAT_00b7482c + iVar7) = DAT_00f5679c + (&DAT_005d2098)[iVar2];
                }
                else {
                  *(int *)((int)&DAT_00b7482c + iVar7) = DAT_00f5679c + (&DAT_005d20a8)[iVar2];
                }
              }
              break;
default: ;
              *(undefined4 *)((int)&DAT_00b7482c + iVar7) = DAT_00f5695c;
              break;
            case 0x21: ;
              if (DAT_00bca2e0 == 0) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565ac + (&DAT_005d2148)[iVar6 + iVar8 * 4];
              }
              else if (DAT_00bca2e0 == 2) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565ac + (&DAT_005d2188)[iVar6 + iVar8 * 4];
              }
              else {
                iVar2 = iVar6 + iVar8 * 4;
                if (DAT_00bca2e0 == 4) {
                  *(int *)((int)&DAT_00b7482c + iVar7) = DAT_00f565ac + (&DAT_005d21c8)[iVar2];
                }
                else {
                  *(int *)((int)&DAT_00b7482c + iVar7) = DAT_00f565ac + (&DAT_005d2208)[iVar2];
                }
              }
              break;
            case 0xd1: ;
              if (DAT_00bca2e0 == 0) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565a4 + *(int *)(&DAT_005d2248 + (local_10 + iVar6) * 4);
              }
              else if (DAT_00bca2e0 == 2) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565a4 + *(int *)(&DAT_005d22ac + (local_10 + iVar6) * 4);
              }
              else if (DAT_00bca2e0 == 4) {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565a4 + *(int *)(&DAT_005d2310 + (local_10 + iVar6) * 4);
              }
              else {
                *(int *)((int)&DAT_00b7482c + iVar7) =
                     DAT_00f565a4 + *(int *)(&DAT_005d2374 + (local_10 + iVar6) * 4);
              }
            }
            *(undefined1 *)((int)&DAT_00ee0c50 + iVar6 + iVar9) = 0;
          }
          (&DAT_00b67d1c)[iVar6 + iVar9] = (&DAT_00b67d1c)[iVar6 + iVar9] | 0x40;
        }
        iVar6 = iVar6 + 1;
        param_1 = param_1 + 1;
        iVar7 = iVar7 + 4;
      } while (iVar6 < local_c);
      iVar8 = iVar8 + 1;
      param_1 = local_8 + 0xe4;
      iVar7 = local_4 + 0x390;
      iVar9 = iVar9 + 0xe4;
      local_14 = local_14 + 3;
      local_10 = local_10 + 5;
      if (local_c <= iVar8) break;
      iVar6 = 0;
      local_8 = param_1;
      local_4 = iVar7;
    }
  }
  return;
}
