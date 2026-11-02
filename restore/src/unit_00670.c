#include "restore_types.h"

int FUN_004477d0(void)

{
  int iVar1;
  byte *in_ECX;
  
  iVar1 = (int)(*(int *)(in_ECX + 0x134) * (uint)*in_ECX) / 400;
  if (iVar1 < 1) {
    iVar1 = 1;
  }
  return iVar1;
}
