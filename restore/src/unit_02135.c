#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005e4ef8 (*(undefined4 *)0x005e4ef8u)
#define DAT_005f3878 (*(undefined4 *)0x005f3878u)
#define DAT_005f5498 (*(undefined4 *)0x005f5498u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)

undefined4 FUN_004f30b0(int param_1,int *param_2,int *param_3,int *param_4)

{
  short sVar1;
  undefined4 uVar2;
  int iVar3;
  
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  if (DAT_005e4ef4 == 5) {
    iVar3 = (param_1 +
            *(int *)(&DAT_005f5498 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0x80)) *
            0x108;
    *param_3 = (int)*(short *)((int)&DAT_007999dc + iVar3 + 2);
    sVar1 = *(short *)(&DAT_007999e0 + iVar3);
    *param_4 = sVar1 + 2;
    *param_2 = *param_3 + (sVar1 + 2) * 0xe4 + DAT_00bca2d8;
    return 1;
  }
  param_1 = param_1 + *(int *)(&DAT_005f3878 +
                              (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0x3c0);
  uVar2 = *(undefined4 *)
           (&DAT_005e4ef8 +
           ((uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 0x90 +
           (int)(short)(&DAT_007999ea)[param_1 * 0x84]) * 8);
  *param_3 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2);
  iVar3 = (int)*(short *)(&DAT_007999e0 + param_1 * 0x108);
  *param_4 = iVar3;
  switch((int)(uVar2)) {
  case 4: ;
    *param_3 = *param_3 + -1;
    break;
  case 5: ;
    iVar3 = iVar3 + 2;
    goto LAB_004f31b5;
  case 6: ;
    *param_3 = *param_3 + 2;
    break;
  case 7: ;
    iVar3 = iVar3 + -1;
LAB_004f31b5: ;
    *param_4 = iVar3;
  }
  *param_2 = *param_3 + *param_4 * 0xe4 + DAT_00bca2d8;
  return 1;
}
