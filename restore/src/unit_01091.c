#include "restore_types.h"
#define DAT_00ae8e7c (*(uint8_t *)0x00ae8e7cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(uint8_t *)0x00b285ccu)
#define DAT_00b67d1c (*(uint8_t *)0x00b67d1cu)
#define DAT_00bc9760 (*(int32_t *)0x00bc9760u)
#define DAT_00bc9764 (*(int32_t *)0x00bc9764u)
#define DAT_00bc9768 (*(int32_t *)0x00bc9768u)
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00bca2d8 (*(int32_t *)0x00bca2d8u)
#define DAT_00bca2e8 (*(int32_t *)0x00bca2e8u)
#define DAT_00bca2ec (*(int32_t *)0x00bca2ecu)
extern undefined4 FUN_0046f930();
extern void FUN_00486600();

void FUN_00479770(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = param_2;
  cVar3 = FUN_0046f930(DAT_00bca2e8 + param_1,DAT_00bca2ec + param_2);
  iVar2 = DAT_00bca2d8;
  if (cVar3 != '\0') {
    iVar4 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
    if (param_3 == 1) {
      (&DAT_00af598c)[iVar4] = (&DAT_00af598c)[iVar4] | 1;
    }
    else {
      (&DAT_00af598c)[iVar4] = (&DAT_00af598c)[iVar4] & 0xfffffffe;
    }
    iVar5 = param_1 + -1;
    DAT_00bc9760 = param_2 + -1;
    iVar4 = 3;
    param_2 = 3;
    if (iVar5 < 0) {
      param_2 = param_1 + 2;
      iVar5 = 0;
    }
    else if (DAT_00bca2d0 < param_1 + 2) {
      param_2 = DAT_00bca2d0 - iVar5;
    }
    if (DAT_00bc9760 < 0) {
      iVar4 = iVar1 + 2;
      DAT_00bc9760 = 0;
    }
    else if (DAT_00bca2d4 < iVar1 + 2) {
      iVar4 = DAT_00bca2d4 - DAT_00bc9760;
    }
    iVar4 = iVar4 + DAT_00bc9760;
    DAT_00bc9768 = iVar2 + DAT_00bc9760 * 0xe4 + iVar5;
    if (DAT_00bc9760 < iVar4) {
      iVar2 = iVar5;
      do {
        while (DAT_00bc9764 = iVar2, DAT_00bc9764 < param_2 + iVar5) {
          if ((((&DAT_00af598c)[DAT_00bc9768] & 1) != 0) &&
             (((&DAT_00af598c)[DAT_00bc9768] & 0x600) == 0)) {
            FUN_00486600(DAT_00bc9768);
            (&DAT_00b285cc)[DAT_00bc9768] = (&DAT_00b285cc)[DAT_00bc9768] & 0xf0;
            (&DAT_00b67d1c)[DAT_00bc9768] = (&DAT_00b67d1c)[DAT_00bc9768] | 0x40;
            (&DAT_00ae8e7c)[DAT_00bc9768] = 0;
          }
          DAT_00bc9768 = DAT_00bc9768 + 1;
          iVar2 = DAT_00bc9764 + 1;
        }
        DAT_00bc9760 = DAT_00bc9760 + 1;
        DAT_00bc9768 = DAT_00bc9768 + (0xe4 - param_2);
        iVar2 = iVar5;
      } while (DAT_00bc9760 < iVar4);
    }
  }
  return;
}
