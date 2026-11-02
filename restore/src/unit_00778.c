#include "restore_types.h"

undefined4 FUN_0044f470(void)

{
  int in_ECX;
  
  switch(*(undefined1 *)(in_ECX + 2)) {
  case 1: ;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 2;
    case 1: ;
      return 0;
    case 2: ;
      return 1;
    case 3: ;
      return 3;
    case 4: ;
      return 4;
    case 5: ;
      return 5;
    case 6: ;
      return 6;
    }
    break;
  case 2: ;
    switch(*(undefined2 *)(in_ECX + 4)) {
    case 1: ;
      return 0x10;
    case 2: ;
      return 0x11;
    case 3: ;
      return 0x12;
    case 4: ;
      return 0x22;
    }
    break;
  case 3: ;
    return 0x1b;
  case 6: ;
    return 0x1c;
  case 7: ;
    return 0x1d;
  case 8: ;
    return 0x19;
  case 9: ;
    return 0x1a;
  case 10: ;
    return 0x1e;
  case 0xb: ;
  case 0xc: ;
    return 0x1f;
  case 0xd: ;
    return 0x15;
  case 0xe: ;
    return 0x16;
  case 0xf: ;
    return 0x17;
  case 0x10: ;
    return 0x18;
  case 0x11: ;
    return 0x13;
  case 0x12: ;
    return 0x14;
  case 0x13: ;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 7;
    case 1: ;
      return 8;
    case 2: ;
      return 9;
    case 3: ;
      return 10;
    case 4: ;
      return 0xb;
    }
    break;
  case 0x14: ;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 0xc;
    case 1: ;
      return 0xd;
    case 2: ;
      return 0xe;
    case 3: ;
      return 0xf;
    }
    break;
  case 0x15: ;
    return 0x20;
  case 0x16: ;
    return 0x21;
  case 0x17: ;
    return 0x23;
  }
  return 0xffffffff;
}
