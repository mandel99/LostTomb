#include "restore_types.h"
undefined4 FUN_004f56a0(int param_1,int param_2)

{
  int iVar1;
  undefined3 uVar2;
  
  iVar1 = param_1 * 0x10;
  uVar2 = (undefined3)((0x005d19c8u + (uint)iVar1) >> 8);
  if (*(short *)(0x005d19ceu + (uint)iVar1) == param_2) {
    return CONCAT31(uVar2,1);
  }
  if (*(short *)(0x005d19d0u + (uint)iVar1) == param_2) {
    return CONCAT31(uVar2,1);
  }
  return CONCAT31((int3)(char)((ushort)*(short *)(0x005d19d2u + (uint)iVar1) >> 8),
                  *(short *)(0x005d19d2u + (uint)iVar1) == param_2);
}
