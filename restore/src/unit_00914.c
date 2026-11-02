#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)

int FUN_00460890(int param_1,int param_2,int param_3)

{
  undefined2 uVar1;
  int iVar2;
  int iVar3;
  
  if (param_1 < 1) {
    return 1;
  }
  if (param_3 == 10) {
    uVar1 = (&DAT_007999e8)[param_1 * 0x84];
    switch((int)(uVar1)) {
    case 0x20: ;
    case 0x33: ;
      iVar2 = 300;
      break;
default: ;
      iVar2 = 200;
      break;
    case 0x2f: ;
    case 0x35: ;
      iVar2 = 500;
      break;
    case 0x54: ;
    case 0x55: ;
    case 0x56: ;
    case 0x57: ;
    case 0xb8: ;
    case 0xbb: ;
    case 0xbc: ;
    case 0xbd: ;
      iVar2 = 100000;
    }
    iVar3 = (int)(short)(&DAT_00799a14)[param_1 * 0x84];
    if (iVar3 < iVar2) {
      switch((int)(uVar1)) {
      case 0x20: ;
      case 0x33: ;
        iVar2 = 300;
        break;
default: ;
        iVar2 = 200;
        break;
      case 0x2f: ;
      case 0x35: ;
        iVar2 = 500;
        break;
      case 0x54: ;
      case 0x55: ;
      case 0x56: ;
      case 0x57: ;
      case 0xb8: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
        iVar2 = 100000;
      }
      if (iVar2 < iVar3 + param_2) {
        switch((int)(uVar1)) {
        case 0x20: ;
        case 0x33: ;
          iVar2 = 300;
          break;
default: ;
          iVar2 = 200;
          break;
        case 0x2f: ;
        case 0x35: ;
          iVar2 = 500;
          break;
        case 0x54: ;
        case 0x55: ;
        case 0x56: ;
        case 0x57: ;
        case 0xb8: ;
        case 0xbb: ;
        case 0xbc: ;
        case 0xbd: ;
          iVar2 = 100000;
        }
        param_2 = param_2 + (iVar3 - iVar2);
      }
      (&DAT_00799a14)[param_1 * 0x84] = (&DAT_00799a14)[param_1 * 0x84] + (short)param_2;
      *(short *)(&DAT_00799adc + param_1 * 0x108) = *(short *)(&DAT_00799adc + param_1 * 0x108) + -1
      ;
      return param_2;
    }
    return 0;
  }
  uVar1 = (&DAT_007999e8)[param_1 * 0x84];
  switch((int)(uVar1)) {
  case 0x20: ;
  case 0x33: ;
    iVar2 = 300;
    break;
default: ;
    iVar2 = 200;
    break;
  case 0x2f: ;
  case 0x35: ;
    iVar2 = 500;
    break;
  case 0x54: ;
  case 0x55: ;
  case 0x56: ;
  case 0x57: ;
  case 0xb8: ;
  case 0xbb: ;
  case 0xbc: ;
  case 0xbd: ;
    iVar2 = 100000;
  }
  iVar3 = (int)*(short *)(&DAT_00799aa4 + param_1 * 0x42);
  if (iVar3 < iVar2) {
    switch((int)(uVar1)) {
    case 0x20: ;
    case 0x33: ;
      iVar2 = 300;
      break;
default: ;
      iVar2 = 200;
      break;
    case 0x2f: ;
    case 0x35: ;
      iVar2 = 500;
      break;
    case 0x54: ;
    case 0x55: ;
    case 0x56: ;
    case 0x57: ;
    case 0xb8: ;
    case 0xbb: ;
    case 0xbc: ;
    case 0xbd: ;
      iVar2 = 100000;
    }
    if (iVar2 < iVar3 + param_2) {
      switch((int)(uVar1)) {
      case 0x20: ;
      case 0x33: ;
        iVar2 = 300;
        break;
default: ;
        iVar2 = 200;
        break;
      case 0x2f: ;
      case 0x35: ;
        iVar2 = 500;
        break;
      case 0x54: ;
      case 0x55: ;
      case 0x56: ;
      case 0x57: ;
      case 0xb8: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
        iVar2 = 100000;
      }
      param_2 = param_2 + (iVar3 - iVar2);
    }
    *(short *)(&DAT_00799aa4 + param_1 * 0x42) =
         *(short *)(&DAT_00799aa4 + param_1 * 0x42) + (short)param_2;
    *(short *)(&DAT_00799ade + param_1 * 0x108) = *(short *)(&DAT_00799ade + param_1 * 0x108) + -1;
    return param_2;
  }
  return 0;
}
