#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f3878 (*(undefined4 *)0x005f3878u)
#define DAT_005f5498 (*(undefined4 *)0x005f5498u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)

void FUN_004f6420(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  if (DAT_005e4ef4 == 5) {
    iVar1 = *(int *)(&DAT_005f5498 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0x80);
  }
  else {
    iVar1 = *(int *)(&DAT_005f3878 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0x3c0);
  }
  *param_2 = (int)(short)(&DAT_007999fe)[(param_1 + iVar1) * 0x84];
  *param_3 = (int)(short)(&DAT_00799a00)[(param_1 + iVar1) * 0x84];
  return;
}
