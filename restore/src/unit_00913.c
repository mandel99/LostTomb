#include "restore_types.h"
#define DAT_00579520 (*(undefined *)0x00579520u)
#define DAT_007027bc (*(char *)0x007027bcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea5b50 (*(undefined4 *)0x00ea5b50u)
#define DAT_00ea6020 (*(undefined4 *)0x00ea6020u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_00417ae0();
extern undefined4 FUN_00459ef0();

int FUN_0045fe20(int param_1,int param_2,int param_3)

{
  short sVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_4;
  
  local_4 = 0;
  if (0 < param_1) {
    sVar1 = (&DAT_007999e8)[param_1 * 0x84];
    if ((sVar1 < 0x6e) || (0x72 < sVar1)) {
      switch((int)(sVar1)) {
      case 0x20: ;
      case 0x2f: ;
      case 0x33: ;
      case 0x35: ;
      case 0x4a: ;
      case 0x71: ;
      case 0xb1: ;
      case 0xb2: ;
      case 0xbb: ;
      case 0xbc: ;
      case 0xbd: ;
      case 0xc2: ;
      case 0xcb: ;
      case 0xcc: ;
      case 0xcd: ;
      case 0xe2: ;
      case 0xe7: ;
      case 0xe8: ;
      case 0xe9: ;
        break;
default: ;
        goto switchD_0045fe6f_caseD_21;
      }
    }
    if (param_3 == 0x15) {
      iVar3 = FUN_00459ef0(param_1);
      iVar5 = DAT_00eaac28;
      iVar4 = DAT_00e38ebc;
      if (iVar3 != 0) {
        iVar3 = DAT_00eaac28 * 0x49d8;
        *(int *)(&DAT_00ea187c + iVar3) = *(int *)(&DAT_00ea187c + iVar3) + param_2;
        (&DAT_00ea5b50)[iVar5 * 0x1276] = (&DAT_00ea5b50)[iVar5 * 0x1276] + param_2;
        *(int *)(&DAT_00ea6020 + iVar3) = *(int *)(&DAT_00ea6020 + iVar3) + param_2;
        if ((((iVar4 == 0) && (DAT_00579520 == DAT_00e38ea8)) && ((char)DAT_007027bc == '\0')) &&
           (499 < (int)(&DAT_00ea5b50)[iVar5 * 0x1276])) {
          FUN_00417ae0(4);
        }
        return param_2;
      }
    }
    else {
      switch(param_3 + -2) {
      case 0: ;
        sVar2 = 0x15;
        break;
default: ;
        sVar2 = -1;
        break;
      case 6: ;
        sVar2 = 0x16;
        break;
      case 7: ;
        sVar2 = 0xd;
        break;
      case 8: ;
        sVar2 = 8;
        break;
      case 9: ;
        sVar2 = 5;
        break;
      case 10: ;
        sVar2 = 7;
        break;
      case 0xb: ;
        sVar2 = 0x18;
        break;
      case 0xc: ;
        sVar2 = 9;
        break;
      case 0xd: ;
        sVar2 = 0x13;
        break;
      case 0xe: ;
        sVar2 = 1;
        break;
      case 0xf: ;
        sVar2 = 0x10;
        break;
      case 0x10: ;
        sVar2 = 10;
        break;
      case 0x12: ;
        sVar2 = 4;
        break;
      case 0x13: ;
        sVar2 = 0xf;
        break;
      case 0x14: ;
        sVar2 = 0xb;
        break;
      case 0x15: ;
        sVar2 = 0xc;
        break;
      case 0x16: ;
        sVar2 = 3;
        break;
      case 0x17: ;
        sVar2 = 6;
        break;
      case 0x1a: ;
        sVar2 = 0xe;
        break;
      case 0x1b: ;
        sVar2 = 0x11;
        break;
      case 0x1c: ;
        sVar2 = 0x12;
        break;
      case 0x1d: ;
        sVar2 = 0x17;
        break;
      case 0x1e: ;
        sVar2 = 0x14;
        break;
      case 0x1f: ;
        sVar2 = 0x19;
        break;
      case 0x20: ;
        sVar2 = 0x1a;
      }
      if ((&DAT_007999ea)[param_1 * 0x84] == sVar2) {
        switch((int)(sVar1)) {
        case 0x20: ;
        case 0x33: ;
          iVar4 = 300;
          break;
default: ;
          iVar4 = 200;
          break;
        case 0x2f: ;
        case 0x35: ;
          iVar4 = 500;
          break;
        case 0x54: ;
        case 0x55: ;
        case 0x56: ;
        case 0x57: ;
        case 0xb8: ;
        case 0xbb: ;
        case 0xbc: ;
        case 0xbd: ;
          iVar4 = 100000;
        }
        iVar5 = (int)(short)(&DAT_00799a14)[param_1 * 0x84];
        if (iVar5 < iVar4) {
          switch((int)(sVar1)) {
          case 0x20: ;
          case 0x33: ;
            iVar4 = 300;
            break;
default: ;
            iVar4 = 200;
            break;
          case 0x2f: ;
          case 0x35: ;
            iVar4 = 500;
            break;
          case 0x54: ;
          case 0x55: ;
          case 0x56: ;
          case 0x57: ;
          case 0xb8: ;
          case 0xbb: ;
          case 0xbc: ;
          case 0xbd: ;
            iVar4 = 100000;
          }
          if (iVar4 <= iVar5 + param_2) {
            switch((int)(sVar1)) {
            case 0x20: ;
            case 0x33: ;
              iVar4 = 300;
              break;
default: ;
              iVar4 = 200;
              break;
            case 0x2f: ;
            case 0x35: ;
              iVar4 = 500;
              break;
            case 0x54: ;
            case 0x55: ;
            case 0x56: ;
            case 0x57: ;
            case 0xb8: ;
            case 0xbb: ;
            case 0xbc: ;
            case 0xbd: ;
              iVar4 = 100000;
            }
            local_4 = (iVar5 - iVar4) + param_2;
          }
          (&DAT_00799a14)[param_1 * 0x84] =
               ((&DAT_00799a14)[param_1 * 0x84] - (short)local_4) + (short)param_2;
          return param_2 - local_4;
        }
      }
      else {
        switch(param_3 + -2) {
        case 0: ;
          sVar2 = 0x15;
          break;
default: ;
          sVar2 = -1;
          break;
        case 6: ;
          sVar2 = 0x16;
          break;
        case 7: ;
          sVar2 = 0xd;
          break;
        case 8: ;
          sVar2 = 8;
          break;
        case 9: ;
          sVar2 = 5;
          break;
        case 10: ;
          sVar2 = 7;
          break;
        case 0xb: ;
          sVar2 = 0x18;
          break;
        case 0xc: ;
          sVar2 = 9;
          break;
        case 0xd: ;
          sVar2 = 0x13;
          break;
        case 0xe: ;
          sVar2 = 1;
          break;
        case 0xf: ;
          sVar2 = 0x10;
          break;
        case 0x10: ;
          sVar2 = 10;
          break;
        case 0x12: ;
          sVar2 = 4;
          break;
        case 0x13: ;
          sVar2 = 0xf;
          break;
        case 0x14: ;
          sVar2 = 0xb;
          break;
        case 0x15: ;
          sVar2 = 0xc;
          break;
        case 0x16: ;
          sVar2 = 3;
          break;
        case 0x17: ;
          sVar2 = 6;
          break;
        case 0x1a: ;
          sVar2 = 0xe;
          break;
        case 0x1b: ;
          sVar2 = 0x11;
          break;
        case 0x1c: ;
          sVar2 = 0x12;
          break;
        case 0x1d: ;
          sVar2 = 0x17;
          break;
        case 0x1e: ;
          sVar2 = 0x14;
          break;
        case 0x1f: ;
          sVar2 = 0x19;
          break;
        case 0x20: ;
          sVar2 = 0x1a;
        }
        if (*(short *)((int)&DAT_00799aa4 + param_1 * 0x108 + 2) == sVar2) {
          switch((int)(sVar1)) {
          case 0x20: ;
          case 0x33: ;
            iVar4 = 300;
            break;
default: ;
            iVar4 = 200;
            break;
          case 0x2f: ;
          case 0x35: ;
            iVar4 = 500;
            break;
          case 0x54: ;
          case 0x55: ;
          case 0x56: ;
          case 0x57: ;
          case 0xb8: ;
          case 0xbb: ;
          case 0xbc: ;
          case 0xbd: ;
            iVar4 = 100000;
          }
          iVar5 = (int)*(short *)(&DAT_00799aa4 + param_1 * 0x42);
          if (iVar5 < iVar4) {
            switch((int)(sVar1)) {
            case 0x20: ;
            case 0x33: ;
              iVar4 = 300;
              break;
default: ;
              iVar4 = 200;
              break;
            case 0x2f: ;
            case 0x35: ;
              iVar4 = 500;
              break;
            case 0x54: ;
            case 0x55: ;
            case 0x56: ;
            case 0x57: ;
            case 0xb8: ;
            case 0xbb: ;
            case 0xbc: ;
            case 0xbd: ;
              iVar4 = 100000;
            }
            if (iVar4 <= iVar5 + param_2) {
              switch((int)(sVar1)) {
              case 0x20: ;
              case 0x33: ;
                iVar4 = 300;
                break;
default: ;
                iVar4 = 200;
                break;
              case 0x2f: ;
              case 0x35: ;
                iVar4 = 500;
                break;
              case 0x54: ;
              case 0x55: ;
              case 0x56: ;
              case 0x57: ;
              case 0xb8: ;
              case 0xbb: ;
              case 0xbc: ;
              case 0xbd: ;
                iVar4 = 100000;
              }
              local_4 = (iVar5 - iVar4) + param_2;
            }
            *(short *)(&DAT_00799aa4 + param_1 * 0x42) =
                 (*(short *)(&DAT_00799aa4 + param_1 * 0x42) - (short)local_4) + (short)param_2;
            return param_2 - local_4;
          }
        }
      }
    }
  }
switchD_0045fe6f_caseD_21: ;
  return 0;
}
