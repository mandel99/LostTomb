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
#define DAT_00f56b7c (*(undefined *)0x00f56b7cu)
#define DAT_00f56c8c (*(undefined *)0x00f56c8cu)
#define DAT_00f572ec (*(undefined *)0x00f572ecu)
extern int FUN_00475fd0();
extern void FUN_0047bf50();
extern undefined4 FUN_0047cae0();

void FUN_00475dc0(void)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  int local_8;
  
  iVar5 = DAT_00bca2d4;
  iVar7 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar3 = (&DAT_00bc9804)[iVar7];
      if ((int)(&DAT_00bc9f24)[iVar7] < (int)(&DAT_00bc9b94)[iVar7]) {
        iVar6 = (&DAT_00bc9b94)[iVar7] - (&DAT_00bc9f24)[iVar7];
        do {
          if (((((&DAT_00af598c)[iVar3] & 2) != 0) &&
              (bVar1 = (&DAT_00b285cc)[iVar3], (bVar1 & 0x80) == 0)) &&
             (((&DAT_00af598c)[iVar3] & 0x600) == 0)) {
            (&DAT_00b7482c)[iVar3] = 0;
            (&DAT_00b285cc)[iVar3] = bVar1 & 0xf0;
            (&DAT_00b67d1c)[iVar3] = (&DAT_00b67d1c)[iVar3] | 0x40;
          }
          iVar3 = iVar3 + 1;
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < iVar5);
  }
  local_8 = 0;
  if (0 < iVar5) {
    do {
      iVar3 = (&DAT_00bc9b94)[local_8];
      iVar6 = (&DAT_00bc9804)[local_8];
      iVar4 = DAT_00f572ec;
      for (iVar7 = (&DAT_00bc9f24)[local_8]; DAT_00f572ec = iVar4, iVar7 < iVar3; iVar7 = iVar7 + 1)
      {
        uVar2 = (&DAT_00af598c)[iVar6];
        if ((((uVar2 & 2) != 0) && (((&DAT_00b285cc)[iVar6] & 0x80) == 0)) &&
           (((uVar2 & 0x600) == 0 && ((&DAT_00b7482c)[iVar6] == 0)))) {
          if ((uVar2 & 0x100002) == 0x100002) {
            uVar8 = 0x100002;
            iVar5 = DAT_00f56c8c;
          }
          else {
            if ((uVar2 & 0x20000002) == 0x20000002) {
              iVar5 = FUN_00475fd0(iVar6);
              if (iVar5 == -1) {
                (&DAT_00b7482c)[iVar6] = ((byte)(&DAT_00acf85c)[iVar6] & 7) + DAT_00f56b7c;
              }
              else {
                (&DAT_00b7482c)[iVar6] = iVar5 + iVar4;
              }
              goto LAB_00475f99;
            }
            uVar8 = 2;
            iVar5 = DAT_00f56b7c;
          }
          iVar4 = FUN_0047cae0(iVar7,local_8,3,3,uVar8,1);
          if (iVar4 == 0) {
            iVar4 = FUN_0047cae0(iVar7,local_8,2,2,uVar8,1);
            if (iVar4 == 0) {
              (&DAT_00b7482c)[iVar6] = ((byte)(&DAT_00acf85c)[iVar6] & 7) + iVar5;
            }
            else {
              FUN_0047bf50(0,iVar7,local_8,2,2,((byte)(&DAT_00acf85c)[iVar6] & 3) + 8 + iVar5,uVar8)
              ;
            }
          }
          else {
            FUN_0047bf50(0,iVar7,local_8,3,3,((byte)(&DAT_00acf85c)[iVar6] & 1) + 0xc + iVar5,uVar8)
            ;
          }
        }
LAB_00475f99: ;
        iVar6 = iVar6 + 1;
        iVar5 = DAT_00bca2d4;
        iVar4 = DAT_00f572ec;
      }
      local_8 = local_8 + 1;
    } while (local_8 < iVar5);
  }
  return;
}
