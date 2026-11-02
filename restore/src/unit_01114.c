#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2dc (*(undefined *)0x00bca2dcu)
extern void FUN_0047be30();
extern void FUN_0047e870();

void FUN_0047bd50(int param_1,int param_2,int param_3,int param_4)

{
  int iVar1;
  int iVar2;
  
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (param_2 < 0) {
    param_2 = 0;
  }
  if (DAT_00bca2d0 <= param_3) {
    param_3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= param_4) {
    param_4 = DAT_00bca2d4 + -1;
  }
  iVar2 = DAT_00bca2d0 - param_3;
  DAT_00bc9768 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  while (iVar1 = param_1, DAT_00bc9760 = param_2, param_2 <= param_4) {
    while (DAT_00bc9764 = iVar1, DAT_00bc9764 <= param_3) {
      if (((((&DAT_00af598c)[DAT_00bc9768] & 0x100) != 0) &&
          (((&DAT_00af598c)[DAT_00bc9768] & 0xc) == 0)) &&
         (((&DAT_00b285cc)[DAT_00bc9768] & 0x80) == 0)) {
        FUN_0047e870();
        FUN_0047be30(0);
      }
      DAT_00bc9768 = DAT_00bc9768 + 1;
      iVar1 = DAT_00bc9764 + 1;
    }
    DAT_00bc9768 = DAT_00bc9768 + DAT_00bca2dc + iVar2 + -1 + param_1;
    param_2 = DAT_00bc9760 + 1;
  }
  return;
}
