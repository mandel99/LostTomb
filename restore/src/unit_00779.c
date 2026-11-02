#include "restore_types.h"

int FUN_0044f660(void)

{
  int in_ECX;
  
  switch(*(undefined1 *)(in_ECX + 2)) {
  case 1: ;
    if (*(char *)(in_ECX + 0x56) == '\0') {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x35;
      case 1: ;
        return 0x11;
      case 2: ;
        return 0x23;
      case 3: ;
        return 0x47;
      case 4: ;
        return 0x59;
      case 5: ;
        return 0x6b;
      case 6: ;
        return 0x7d;
      }
    }
    else {
      switch(*(undefined1 *)(in_ECX + 0x60)) {
      case 0: ;
        return 0x34;
      case 1: ;
        return 0x10;
      case 2: ;
        return 0x22;
      case 3: ;
        return 0x46;
      case 4: ;
        return 0x58;
      case 5: ;
        return 0x6a;
      case 6: ;
        return 0x7c;
      }
    }
    break;
  case 2: ;
    switch(*(undefined2 *)(in_ECX + 4)) {
    case 1: ;
      return 0xbe;
    case 2: ;
      return 0xb4;
    case 3: ;
      return 0xaa;
    case 4: ;
      return 0x107;
    }
    break;
  case 3: ;
    return 0xda;
  case 6: ;
    return 0xde;
  case 7: ;
    return 0xe2;
  case 8: ;
    return 0xe6;
  case 9: ;
    return 0xea;
  case 10: ;
    return 0xee;
  case 0xb: ;
    return 0xf2;
  case 0xd: ;
    return 0xd2;
  case 0xe: ;
    return 0xd6;
  case 0xf: ;
    return 0xca;
  case 0x10: ;
    return 0xce;
  case 0x11: ;
    return 0xc2;
  case 0x12: ;
    return 0xc6;
  case 0x13: ;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 0x81;
    case 1: ;
      return 0x85;
    case 2: ;
      return 0x89;
    case 3: ;
      return 0x8d;
    case 4: ;
      return 0x91;
    }
    break;
  case 0x14: ;
    switch(*(undefined1 *)(in_ECX + 0x60)) {
    case 0: ;
      return 0x95;
    case 1: ;
      return 0x99;
    case 2: ;
      return 0x9d;
    case 3: ;
      return 0xa1;
    }
    break;
  case 0x15: ;
    return 0xfa;
  case 0x16: ;
    return 0xfe;
  case 0x17: ;
    return 0x125 - (uint)(*(char *)(in_ECX + 0x56) != '\0');
  }
  return -1;
}
