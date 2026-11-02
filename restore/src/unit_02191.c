#include "restore_types.h"

int FUN_004f6630(void)

{
  int in_ECX;
  int iVar1;
  
  iVar1 = *(int *)(in_ECX + 8) - *(int *)(in_ECX + 4);
  if (-1 < iVar1) {
    return iVar1;
  }
  return *(int *)(in_ECX + 0xc) + iVar1;
}
