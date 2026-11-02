#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f3878 (*(undefined4 *)0x005f3878u)
#define DAT_005f5498 (*(undefined4 *)0x005f5498u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)

void FUN_004f3000(int param_1,int param_2)

{
  int iVar1;
  undefined4 uVar2;
  
  param_2 = param_2 - (short)(&DAT_007999ea)[param_2 * 0x84];
  if (DAT_005e4ef4 == 5) {
    iVar1 = *(int *)(&DAT_005f5498 + (uint)*(byte *)(&DAT_00799a2c + param_2 * 0x84) * 0x80);
  }
  else {
    iVar1 = *(int *)(&DAT_005f3878 + (uint)*(byte *)(&DAT_00799a2c + param_2 * 0x84) * 0x3c0);
  }
  param_2 = param_2 + iVar1;
  param_1 = param_1 * 0x184;
  *(undefined2 *)(&DAT_00bcd61c + param_1) = (&DAT_007999fe)[param_2 * 0x84];
  uVar2 = *(undefined4 *)(&DAT_007999e4 + param_2 * 0x108);
  *(undefined2 *)(&DAT_00bcd61e + param_1) = (&DAT_00799a00)[param_2 * 0x84];
  *(undefined4 *)(&DAT_00bcd620 + param_1) = uVar2;
  *(undefined2 *)(&DAT_00bcd63c + param_1) = 0;
  return;
}
