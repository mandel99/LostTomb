#include "restore_types.h"
#define DAT_005d2c48 (*(undefined4 *)0x005d2c48u)
#define DAT_0077af00 (*(undefined4 *)0x0077af00u)
#define DAT_0077af08 (*(undefined4 *)0x0077af08u)
#define DAT_0077af50 (*(undefined4 *)0x0077af50u)
#define DAT_00bcd677 (*(undefined4 *)0x00bcd677u)

void FUN_00444bd0(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = (uint)(byte)(&DAT_00bcd677)[param_1 * 0x184] * 0x58;
  *(int *)(&DAT_0077af00 + iVar1) = *(int *)(&DAT_0077af00 + iVar1) + 1;
  (&DAT_0077af08)[param_2 + iVar1] = (&DAT_0077af08)[param_2 + iVar1] + '\x01';
  *(int *)(&DAT_0077af50 + iVar1) =
       *(int *)(&DAT_0077af50 + iVar1) + *(int *)(&DAT_005d2c48 + param_2 * 8);
  return;
}
