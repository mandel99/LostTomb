#include "restore_types.h"
extern undefined2 * FUN_0044c0d0();

uint FUN_004496d0(int param_1)

{
  uint uVar1;
  short *in_ECX;
  
  uVar1 = FUN_0044c0d0(param_1);
  if (uVar1 == 0) {
    return 0;
  }
  if ((*(byte *)(uVar1 + 0x28) & 1) == 0) {
    return uVar1 & 0xffffff00;
  }
  return CONCAT31((int3)(char)((ushort)*in_ECX >> 8),param_1 != *in_ECX);
}
