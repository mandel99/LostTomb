#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00ea1870 (*(undefined4 *)0x00ea1870u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea44fc (*(undefined4 *)0x00ea44fcu)

void FUN_004e9680(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = param_1 * 0x49d8;
  if ((*(char *)((int)&DAT_00ea1870 + iVar1 + 2) == '\x01') && (DAT_00605a3c != 1)) {
    *(int *)(&DAT_00ea187c + iVar1) = *(int *)(&DAT_00ea187c + iVar1) - param_2;
    (&DAT_00ea44fc)[param_1 * 0x1276] = (&DAT_00ea44fc)[param_1 * 0x1276] + param_2;
  }
  return;
}
