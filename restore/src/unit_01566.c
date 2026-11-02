#include "restore_types.h"

int FUN_004becf0(void)

{
  int iVar1;
  int in_ECX;
  
  iVar1 = *(int *)(in_ECX + 0x32fb0) + -1;
  if (iVar1 < 0) {
    iVar1 = *(int *)(in_ECX + 0x32fb0) + 0xb;
  }
  return iVar1;
}
