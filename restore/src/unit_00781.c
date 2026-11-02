#include "restore_types.h"

int FUN_0044fb20(void)

{
  int in_ECX;
  
  if (*(char *)(in_ECX + 2) == '\x01') {
    if (*(char *)(in_ECX + 0x56) == '\0') {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x33;
      case 1: ;
        return 0xd;
      case 2: ;
        return 0x1f;
      case 3: ;
        return 0x45;
      case 4: ;
        return 0x57;
      case 5: ;
        return 0x69;
      case 6: ;
        return 0x7b;
      }
    }
    else {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x32;
      case 1: ;
        return 0xc;
      case 2: ;
        return 0x1e;
      case 3: ;
        return 0x44;
      case 4: ;
        return 0x56;
      case 5: ;
        return 0x68;
      case 6: ;
        return 0x7a;
      }
    }
  }
  else if (*(char *)(in_ECX + 2) == '\x17') {
    return 0x123 - (uint)(*(char *)(in_ECX + 0x56) != '\0');
  }
  return -1;
}
