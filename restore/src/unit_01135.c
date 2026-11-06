#include "restore_types.h"
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)

void FUN_0047d720(void)

{
  byte bVar1;
  byte bVar2;
  int iVar3;
  byte bVar4;
  byte bVar5;
  int iVar6;
  int iVar7;
  byte bVar8;
  int iVar9;
  bool bVar10;
  
  iVar3 = DAT_00e94e04;
  iVar9 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar7 = (&DAT_00bc9804)[iVar9];
      if ((int)(&DAT_00bc9f24)[iVar9] < (int)(&DAT_00bc9b94)[iVar9]) {
        iVar6 = (&DAT_00bc9b94)[iVar9] - (&DAT_00bc9f24)[iVar9];
        do {
          bVar4 = (&DAT_00b285cc)[iVar7] & 0xf;
          bVar5 = (&DAT_00b67d1c)[iVar7];
          bVar1 = bVar5 & 0x3f;
          bVar2 = bVar5 & 0x3f;
          bVar8 = bVar5 & 0x3f;
          if (((&DAT_00b285cc)[iVar7] & 0xf) == 0) goto LAB_0047d84c;
          bVar5 = bVar5 & 0xbf;
          (&DAT_00b67d1c)[iVar7] = bVar5;
          if (bVar4 == 1) {
            if (iVar3 == 0) {
              bVar10 = bVar8 == 8;
            }
            else {
              if (iVar3 != 6) {
                if (iVar3 != 4) goto LAB_0047d810;
                bVar10 = bVar8 == 1;
LAB_0047d80c: ;
                if (bVar10) goto LAB_0047d84c;
                goto LAB_0047d854;
              }
              bVar10 = bVar8 == 9;
            }
LAB_0047d817: ;
            if (bVar10) {
LAB_0047d84c: ;
              (&DAT_00b67d1c)[iVar7] = bVar5 | 0x40;
            }
          }
          else {
            if (bVar4 == 2) {
              if (iVar3 == 0) {
                bVar10 = bVar8 == 0x10;
              }
              else {
                if (iVar3 != 6) {
                  if (iVar3 != 4) goto LAB_0047d810;
                  bVar10 = bVar8 == 2;
                  goto LAB_0047d80c;
                }
                bVar10 = bVar8 == 0x12;
              }
              goto LAB_0047d817;
            }
            if (bVar4 == 3) {
              if (iVar3 == 0) {
                bVar10 = bVar8 == 0x18;
              }
              else if (iVar3 == 6) {
                bVar10 = bVar8 == 0x1b;
              }
              else {
                if (iVar3 != 4) goto LAB_0047d810;
                bVar10 = bVar8 == 3;
              }
              goto LAB_0047d817;
            }
            if (bVar4 != 4) {
              if (bVar4 != 5) goto LAB_0047d854;
              if (iVar3 == 0) {
                if (bVar8 == 0x28) goto LAB_0047d84c;
                goto LAB_0047d854;
              }
              if (iVar3 == 6) {
                if (bVar8 == 0x2d) goto LAB_0047d84c;
                goto LAB_0047d854;
              }
              if (iVar3 != 4) {
                if ((iVar3 == 2) && (bVar2 == 0)) goto LAB_0047d84c;
                goto LAB_0047d854;
              }
              if (bVar8 != 5) goto LAB_0047d854;
              goto LAB_0047d84c;
            }
            if (iVar3 == 0) {
              if (bVar8 == 0x20) goto LAB_0047d84c;
            }
            else if (iVar3 == 6) {
              if (bVar8 == 0x24) goto LAB_0047d84c;
            }
            else {
              if (iVar3 == 4) {
                bVar10 = bVar8 == 4;
                goto LAB_0047d80c;
              }
LAB_0047d810: ;
              if (iVar3 == 2) {
                bVar10 = bVar1 == 0;
                goto LAB_0047d817;
              }
            }
          }
LAB_0047d854: ;
          iVar7 = iVar7 + 1;
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
      }
      iVar9 = iVar9 + 1;
    } while (iVar9 < DAT_00bca2d4);
  }
  return;
}
