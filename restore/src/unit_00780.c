#include "restore_types.h"

int FUN_0044f8c0(void)

{
  int in_ECX;
  
  switch(*(undefined1 *)(in_ECX + 2)) {
  case 1: ;
    if (*(char *)(in_ECX + 0x56) == '\0') {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x2f;
      case 1: ;
        return 9;
      case 2: ;
        return 0x1b;
      case 3: ;
        return 0x41;
      case 4: ;
        return 0x53;
      case 5: ;
        return 0x65;
      case 6: ;
        return 0x77;
      }
    }
    else {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x2e;
      case 1: ;
        return 8;
      case 2: ;
        return 0x1a;
      case 3: ;
        return 0x40;
      case 4: ;
        return 0x52;
      case 5: ;
        return 100;
      case 6: ;
        return 0x76;
      }
    }
    break;
  case 2: ;
    switch(*(undefined2 *)(in_ECX + 4)) {
    case 1: ;
      return 0xbd;
    case 2: ;
      return 0xb3;
    case 3: ;
      return 0xa9;
    case 4: ;
      return 0x106;
    }
    break;
  case 3: ;
    return 0xd9;
  case 6: ;
    return 0xdd;
  case 7: ;
    return 0xe1;
  case 8: ;
    return 0xe5;
  case 9: ;
    return 0xe9;
  case 10: ;
    return 0xed;
  case 0xb: ;
    return 0xf1;
  case 0xd: ;
    return 0xd1;
  case 0xe: ;
    return 0xd5;
  case 0xf: ;
    return 0xc9;
  case 0x10: ;
    return 0xcd;
  case 0x11: ;
    return 0xc1;
  case 0x12: ;
    return 0xc5;
  case 0x13: ;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 0x80;
    case 1: ;
      return 0x84;
    case 2: ;
      return 0x88;
    case 3: ;
      return 0x8c;
    case 4: ;
      return 0x90;
    }
    break;
  case 0x14: ;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 0x94;
    case 1: ;
      return 0x98;
    case 2: ;
      return 0x9c;
    case 3: ;
      return 0xa0;
    }
    break;
  case 0x15: ;
    return 0xf9;
  case 0x16: ;
    return 0xfd;
  case 0x17: ;
    return 0x11f - (uint)(*(char *)(in_ECX + 0x56) != '\0');
  }
  return -1;
}
