#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)

undefined4 FUN_0040eb90(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int in_ECX;
  
  if ((int)(&DAT_00799a30)[param_1 * 0x42] < 3) {
    iVar1 = *(int *)(in_ECX + 0x54 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34) / 2;
    *param_2 = *(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) + iVar1;
    *param_3 = *(short *)(&DAT_007999e0 + param_1 * 0x108) + iVar1;
    return 1;
  }
  iVar1 = (param_1 - (short)(&DAT_007999ea)[param_1 * 0x84]) * 0x108;
  *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
  *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 0xb;
  return 1;
}
