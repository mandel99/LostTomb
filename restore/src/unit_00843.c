#include "restore_types.h"
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd65c (*(undefined4 *)0x00bcd65cu)
extern int FUN_004b4800();
extern void FUN_004e9250();

void FUN_00459020(undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  
  iVar1 = FUN_004b4800(param_1,3,param_2,param_3,0);
  iVar1 = iVar1 * 0x184;
  (&DAT_00bcd630)[iVar1] = 8;
  *(undefined2 *)(&DAT_00bcd62e + iVar1) = 0;
  (&DAT_00bcd65c)[iVar1] = (char)param_4;
  FUN_004e9250(param_1,param_4);
  return;
}
