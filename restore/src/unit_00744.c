#include "restore_types.h"
#define DAT_007848d8 (*(undefined *)0x007848d8u)
#define DAT_0078d7bc (*(undefined *)0x0078d7bcu)
#define DAT_0078d7c0 (*(undefined1 *)0x0078d7c0u)

bool FUN_0044d680(char param_1)

{
  int in_ECX;
  
  if ((param_1 == '\0') && ((*(byte *)(in_ECX + 0x28) & 5) != 0)) {
    return false;
  }
  if (DAT_0078d7bc != (int)*(short *)(in_ECX + 0x14) + (int)DAT_007848d8) {
    return false;
  }
  return DAT_0078d7c0 == *(byte *)(in_ECX + 3);
}
