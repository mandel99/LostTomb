#include "restore_types.h"
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9804 (*(undefined4 *)0x00bc9804u)
#define DAT_00bc9b94 (*(undefined4 *)0x00bc9b94u)
#define DAT_00bc9f24 (*(undefined4 *)0x00bc9f24u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00f565f4 (*(undefined *)0x00f565f4u)
extern void FUN_0047bf50();
extern undefined4 FUN_0047cae0();

void FUN_00476530(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  byte *pbVar7;
  int iVar8;
  uint uVar9;
  
  iVar3 = DAT_00bca2d4;
  iVar8 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar2 = (&DAT_00bc9804)[iVar8];
      if ((int)(&DAT_00bc9f24)[iVar8] < (int)(&DAT_00bc9b94)[iVar8]) {
        iVar5 = (&DAT_00bc9b94)[iVar8] - (&DAT_00bc9f24)[iVar8];
        do {
          if ((((&DAT_00af598c)[iVar2] & 0x20) != 0) && (((&DAT_00af598c)[iVar2] & 0x600) == 0)) {
            bVar1 = (&DAT_00b285cc)[iVar2];
            (&DAT_00b7482c)[iVar2] = 0;
            (&DAT_00b285cc)[iVar2] = bVar1 & 0xf0;
            (&DAT_00b67d1c)[iVar2] = (&DAT_00b67d1c)[iVar2] | 0x40;
          }
          iVar2 = iVar2 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < iVar3);
  }
  uVar6 = 0;
  if (0 < iVar3) {
    do {
      uVar9 = (&DAT_00bc9f24)[uVar6];
      iVar8 = (&DAT_00bc9b94)[uVar6];
      if ((int)uVar9 < iVar8) {
        pbVar7 = &DAT_00acf85c + (&DAT_00bc9804)[uVar6];
        iVar2 = (&DAT_00bc9804)[uVar6] * 4;
        do {
          if ((((*(uint *)((int)&DAT_00af598c + iVar2) & 0x20) != 0) &&
              ((*(uint *)((int)&DAT_00af598c + iVar2) & 0x600) == 0)) &&
             (*(int *)((int)&DAT_00b7482c + iVar2) == 0)) {
            iVar3 = FUN_0047cae0(uVar9,uVar6,3,3,0x20,1);
            if (iVar3 == 0) {
              iVar3 = FUN_0047cae0(uVar9,uVar6,2,2,0x20,1);
              if (iVar3 == 0) {
                if ((uVar6 & 1) == 0) {
                  uVar4 = uVar9 & 3;
                  if (uVar4 == 0) {
                    *(int *)((int)&DAT_00b7482c + iVar2) = DAT_00f565f4;
                  }
                  else {
                    if (uVar4 == 1) {
                      iVar3 = DAT_00f565f4 + 1;
                      goto LAB_00476719;
                    }
                    if (uVar4 == 2) {
                      *(int *)((int)&DAT_00b7482c + iVar2) = DAT_00f565f4;
                    }
                    else {
                      *(int *)((int)&DAT_00b7482c + iVar2) = DAT_00f565f4 + 1;
                    }
                  }
                }
                else {
                  uVar4 = uVar9 & 3;
                  if (uVar4 == 0) {
                    iVar3 = DAT_00f565f4 + 2;
LAB_00476719: ;
                    *(int *)((int)&DAT_00b7482c + iVar2) = iVar3;
                  }
                  else if (uVar4 == 1) {
                    *(int *)((int)&DAT_00b7482c + iVar2) = DAT_00f565f4 + 3;
                  }
                  else {
                    if (uVar4 != 2) {
                      iVar3 = DAT_00f565f4 + 3;
                      goto LAB_00476719;
                    }
                    *(int *)((int)&DAT_00b7482c + iVar2) = DAT_00f565f4 + 2;
                  }
                }
              }
              else {
                if ((*pbVar7 & 3) < 2) {
                  iVar3 = DAT_00f565f4 + 6;
                }
                else if ((*pbVar7 & 3) < 3) {
                  iVar3 = DAT_00f565f4 + 5;
                }
                else {
                  iVar3 = DAT_00f565f4 + 4;
                }
                FUN_0047bf50(0,uVar9,uVar6,2,2,iVar3,0x20);
              }
            }
            else {
              FUN_0047bf50(0,uVar9,uVar6,3,3,DAT_00f565f4 + 7,0x20);
            }
          }
          uVar9 = uVar9 + 1;
          pbVar7 = pbVar7 + 1;
          iVar2 = iVar2 + 4;
          iVar3 = DAT_00bca2d4;
        } while ((int)uVar9 < iVar8);
      }
      uVar6 = uVar6 + 1;
    } while ((int)uVar6 < iVar3);
  }
  return;
}
