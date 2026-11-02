#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)

undefined4 FUN_00556230(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1 * 0x108;
  switch((&DAT_007999ea)[param_1 * 0x84]) {
  case 0: ;
  case 2: ;
  case 4: ;
  case 8: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 1;
    break;
  case 1: ;
  case 7: ;
  case 9: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + -1;
    return 1;
  case 3: ;
  case 0x13: ;
    *param_2 = (int)*(short *)((int)&DAT_007999dc + iVar1 + 2);
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + -1;
    return 1;
  case 5: ;
  case 0xb: ;
  case 0xf: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 1;
    goto LAB_00556331;
  case 6: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 10: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    break;
  case 0xc: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
  case 0xd: ;
  case 0x11: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
LAB_00556331: ;
    *param_2 = iVar2;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
  case 0xe: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
  case 0x10: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 0x12: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 0x14: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    break;
default: ;
    goto switchD_0055624e_default;
  }
  *param_2 = iVar2;
  *param_3 = *(short *)(&DAT_007999e0 + iVar1) + -1;
switchD_0055624e_default: ;
  return 1;
}
