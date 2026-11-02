#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)

undefined4 FUN_0054f8a0(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1 * 0x108;
  switch((&DAT_007999ea)[param_1 * 0x84]) {
  case 0: ;
  case 2: ;
  case 4: ;
  case 0x10: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 1: ;
  case 0xb: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    break;
  case 3: ;
  case 5: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 1;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
  case 6: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 7: ;
  case 0xd: ;
  case 0x17: ;
  case 0x19: ;
  case 0x1d: ;
    *param_2 = (int)*(short *)((int)&DAT_007999dc + iVar1 + 2);
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 8: ;
  case 0x18: ;
  case 0x1a: ;
  case 0x1e: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 1;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 3;
    return 1;
  case 9: ;
    iVar2 = (int)*(short *)((int)&DAT_007999dc + iVar1 + 2);
    break;
  case 10: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 2;
    return 1;
  case 0xc: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 5;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 4;
    return 1;
  case 0xe: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 6;
    return 1;
  case 0xf: ;
  case 0x15: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 1;
    goto LAB_0054fa1c;
  case 0x11: ;
  case 0x13: ;
  case 0x1b: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
LAB_0054fa1c: ;
    *param_2 = iVar2;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
  case 0x12: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
  case 0x14: ;
  case 0x1c: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 0x16: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
default: ;
    return 1;
  }
  *param_2 = iVar2;
  *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 2;
  return 1;
}
