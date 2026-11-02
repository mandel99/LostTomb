#include "restore_types.h"

undefined1 FUN_00449640(void)

{
  byte bVar1;
  int in_ECX;
  
  bVar1 = *(byte *)(in_ECX + 2);
  if ((bVar1 != 1) && ((bVar1 < 0x13 || (0x14 < bVar1)))) {
    return 0;
  }
  return 1;
}
