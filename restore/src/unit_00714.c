#include "restore_types.h"

undefined4 FUN_00449b30(void)

{
  char cVar1;
  int in_ECX;
  
  cVar1 = *(char *)(in_ECX + 2);
  if (cVar1 == '\x01') {
    return 0x95;
  }
  if (cVar1 != '\x13') {
    if (cVar1 != '\x14') {
      return 0xffffffff;
    }
    return 0x96;
  }
  return 0x23;
}
