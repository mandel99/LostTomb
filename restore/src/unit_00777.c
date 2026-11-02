#include "restore_types.h"

undefined4 FUN_0044f440(void)

{
  int in_ECX;
  
  if (*(char *)(in_ECX + 2) == '\x01') {
    if (*(char *)(in_ECX + 0x60) == '\x01') {
      return 1;
    }
    if (*(char *)(in_ECX + 0x60) == '\x02') {
      return 5;
    }
  }
  return 0xffffffff;
}
