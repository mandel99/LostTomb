#include "restore_types.h"

int FUN_0044fc00(void)

{
  int in_ECX;
  
  if (*(char *)(in_ECX + 2) == '\x01') {
    if (*(char *)(in_ECX + 0x56) == '\0') {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x31;
      case 1: ;
        return 0xb;
      case 2: ;
        return 0x1d;
      case 3: ;
        return 0x43;
      case 4: ;
        return 0x55;
      case 5: ;
        return 0x67;
      case 6: ;
        return 0x79;
      }
    }
    else {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x30;
      case 1: ;
        return 10;
      case 2: ;
        return 0x1c;
      case 3: ;
        return 0x42;
      case 4: ;
        return 0x54;
      case 5: ;
        return 0x66;
      case 6: ;
        return 0x78;
      }
    }
  }
  else if (*(char *)(in_ECX + 2) == '\x17') {
    return 0x121 - (uint)(*(char *)(in_ECX + 0x56) != '\0');
  }
  return -1;
}
