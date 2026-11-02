#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)

uint FUN_0044d6d0(void)

{
  int iVar1;
  uint3 uVar2;
  int in_ECX;
  
  iVar1 = (int)*(short *)(in_ECX + 0x14) + (int)DAT_007848d8;
  uVar2 = (uint3)((uint)iVar1 >> 8);
  if (DAT_0078d7bc < iVar1) {
    return CONCAT31(uVar2,1);
  }
  if (iVar1 < DAT_0078d7bc) {
    return (uint)uVar2 << 8;
  }
  return (uint)(DAT_0078d7c0 < (int)(uint)*(byte *)(in_ECX + 3));
}
