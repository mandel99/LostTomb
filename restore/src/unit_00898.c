#include "restore_types.h"
#define DAT_005d2c48 (*(undefined4 *)0x005d2c48u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea44dc (*(undefined4 *)0x00ea44dcu)

void FUN_0045d4d0(int param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  
  iVar1 = *(int *)(&DAT_005d2c48 + param_3 * 8);
  *(int *)(&DAT_00ea187c + param_1 * 0x49d8) = *(int *)(&DAT_00ea187c + param_1 * 0x49d8) - iVar1;
  (&DAT_00ea44dc)[param_1 * 0x1276] = (&DAT_00ea44dc)[param_1 * 0x1276] + iVar1;
  return;
}
