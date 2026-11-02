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
#define DAT_00f56ba4 (*(undefined *)0x00f56ba4u)
extern void FUN_0047bf50();
extern undefined4 FUN_0047cae0();
extern undefined4 FUN_00482650();

void FUN_00477210(void)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar2 = (&DAT_00bc9804)[iVar6];
      if ((int)(&DAT_00bc9f24)[iVar6] < (int)(&DAT_00bc9b94)[iVar6]) {
        iVar4 = (&DAT_00bc9b94)[iVar6] - (&DAT_00bc9f24)[iVar6];
        do {
          if (((((&DAT_00af598c)[iVar2] & 0x2000000) != 0) &&
              (bVar1 = (&DAT_00b285cc)[iVar2], (bVar1 & 0x80) == 0)) &&
             (((&DAT_00af598c)[iVar2] & 0x600) == 0)) {
            (&DAT_00b7482c)[iVar2] = 0;
            (&DAT_00b285cc)[iVar2] = bVar1 & 0xf0;
            (&DAT_00b67d1c)[iVar2] = (&DAT_00b67d1c)[iVar2] | 0x40;
          }
          iVar2 = iVar2 + 1;
          iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < DAT_00bca2d4);
  }
  iVar6 = 0;
  if (0 < DAT_00bca2d4) {
    do {
      iVar4 = (&DAT_00bc9b94)[iVar6];
      iVar5 = (&DAT_00bc9804)[iVar6];
      for (iVar2 = (&DAT_00bc9f24)[iVar6]; iVar2 < iVar4; iVar2 = iVar2 + 1) {
        if (((((&DAT_00af598c)[iVar5] & 0x2000000) != 0) && (((&DAT_00b285cc)[iVar5] & 0x80) == 0))
           && ((((&DAT_00af598c)[iVar5] & 0x600) == 0 && ((&DAT_00b7482c)[iVar5] == 0)))) {
          iVar3 = FUN_0047cae0(iVar2,iVar6,3,3,0x2000000,1);
          if (iVar3 == 0) {
            iVar3 = FUN_0047cae0(iVar2,iVar6,2,2,0x2000000,1);
            if (iVar3 == 0) {
              iVar3 = FUN_00482650(iVar2,iVar6,1,4,0x200);
              if (iVar3 == 0) {
                (&DAT_00b7482c)[iVar5] = ((byte)(&DAT_00acf85c)[iVar5] & 7) + DAT_00f56ba4;
              }
              else {
                (&DAT_00b7482c)[iVar5] = ((byte)(&DAT_00acf85c)[iVar5] & 7) + DAT_00f56ba4;
              }
            }
            else {
              iVar3 = FUN_00482650(iVar2,iVar6,2,4,0x200);
              if (iVar3 == 0) {
                bVar1 = (&DAT_00acf85c)[iVar5];
              }
              else {
                bVar1 = (&DAT_00acf85c)[iVar5];
              }
              FUN_0047bf50(0,iVar2,iVar6,2,2,(bVar1 & 3) + 8 + DAT_00f56ba4,0x2000000);
            }
          }
          else {
            iVar3 = FUN_00482650(iVar2,iVar6,3,4,0x200);
            if (iVar3 == 0) {
              bVar1 = (&DAT_00acf85c)[iVar5];
            }
            else {
              bVar1 = (&DAT_00acf85c)[iVar5];
            }
            FUN_0047bf50(0,iVar2,iVar6,3,3,(bVar1 & 1) + 0xc + DAT_00f56ba4,0x2000000);
          }
        }
        iVar5 = iVar5 + 1;
      }
      iVar6 = iVar6 + 1;
    } while (iVar6 < DAT_00bca2d4);
  }
  return;
}
