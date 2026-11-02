#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bc9760 (*(undefined *)0x00bc9760u)
#define DAT_00bc9764 (*(undefined *)0x00bc9764u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern void FUN_0047b670();
extern void FUN_0047db00();
extern void FUN_0047e5c0();

void FUN_0047b480(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  
  param_1 = param_1 + -1;
  DAT_00bc9760 = param_2 + -1;
  if (param_1 < 0) {
    iVar2 = param_1 + param_3;
    param_1 = 0;
  }
  else {
    iVar2 = param_3;
    if (DAT_00bca2d0 < param_1 + param_3) {
      iVar2 = DAT_00bca2d0 - param_1;
    }
  }
  if (DAT_00bc9760 < 0) {
    param_3 = DAT_00bc9760 + param_3;
    DAT_00bc9760 = 0;
  }
  else if (DAT_00bca2d4 < DAT_00bc9760 + param_3) {
    param_3 = DAT_00bca2d4 - DAT_00bc9760;
  }
  param_3 = param_3 + DAT_00bc9760;
  DAT_00bc9768 = DAT_00bca2d8 + DAT_00bc9760 * 0xe4 + param_1;
  if (DAT_00bc9760 < param_3) {
    iVar1 = param_1;
    do {
      while (DAT_00bc9764 = iVar1, DAT_00bc9764 < param_1 + iVar2) {
        if ((((&DAT_00af598c)[DAT_00bc9768] & 0x40) != 0) &&
           (((&DAT_00af598c)[DAT_00bc9768] & 0xc) == 0)) {
          FUN_0047db00(0x8000040);
          FUN_0047e5c0();
          FUN_0047b670();
        }
        DAT_00bc9768 = DAT_00bc9768 + 1;
        iVar1 = DAT_00bc9764 + 1;
      }
      DAT_00bc9760 = DAT_00bc9760 + 1;
      DAT_00bc9768 = DAT_00bc9768 + (0xe4 - iVar2);
      iVar1 = param_1;
    } while (DAT_00bc9760 < param_3);
  }
  return;
}
