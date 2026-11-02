#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)

undefined4 FUN_00545a30(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  
  iVar1 = param_1 * 0x108;
  switch((&DAT_007999ea)[param_1 * 0x84]) {
  case 0: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 1: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 2;
    return 1;
  case 2: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 3;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + 1;
    return 1;
  case 3: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + -1;
    return 1;
  case 4: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 2;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + -1;
    return 1;
  case 5: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 1;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + -1;
    return 1;
  case 6: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + 1;
    *param_3 = *(short *)(&DAT_007999e0 + iVar1) + -1;
    return 1;
  case 7: ;
    iVar2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
    break;
  case 8: ;
    *param_2 = *(short *)((int)&DAT_007999dc + iVar1 + 2) + -1;
    *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
    return 1;
default: ;
    iVar2 = (int)*(short *)((int)&DAT_007999dc + iVar1 + 2);
  }
  *param_2 = iVar2;
  *param_3 = (int)*(short *)(&DAT_007999e0 + iVar1);
  return 1;
}
