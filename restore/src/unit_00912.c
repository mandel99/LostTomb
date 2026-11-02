#include "restore_types.h"
#define DAT_007058b8 (*(undefined *)0x007058b8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)
#define DAT_00bcb9b4 (*(undefined *)0x00bcb9b4u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00ea5d1c (*(undefined4 *)0x00ea5d1cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00480180();

int FUN_0045f7b0(undefined4 param_1,undefined4 param_2,int param_3,uint param_4)

{
  int iVar1;
  int iVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  
  iVar1 = (&DAT_00ea5d1c)[DAT_00eaac28 * 0x1276];
  switch((int)(param_3)) {
  case 2: ;
    sVar3 = 0x15;
    break;
default: ;
    sVar3 = -1;
    break;
  case 8: ;
    sVar3 = 0x16;
    break;
  case 9: ;
    sVar3 = 0xd;
    break;
  case 10: ;
    sVar3 = 8;
    break;
  case 0xb: ;
    sVar3 = 5;
    break;
  case 0xc: ;
    sVar3 = 7;
    break;
  case 0xd: ;
    sVar3 = 0x18;
    break;
  case 0xe: ;
    sVar3 = 9;
    break;
  case 0xf: ;
    sVar3 = 0x13;
    break;
  case 0x10: ;
    sVar3 = 1;
    break;
  case 0x11: ;
    sVar3 = 0x10;
    break;
  case 0x12: ;
    sVar3 = 10;
    break;
  case 0x14: ;
    sVar3 = 4;
    break;
  case 0x15: ;
    sVar3 = 0xf;
    break;
  case 0x16: ;
    sVar3 = 0xb;
    break;
  case 0x17: ;
    sVar3 = 0xc;
    break;
  case 0x18: ;
    sVar3 = 3;
    break;
  case 0x19: ;
    sVar3 = 6;
    break;
  case 0x1c: ;
    sVar3 = 0xe;
    break;
  case 0x1d: ;
    sVar3 = 0x11;
    break;
  case 0x1e: ;
    sVar3 = 0x12;
    break;
  case 0x1f: ;
    sVar3 = 0x17;
    break;
  case 0x20: ;
    sVar3 = 0x14;
    break;
  case 0x21: ;
    sVar3 = 0x19;
    break;
  case 0x22: ;
    sVar3 = 0x1a;
  }
  if ((param_3 != 10) && (param_3 != 0x1c)) {
    return 0;
  }
  if (*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + param_3) * 4) != 0) {
    return 0;
  }
  if (DAT_007058b8 < 1) {
    return 0;
  }
  iVar4 = iVar1 * 0x108;
  if (sVar3 == (&DAT_007999ea)[iVar1 * 0x84]) {
    switch((&DAT_007999e8)[iVar1 * 0x84]) {
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
    iVar5 = (int)(short)(&DAT_00799a14)[iVar1 * 0x84];
    if (iVar2 <= iVar5) {
      return 0;
    }
    if (0 < *(short *)(&DAT_00799adc + iVar4)) {
      switch((&DAT_007999e8)[iVar1 * 0x84]) {
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
      iVar2 = iVar2 + *(short *)(&DAT_00799adc + iVar4) * -100;
      if (iVar2 == iVar5 || iVar2 - iVar5 < 0) {
        return 0;
      }
    }
  }
  if (sVar3 == *(short *)((int)&DAT_00799aa4 + iVar4 + 2)) {
    switch((&DAT_007999e8)[iVar1 * 0x84]) {
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
    iVar5 = (int)*(short *)(&DAT_00799aa4 + iVar1 * 0x42);
    if (iVar2 <= iVar5) {
      return 0;
    }
    if (0 < *(short *)(&DAT_00799ade + iVar4)) {
      switch((&DAT_007999e8)[iVar1 * 0x84]) {
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
      iVar2 = iVar2 + *(short *)(&DAT_00799ade + iVar4) * -100;
      if (iVar2 == iVar5 || iVar2 - iVar5 < 0) {
        return 0;
      }
    }
  }
  if (DAT_00bcb9b4 < 1) {
    return 0;
  }
  iVar2 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar4 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar4),(&DAT_007999db)[iVar4]);
  if (iVar2 == 0) {
    return 0;
  }
  if ((byte)(&DAT_007999ec)[iVar4] != param_4) {
    return 0;
  }
  if (sVar3 == (&DAT_007999ea)[iVar1 * 0x84]) {
    *(short *)(&DAT_00799adc + iVar4) = *(short *)(&DAT_00799adc + iVar4) + 1;
  }
  if (sVar3 == *(short *)((int)&DAT_00799aa4 + iVar4 + 2)) {
    *(short *)(&DAT_00799ade + iVar4) = *(short *)(&DAT_00799ade + iVar4) + 1;
  }
  return iVar1;
}
