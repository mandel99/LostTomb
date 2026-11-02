#include "restore_types.h"

void FUN_0055d2e0(uint param_1)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x40) != 0) {
    *(uint *)(in_ECX + 0x44) = *(int *)(*(int *)(in_ECX + 0x40) + 0xc) + (param_1 & 0xffff) * 8;
  }
  return;
}
