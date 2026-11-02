#include "restore_types.h"
extern undefined4 FUN_004f6700();

void FUN_004f6690(undefined4 param_1)

{
  undefined1 *in_ECX;
  
  *(undefined4 *)(in_ECX + 8) = 0;
  *(undefined4 *)(in_ECX + 0xc) = 0;
  *(undefined4 *)(in_ECX + 0x14) = 0;
  *(undefined4 *)(in_ECX + 0x10) = 0;
  *in_ECX = 0;
  *(undefined4 *)(in_ECX + 4) = 1;
  FUN_004f6700(param_1);
  return;
}
