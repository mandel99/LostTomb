#include "restore_types.h"
#define DAT_005d2c4c (*(undefined4 *)0x005d2c4cu)
#define DAT_0077af04 (*(undefined4 *)0x0077af04u)
#define DAT_0077af2c (*(undefined4 *)0x0077af2cu)
#define DAT_0077af54 (*(undefined4 *)0x0077af54u)
#define DAT_00bcd677 (*(undefined4 *)0x00bcd677u)

void FUN_00444b80(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = (uint)(byte)(&DAT_00bcd677)[param_1 * 0x184] * 0x58;
  *(int *)(&DAT_0077af04 + iVar1) = *(int *)(&DAT_0077af04 + iVar1) + 1;
  (&DAT_0077af2c)[param_2 + iVar1] = (&DAT_0077af2c)[param_2 + iVar1] + '\x01';
  *(int *)(&DAT_0077af54 + iVar1) =
       *(int *)(&DAT_0077af54 + iVar1) + *(int *)(&DAT_005d2c4c + param_2 * 8);
  return;
}
