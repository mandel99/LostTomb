#include "restore_types.h"

undefined4 FUN_0044f360(void)

{
  int in_ECX;
  
  if (*(char *)(in_ECX + 2) == '\x01') {
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 10;
    case 1: ;
      return 3;
    case 2: ;
      return 7;
    case 3: ;
      return 0xd;
    case 4: ;
      return 0x10;
    case 5: ;
      return 0x13;
    case 6: ;
      return 0x16;
    }
  }
  else if (*(char *)(in_ECX + 2) == '\x17') {
    return 0x33;
  }
  return 0xffffffff;
}
