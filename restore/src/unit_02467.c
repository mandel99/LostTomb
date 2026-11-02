#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00f53ffc (*(undefined1 *)0x00f53ffcu)
#define DAT_00f54118 (*(undefined *)0x00f54118u)
extern undefined4 FUN_00538e30();

undefined4
FUN_00538dc0(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5)

{
  int iVar1;
  
  if (DAT_00f54118 == 0) {
    return 0;
  }
  DAT_00f53ffc = param_5;
  iVar1 = FUN_00538e30(param_1,param_2,param_3,param_4);
  if (iVar1 == 0) {
    DAT_00e38dda = (char)param_5;
    if (DAT_00f54118 == 2) {
      DAT_00e38dca = 1;
    }
    return 0;
  }
  DAT_00e38dda = 0x13;
  return 1;
}
