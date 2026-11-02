#include "restore_types.h"
extern undefined4 FUN_0044c110();
extern int FUN_0044d750();

int FUN_0044c9a0(void)

{
  int iVar1;
  int iVar2;
  int in_ECX;
  
  iVar1 = FUN_0044d750();
  iVar2 = FUN_0044c110();
  if (iVar2 < 1) {
    iVar2 = 0xc;
  }
  iVar1 = ((iVar1 - (uint)*(byte *)(in_ECX + 0x61)) - iVar2) + 6;
  if (iVar1 < 1) {
    iVar1 = 1;
  }
  return iVar1;
}
