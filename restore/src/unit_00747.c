#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)

int FUN_0044d750(void)

{
  int in_ECX;
  
  return ((int)*(short *)(in_ECX + 0x2c) +
          (((int)*(short *)(in_ECX + 0x14) + (int)DAT_007848d8) - DAT_0078d7bc) * 0xc +
         (uint)*(byte *)(in_ECX + 3)) - DAT_0078d7c0;
}
