#include "restore_types.h"
#define DAT_005cbea8 (*(undefined4 *)0x005cbea8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
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
#define DAT_00f56b6c (*(undefined *)0x00f56b6cu)
extern undefined4 FUN_0046f930();
extern void FUN_0047dee0();
extern int FUN_00484be0();
extern void FUN_00484f20();

void FUN_004798e0(int param_1,int param_2,int param_3)

{
  int iVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  byte bVar5;
  int iVar6;
  
  iVar1 = param_2;
  cVar2 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  iVar4 = DAT_00bca2d8;
  if (cVar2 != '\0') {
    iVar3 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    if (param_3 == 1) {
      bVar5 = (&DAT_00b285cc)[iVar3];
      (&DAT_00af598c)[iVar3] = (&DAT_00af598c)[iVar3] | 2;
      bVar5 = bVar5 | 0x80;
    }
    else {
      bVar5 = (&DAT_00b285cc)[iVar3];
      (&DAT_00af598c)[iVar3] = (&DAT_00af598c)[iVar3] & 0xdfcffffd;
      bVar5 = bVar5 & 0x7f;
    }
    (&DAT_00b285cc)[iVar3] = bVar5;
    iVar6 = param_1 + -1;
    DAT_00bc9760 = param_2 + -1;
    iVar3 = 3;
    param_2 = 3;
    if (iVar6 < 0) {
      param_2 = param_1 + 2;
      iVar6 = 0;
    }
    else if (DAT_00bca2d0 < param_1 + 2) {
      param_2 = DAT_00bca2d0 - iVar6;
    }
    if (DAT_00bc9760 < 0) {
      iVar3 = iVar1 + 2;
      DAT_00bc9760 = 0;
    }
    else if (DAT_00bca2d4 < iVar1 + 2) {
      iVar3 = DAT_00bca2d4 - DAT_00bc9760;
    }
    iVar3 = iVar3 + DAT_00bc9760;
    DAT_00bc9768 = iVar4 + DAT_00bc9760 * 0xe4 + iVar6;
    if (DAT_00bc9760 < iVar3) {
      iVar4 = iVar6;
      do {
        while (DAT_00bc9764 = iVar4, DAT_00bc9764 < param_2 + iVar6) {
          if (((*(byte *)(&DAT_00af598c + DAT_00bc9768) & 2) != 0) &&
             (((&DAT_00b285cc)[DAT_00bc9768] & 0x80) != 0)) {
            FUN_0047dee0();
            FUN_00484f20();
            iVar4 = FUN_00484be0(&DAT_005cbea8,0x11,0xffffffff,0);
            if (iVar4 == 0) {
              (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f56b6c;
            }
            else {
              (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f56b6c + DAT_00bc9740 + DAT_00bc9748;
            }
            (&DAT_00b285cc)[DAT_00bc9768] = (&DAT_00b285cc)[DAT_00bc9768] & 0xf0;
            (&DAT_00b67d1c)[DAT_00bc9768] = (&DAT_00b67d1c)[DAT_00bc9768] | 0x40;
          }
          DAT_00bc9768 = DAT_00bc9768 + 1;
          iVar4 = DAT_00bc9764 + 1;
        }
        DAT_00bc9760 = DAT_00bc9760 + 1;
        DAT_00bc9768 = DAT_00bc9768 + (0xe4 - param_2);
        iVar4 = iVar6;
      } while (DAT_00bc9760 < iVar3);
    }
  }
  return;
}
