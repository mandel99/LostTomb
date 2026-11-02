#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00bc971c (*(undefined *)0x00bc971cu)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
extern undefined4 FUN_0046f930();
extern void FUN_004746d0();
extern void FUN_0047be30();
extern void FUN_0047e870();

void FUN_0047b960(int param_1,int param_2,undefined4 param_3,int param_4)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  
  iVar1 = param_2;
  cVar3 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  iVar2 = DAT_00bca2d8;
  if (cVar3 != '\0') {
    iVar4 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    uVar5 = (&DAT_00af598c)[iVar4];
    if ((uVar5 & 0x100) == 0) {
      DAT_00bc971c = DAT_00bc971c + 1;
    }
    if (param_4 == 1) {
      uVar5 = uVar5 | 0x100;
    }
    else {
      uVar5 = uVar5 & 0xfffffeff;
    }
    (&DAT_00af598c)[iVar4] = uVar5;
    iVar7 = param_1 + -1;
    (&DAT_00b285cc)[iVar4] = (&DAT_00b285cc)[iVar4] & 0xef;
    iVar6 = param_2 + -1;
    iVar4 = 3;
    param_2 = 3;
    if (iVar7 < 0) {
      param_2 = param_1 + 2;
      iVar7 = 0;
    }
    else if (DAT_00bca2d0 < param_1 + 2) {
      param_2 = DAT_00bca2d0 - iVar7;
    }
    if (iVar6 < 0) {
      iVar4 = iVar1 + 2;
      iVar6 = 0;
    }
    else if (DAT_00bca2d4 < iVar1 + 2) {
      iVar4 = DAT_00bca2d4 - iVar6;
    }
    DAT_00bc9768 = iVar2 + iVar6 * 0xe4 + iVar7;
    DAT_00bc9760 = iVar6;
    if (iVar6 < iVar4 + iVar6) {
      iVar2 = iVar7;
      do {
        while (DAT_00bc9764 = iVar2, DAT_00bc9764 < param_2 + iVar7) {
          if (((((&DAT_00af598c)[DAT_00bc9768] & 0x100) != 0) &&
              (((&DAT_00af598c)[DAT_00bc9768] & 0xc) == 0)) &&
             (((&DAT_00b285cc)[DAT_00bc9768] & 0x80) == 0)) {
            FUN_0047e870();
            FUN_0047be30(param_3);
          }
          DAT_00bc9768 = DAT_00bc9768 + 1;
          iVar2 = DAT_00bc9764 + 1;
        }
        DAT_00bc9760 = DAT_00bc9760 + 1;
        DAT_00bc9768 = DAT_00bc9768 + (0xe4 - param_2);
        iVar2 = iVar7;
      } while (DAT_00bc9760 < iVar4 + iVar6);
    }
    FUN_004746d0(iVar7,iVar6,param_2);
  }
  return;
}
