#include "restore_types.h"

undefined4 FUN_0044f3d0(void)

{
  int in_ECX;
  
  if (*(char *)(in_ECX + 2) == '\x01') {
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 9;
    case 1: ;
      return 2;
    case 2: ;
      return 6;
    case 3: ;
      return 0xc;
    case 4: ;
      return 0xf;
    case 5: ;
      return 0x12;
    case 6: ;
      return 0x15;
    }
  }
  else if (*(char *)(in_ECX + 2) == '\x17') {
    return 0x34;
  }
  return 0xffffffff;
}
