#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)
extern uint FUN_0044d6d0();

uint FUN_0044d700(void)

{
  uint in_EAX;
  uint uVar1;
  int iVar2;
  int in_ECX;
  
  if ((*(byte *)(in_ECX + 0x28) & 0x11) != 0) {
    return in_EAX & 0xffffff00;
  }
  uVar1 = FUN_0044d6d0();
  if ((char)uVar1 != '\0') {
    return uVar1 & 0xffffff00;
  }
  iVar2 = (((DAT_0078d7bc - *(short *)(in_ECX + 0x14)) - (int)DAT_007848d8) * 0xc -
          (uint)*(byte *)(in_ECX + 3)) + DAT_0078d7c0;
  return CONCAT31((int3)((uint)iVar2 >> 8),iVar2 <= *(short *)(in_ECX + 0x2c));
}
