#include "restore_types.h"
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004b3250();

void FUN_00495d30(int param_1)

{
  int iVar1;
  
  iVar1 = param_1 * 0x184;
  (&DAT_00bcd630)[iVar1] = 9;
  *(undefined2 *)(&DAT_00bcd61c + iVar1) = *(undefined2 *)(&DAT_00bcd624 + iVar1);
  *(undefined2 *)(&DAT_00bcd61e + iVar1) = *(undefined2 *)(&DAT_00bcd626 + iVar1);
  FUN_004b3250(param_1);
  return;
}
