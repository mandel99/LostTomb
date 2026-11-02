#include "restore_types.h"
void FUN_004f1cb0(int param_1)

{
  undefined2 uVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  *(undefined2 *)(0x00bcd718u + (uint)iVar2) = 1;
  uVar1 = *(undefined2 *)(0x00b4e6fcu +
      (uint)*(int *)(0x00bcd620u + (uint)iVar2) * 2u);
  *(undefined2 *)(0x00bcd642u + (uint)iVar2) = uVar1;
  *(undefined2 *)(0x00bcd644u + (uint)iVar2) = 0;
  *(undefined2 *)(0x00bcd646u + (uint)iVar2) = uVar1;
  *(undefined2 *)(0x00bcd648u + (uint)iVar2) = 0;
  *(undefined2 *)(0x00bcd64au + (uint)iVar2) = 1;
  *(undefined2 *)(0x00bcd64cu + (uint)iVar2) = 0xf;
  return;
}
