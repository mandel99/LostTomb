#include "restore_types.h"

undefined1 FUN_00449690(void)

{
  undefined1 uVar1;
  int in_ECX;
  
  uVar1 = 1;
  if ((*(char *)(in_ECX + 2) != '\x01') || (*(char *)(in_ECX + 0x60) != '\x03')) {
    uVar1 = 0;
  }
  return uVar1;
}
