#include "restore_types.h"

undefined4 FUN_0044fce0(void)

{
  int in_ECX;
  
  if (*(char *)(in_ECX + 2) == '\x01') {
    if (*(char *)(in_ECX + 0x56) == '\0') {
      if (*(char *)(in_ECX + 0x60) == '\x01') {
        return 0xf;
      }
      if (*(char *)(in_ECX + 0x60) == '\x02') {
        return 0x21;
      }
    }
    else {
      if (*(char *)(in_ECX + 0x60) == '\x01') {
        return 0xe;
      }
      if (*(char *)(in_ECX + 0x60) == '\x02') {
        return 0x20;
      }
    }
  }
  return 0xffffffff;
}
