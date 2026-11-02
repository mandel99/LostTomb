#include "restore_types.h"
#define DAT_007999d9 (*(undefined4 *)0x007999d9u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd668 (*(undefined4 *)0x00bcd668u)
#define DAT_00bcd690 (*(undefined4 *)0x00bcd690u)
#define DAT_00d0569c (*(undefined *)0x00d0569cu)
#define DAT_00d056a0 (*(undefined *)0x00d056a0u)
#define DAT_00d056a4 (*(undefined *)0x00d056a4u)
#define DAT_00d056a8 (*(undefined *)0x00d056a8u)
#define DAT_00d056ac (*(undefined *)0x00d056acu)
#define DAT_00d056b0 (*(undefined *)0x00d056b0u)
#define DAT_00d056b4 (*(undefined *)0x00d056b4u)
#define DAT_00d056b8 (*(undefined *)0x00d056b8u)
#define DAT_00d056bc (*(undefined *)0x00d056bcu)
#define DAT_00d056d4 (*(undefined *)0x00d056d4u)
#define DAT_00d056e0 (*(undefined *)0x00d056e0u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00480180();
extern int FUN_004b4800();
extern undefined4 FUN_004c5120();

void FUN_00461b20(int param_1)

{
  undefined2 uVar1;
  undefined1 uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  
  iVar5 = param_1 * 0x108;
  iVar3 = FUN_00480180((int)*(short *)((int)&DAT_007999dc + iVar5 + 2),
                       (int)*(short *)(&DAT_007999e0 + iVar5),(&DAT_007999db)[iVar5]);
  if (iVar3 != 0) {
    iVar3 = FUN_004b4800((&DAT_007999d9)[iVar5],0x5b,_DAT_00d3b4e4,_DAT_00d3b4e0,0);
    (&DAT_00799a02)[param_1 * 0x84] = (short)iVar3;
    uVar1 = (&DAT_007999e8)[param_1 * 0x84];
    iVar5 = iVar3 * 0x184;
    (&DAT_00bcd630)[iVar5] = 8;
    (&DAT_00bcd652)[iVar3 * 0xc2] = (short)param_1;
    *(undefined2 *)(&DAT_00bcd62e + iVar5) = 0;
    switch((int)(uVar1)) {
default: ;
      uVar2 = 0x28;
      break;
    case 0x1e: ;
    case 0x22: ;
      uVar2 = 0x10;
      break;
    case 0x1f: ;
    case 0x20: ;
    case 0x24: ;
    case 0xe2: ;
      uVar2 = 0xf;
      break;
    case 0x21: ;
    case 0x23: ;
      uVar2 = 0x11;
      break;
    case 0x33: ;
    case 0x35: ;
      uVar2 = 0x1d;
      break;
    case 0x3c: ;
    case 0x3d: ;
    case 0x3e: ;
    case 0x3f: ;
    case 0x40: ;
    case 0x41: ;
    case 0x42: ;
    case 0x43: ;
    case 0x44: ;
    case 0x45: ;
    case 0x97: ;
    case 0x9c: ;
      uVar2 = 0x1b;
    }
    (&DAT_00bcd603)[iVar5] = uVar2;
    switch((int)(uVar2)) {
    case 5: ;
      *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056e0;
      DAT_00d056e0 = DAT_00d056e0 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0xfe;
      if (0x7f < DAT_00d056e0) {
        DAT_00d056e0 = 0;
        return;
      }
      break;
default: ;
      *(undefined2 *)(&DAT_00bcd668 + iVar5) = 0xffff;
      *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0xffffffff;
      break;
    case 0xf: ;
      *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056bc;
      DAT_00d056bc = DAT_00d056bc + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x107;
      if (0x1f < DAT_00d056bc) {
        DAT_00d056bc = 0;
        return;
      }
      break;
    case 0x10: ;
      *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056b8;
      DAT_00d056b8 = DAT_00d056b8 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x108;
      if (0x1f < DAT_00d056b8) {
        DAT_00d056b8 = 0;
        return;
      }
      break;
    case 0x11: ;
      *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056b4;
      DAT_00d056b4 = DAT_00d056b4 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x109;
      if (0xf < DAT_00d056b4) {
        DAT_00d056b4 = 0;
        return;
      }
      break;
    case 0x1b: ;
      uVar4 = FUN_004c5120((int)(short)(&DAT_00bcd652)[iVar3 * 0xc2]);
      switch((int)(uVar4)) {
      case 0: ;
        *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056b0;
        DAT_00d056b0 = DAT_00d056b0 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x10a;
        if (0xf < DAT_00d056b0) {
          DAT_00d056b0 = 0;
          return;
        }
        break;
      case 1: ;
        *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056ac;
        DAT_00d056ac = DAT_00d056ac + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x10b;
        if (0xf < DAT_00d056ac) {
          DAT_00d056ac = 0;
          return;
        }
        break;
      case 2: ;
        *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056a8;
        DAT_00d056a8 = DAT_00d056a8 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x10c;
        if (0xf < DAT_00d056a8) {
          DAT_00d056a8 = 0;
          return;
        }
        break;
      case 3: ;
        *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056a4;
        DAT_00d056a4 = DAT_00d056a4 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x10d;
        if (0xf < DAT_00d056a4) {
          DAT_00d056a4 = 0;
          return;
        }
        break;
      case 4: ;
        *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056a0;
        DAT_00d056a0 = DAT_00d056a0 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x10e;
        if (0xf < DAT_00d056a0) {
          DAT_00d056a0 = 0;
          return;
        }
      }
      break;
    case 0x1d: ;
    case 0x1e: ;
      *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d0569c;
      DAT_00d0569c = DAT_00d0569c + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x10f;
      if (0x1f < DAT_00d0569c) {
        DAT_00d0569c = 0;
        return;
      }
      break;
    case 0x28: ;
      *(short *)(&DAT_00bcd668 + iVar5) = (short)DAT_00d056d4;
      DAT_00d056d4 = DAT_00d056d4 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar5) = 0x101;
      if (0x1f < DAT_00d056d4) {
        DAT_00d056d4 = 0;
        return;
      }
    }
  }
  return;
}
