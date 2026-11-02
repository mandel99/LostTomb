#include "restore_types.h"
undefined4 FUN_00542f40(int param_1,int *param_2,int *param_3)

{
  int iVar1;
  int iVar2;
  short direction;
  
  iVar1 = param_1 * 0x108;
  direction = *(short *)(0x007999eau + (uint)iVar1);
  switch(*(int *)(0x00601de4u + (uint)(int)direction * 0x34u)) {
  case 0: ;
    *param_2 = *(short *)(0x007999deu + (uint)iVar1) + 2;
    *param_3 = *(short *)(0x007999e0u + (uint)iVar1) + 5;
    return 1;
  case 1: ;
    *param_2 = *(short *)(0x007999deu + (uint)iVar1) + 1;
    *param_3 = *(short *)(0x007999e0u + (uint)iVar1) + 2;
    return 1;
  case 2: ;
    *param_2 = *(short *)(0x007999deu + (uint)iVar1) + 1;
    *param_3 = *(short *)(0x007999e0u + (uint)iVar1) + 1;
    return 1;
  case 3: ;
    *param_2 = *(short *)(0x007999deu + (uint)iVar1) + -1;
    *param_3 = *(short *)(0x007999e0u + (uint)iVar1) + 1;
    return 1;
  case 4: ;
    *param_2 = *(short *)(0x007999deu + (uint)iVar1) + -1;
    *param_3 = *(short *)(0x007999e0u + (uint)iVar1) + -1;
    return 1;
  case 5: ;
    *param_2 = *(short *)(0x007999deu + (uint)iVar1) + 1;
    *param_3 = *(short *)(0x007999e0u + (uint)iVar1) + -1;
    return 1;
  case 6: ;
    if ((direction & 1) == 0) {
      iVar2 = *(short *)(0x007999deu + (uint)iVar1) + 1;
      goto LAB_00543093;
    }
    iVar2 = *(short *)(0x007999deu + (uint)iVar1) + -1;
    break;
  case 7: ;
  case 8: ;
  case 9: ;
  case 10: ;
  case 0xb: ;
    iVar2 = (int)*(short *)(0x007999deu + (uint)iVar1);
    break;
  case 0xc: ;
    *param_2 = *(short *)(0x007999deu + (uint)iVar1) + 1;
    *param_3 = *(short *)(0x007999e0u + (uint)iVar1) + 1;
    return 1;
default: ;
    iVar2 = (int)*(short *)(0x007999deu + (uint)iVar1);
LAB_00543093: ;
    *param_2 = iVar2;
    *param_3 = (int)*(short *)(0x007999e0u + (uint)iVar1);
    return 1;
  }
  *param_2 = iVar2;
  *param_3 = (int)*(short *)(0x007999e0u + (uint)iVar1);
  return 1;
}
