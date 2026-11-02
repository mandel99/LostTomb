#include "restore_types.h"
#define DAT_005cb9d8 (*(undefined4 *)0x005cb9d8u)
#define DAT_005cbcd8 (*(undefined4 *)0x005cbcd8u)
#define DAT_00705f94 (*(undefined *)0x00705f94u)
#define DAT_007078e4 (*(undefined *)0x007078e4u)
#define DAT_00804000 (*(undefined4 *)0x00804000u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9718 (*(undefined *)0x00bc9718u)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bc9740 (*(undefined *)0x00bc9740u)
#define DAT_00bc9748 (*(undefined *)0x00bc9748u)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00f5686c (*(undefined *)0x00f5686cu)
extern undefined4 FUN_0046f930();
extern void FUN_004746d0();
extern void FUN_0047db00();
extern undefined4 FUN_0047e000();
extern void FUN_0047e750();
extern int FUN_00484be0();
extern void FUN_00484f00();
extern void FUN_0051f780();

void FUN_0047ad20(int param_1,int param_2,int param_3,undefined *param_4)

{
  byte bVar1;
  int iVar2;
  char cVar3;
  undefined *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  
  iVar2 = param_2;
  cVar3 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  iVar6 = DAT_00bca2d8;
  if (cVar3 != '\0') {
    iVar7 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    if (param_3 == 1) {
      puVar4 = (undefined *)((&DAT_00af598c)[iVar7] & 0xc04000);
      if (puVar4 != param_4) {
        if (puVar4 == (undefined *)0x4000) {
          iVar5 = DAT_00705f94 / 2;
        }
        else if ((puVar4 == (undefined *)0x404000) && (param_4 == &DAT_00804000)) {
          iVar5 = DAT_007078e4 / 2;
        }
        else {
          iVar5 = 0;
        }
        DAT_00bc971c = DAT_00bc971c + 1;
        DAT_00bc9718 = DAT_00bc9718 + iVar5;
      }
      (&DAT_00af598c)[iVar7] = (&DAT_00af598c)[iVar7] & 0xff3fbfff | (uint)param_4;
    }
    else {
      (&DAT_00af598c)[iVar7] = (&DAT_00af598c)[iVar7] & 0xff3fbfff;
    }
    iVar5 = param_1 + -1;
    (&DAT_00b285cc)[iVar7] = (&DAT_00b285cc)[iVar7] & 0xef;
    DAT_00bc9760 = param_2 + -1;
    iVar7 = 3;
    param_2 = 3;
    if (iVar5 < 0) {
      param_2 = param_1 + 2;
      iVar5 = 0;
    }
    else if (DAT_00bca2d0 < param_1 + 2) {
      param_2 = DAT_00bca2d0 - iVar5;
    }
    if (DAT_00bc9760 < 0) {
      iVar7 = iVar2 + 2;
      DAT_00bc9760 = 0;
    }
    else if (DAT_00bca2d4 < iVar2 + 2) {
      iVar7 = DAT_00bca2d4 - DAT_00bc9760;
    }
    iVar7 = iVar7 + DAT_00bc9760;
    DAT_00bc9768 = iVar6 + DAT_00bc9760 * 0xe4 + iVar5;
    if (DAT_00bc9760 < iVar7) {
      iVar6 = iVar5;
      do {
        while (DAT_00bc9764 = iVar6, DAT_00bc9764 < param_2 + iVar5) {
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x4000) != 0) &&
             (((&DAT_00af598c)[DAT_00bc9768] & 8) == 0)) {
            FUN_0047db00(0x4000);
            FUN_00484f00();
            FUN_00484be0(&DAT_005cb9d8,0x30,0xffffffff,0);
            iVar6 = DAT_00bc9768;
            bVar1 = (&DAT_00b67d1c)[DAT_00bc9768];
            (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f5686c + DAT_00bc9740 + DAT_00bc9748;
            (&DAT_00b67d1c)[iVar6] = bVar1 | 0x40;
            (&DAT_00b285cc)[iVar6] = (&DAT_00b285cc)[iVar6] & 0xf0;
            iVar6 = FUN_0047e000();
            if (iVar6 == 0) {
              uVar8 = 0;
LAB_0047af50: ;
              FUN_0051f780(uVar8);
            }
            else {
              FUN_0047e750();
              iVar6 = FUN_00484be0(&DAT_005cbcd8,0xf,0xffffffff,0);
              if (iVar6 == 0) {
                uVar8 = 1;
                goto LAB_0047af50;
              }
              (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f5686c + DAT_00bc9740 + DAT_00bc9748;
            }
            FUN_004746d0(DAT_00bc9764,DAT_00bc9760,1);
          }
          DAT_00bc9768 = DAT_00bc9768 + 1;
          iVar6 = DAT_00bc9764 + 1;
        }
        DAT_00bc9760 = DAT_00bc9760 + 1;
        DAT_00bc9768 = DAT_00bc9768 + (0xe4 - param_2);
        iVar6 = iVar5;
      } while (DAT_00bc9760 < iVar7);
    }
  }
  return;
}
