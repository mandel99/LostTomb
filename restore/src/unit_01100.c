#include "restore_types.h"
#define DAT_005ca658 (*(undefined4 *)0x005ca658u)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
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
#define DAT_00f56b94 (*(undefined *)0x00f56b94u)
#define DAT_00f5723c (*(undefined *)0x00f5723cu)
extern undefined4 FUN_0046f930();
extern uint32_t FUN_0047a1c0(int32_t);
extern void FUN_0047db00();
extern int FUN_00484be0();
extern void FUN_00484f00();
extern void FUN_00486600();

void FUN_0047ab10(int param_1,int param_2,int param_3)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar1 = param_2;
  cVar2 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  if (cVar2 != '\0') {
    DAT_00bc9768 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    if (param_3 == 1) {
      uVar3 = (&DAT_00af598c)[DAT_00bc9768] | 0x40000;
    }
    else {
      uVar3 = (&DAT_00af598c)[DAT_00bc9768] & 0xfffbffff;
    }
    iVar4 = param_1 + -1;
    iVar5 = param_2 + -1;
    iVar6 = 3;
    (&DAT_00af598c)[DAT_00bc9768] = uVar3;
    param_2 = 3;
    if (iVar4 < 0) {
      param_2 = param_1 + 2;
      iVar4 = 0;
    }
    else if (DAT_00bca2d0 < param_1 + 2) {
      param_2 = DAT_00bca2d0 - iVar4;
    }
    if (iVar5 < 0) {
      iVar6 = iVar1 + 2;
      iVar5 = 0;
    }
    else if (DAT_00bca2d4 < iVar1 + 2) {
      iVar6 = DAT_00bca2d4 - iVar5;
    }
    cVar2 = FUN_0047a1c0(iVar4);
    DAT_00bc9768 = DAT_00bca2d8 + iVar5 * 0xe4 + iVar4;
    for (DAT_00bc9760 = iVar5; DAT_00bc9760 < iVar6 + iVar5; DAT_00bc9760 = DAT_00bc9760 + 1) {
      DAT_00bc9764 = iVar4;
      if (iVar4 < param_2 + iVar4) {
        do {
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x40000) != 0) &&
             (((&DAT_00af598c)[DAT_00bc9768] & 8) == 0)) {
            FUN_0047db00(0x40000);
            FUN_00484f00();
            FUN_00484be0(&DAT_005ca658,0x30,(&DAT_00acf85c)[DAT_00bc9768],0);
            iVar1 = DAT_00bc9768;
            if (cVar2 == '\0') {
              (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f56b94 + DAT_00bc9740 + DAT_00bc9748;
            }
            else {
              (&DAT_00b7482c)[DAT_00bc9768] = DAT_00f5723c + DAT_00bc9740 + DAT_00bc9748;
            }
            (&DAT_00b285cc)[iVar1] = (&DAT_00b285cc)[iVar1] & 0xf0;
            (&DAT_00b67d1c)[iVar1] = (&DAT_00b67d1c)[iVar1] | 0x40;
            FUN_00486600(iVar1);
          }
          DAT_00bc9764 = DAT_00bc9764 + 1;
          DAT_00bc9768 = DAT_00bc9768 + 1;
        } while (DAT_00bc9764 < param_2 + iVar4);
      }
      DAT_00bc9768 = DAT_00bc9768 + (0xe4 - param_2);
    }
  }
  return;
}
