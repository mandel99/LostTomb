#include "restore_types.h"
#define DAT_00bcba32 (*(undefined4 *)0x00bcba32u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd668 (*(undefined4 *)0x00bcd668u)
#define DAT_00bcd690 (*(undefined4 *)0x00bcd690u)
#define DAT_00d05640 (*(undefined *)0x00d05640u)
#define DAT_00d05644 (*(undefined *)0x00d05644u)
#define DAT_00d05648 (*(undefined *)0x00d05648u)
#define DAT_00d0564c (*(undefined *)0x00d0564cu)
#define DAT_00d05650 (*(undefined *)0x00d05650u)
#define DAT_00d05654 (*(undefined *)0x00d05654u)
#define DAT_00d05658 (*(undefined *)0x00d05658u)
#define DAT_00d0565c (*(undefined *)0x00d0565cu)
#define DAT_00d05660 (*(undefined *)0x00d05660u)
#define DAT_00d05664 (*(undefined *)0x00d05664u)
#define DAT_00d05668 (*(undefined *)0x00d05668u)
#define DAT_00d0566c (*(undefined *)0x00d0566cu)
#define DAT_00d05670 (*(undefined *)0x00d05670u)
#define DAT_00d05674 (*(undefined *)0x00d05674u)
#define DAT_00d05678 (*(undefined *)0x00d05678u)
#define DAT_00d0567c (*(undefined *)0x00d0567cu)
#define DAT_00d05680 (*(undefined *)0x00d05680u)
#define DAT_00d05684 (*(undefined *)0x00d05684u)
#define DAT_00d05688 (*(undefined *)0x00d05688u)
#define DAT_00d0568c (*(undefined *)0x00d0568cu)
#define DAT_00d05690 (*(undefined *)0x00d05690u)
#define DAT_00d05694 (*(undefined *)0x00d05694u)
#define DAT_00d05698 (*(undefined *)0x00d05698u)
#define DAT_00d0569c (*(undefined *)0x00d0569cu)
#define DAT_00d056b4 (*(undefined *)0x00d056b4u)
#define DAT_00d056b8 (*(undefined *)0x00d056b8u)
#define DAT_00d056bc (*(undefined *)0x00d056bcu)
#define DAT_00d056c0 (*(undefined *)0x00d056c0u)
#define DAT_00d056c4 (*(undefined *)0x00d056c4u)
#define DAT_00d056c8 (*(undefined *)0x00d056c8u)
#define DAT_00d056cc (*(undefined *)0x00d056ccu)
#define DAT_00d056d0 (*(undefined *)0x00d056d0u)
#define DAT_00d056d4 (*(undefined *)0x00d056d4u)
#define DAT_00d056d8 (*(undefined *)0x00d056d8u)
#define DAT_00d056dc (*(undefined *)0x00d056dcu)
#define DAT_00d056e0 (*(undefined *)0x00d056e0u)

void FUN_004b5af0(int param_1)

{
  char cVar1;
  int iVar2;
  
  iVar2 = param_1 * 0x184;
  cVar1 = (&DAT_00bcd602)[iVar2];
  if ((((cVar1 != '\x06') && (cVar1 != '\x0e')) && (cVar1 != 'A')) &&
     (((cVar1 != '_' && (cVar1 != '?')) && ((cVar1 != 'H' && (cVar1 != 'a')))))) {
    switch((int)(cVar1)) {
    case '\x01': ;
    case '\x02': ;
    case '\x04': ;
    case '\x05': ;
    case '\t': ;
    case '#': ;
    case '&': ;
    case '6': ;
    case '7': ;
    case '8': ;
    case 'K': ;
    case 'O': ;
    case 'P': ;
    case 'Q': ;
    case 'U': ;
    case 'W': ;
    case 'Z': ;
    case '`': ;
    case 'b': ;
    case 'l': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056e0;
      DAT_00d056e0 = DAT_00d056e0 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0xfe;
      if (0x7f < DAT_00d056e0) {
        DAT_00d056e0 = 0;
        return;
      }
      break;
    case '\x03': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056dc;
      DAT_00d056dc = DAT_00d056dc + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0xff;
      if (0x1f < DAT_00d056dc) {
        DAT_00d056dc = 0;
        return;
      }
      break;
default: ;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0xfffffffe;
      break;
    case '\a': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05684;
      DAT_00d05684 = DAT_00d05684 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x115;
      if (0xf < DAT_00d05684) {
        DAT_00d05684 = 0;
        return;
      }
      break;
    case '\b': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d0568c;
      DAT_00d0568c = DAT_00d0568c + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x113;
      if (0xf < DAT_00d0568c) {
        DAT_00d0568c = 0;
        return;
      }
      break;
    case '\n': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05690;
      DAT_00d05690 = DAT_00d05690 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x112;
      if (0xf < DAT_00d05690) {
        DAT_00d05690 = 0;
        return;
      }
      break;
    case '\x0f': ;
    case '\x12': ;
    case 'i': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056bc;
      DAT_00d056bc = DAT_00d056bc + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x107;
      if (0x1f < DAT_00d056bc) {
        DAT_00d056bc = 0;
        return;
      }
      break;
    case '\x10': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056b8;
      DAT_00d056b8 = DAT_00d056b8 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x108;
      if (0x1f < DAT_00d056b8) {
        DAT_00d056b8 = 0;
        return;
      }
      break;
    case '\x11': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056b4;
      DAT_00d056b4 = DAT_00d056b4 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x109;
      if (0xf < DAT_00d056b4) {
        DAT_00d056b4 = 0;
        return;
      }
      break;
    case '\x13': ;
    case '\x15': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056c0;
      DAT_00d056c0 = DAT_00d056c0 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x106;
      if (0xf < DAT_00d056c0) {
        DAT_00d056c0 = 0;
        return;
      }
      break;
    case '\x14': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056c4;
      DAT_00d056c4 = DAT_00d056c4 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x105;
      if (0xf < DAT_00d056c4) {
        DAT_00d056c4 = 0;
        return;
      }
      break;
    case '\x16': ;
    case '\x17': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056d8;
      DAT_00d056d8 = DAT_00d056d8 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x100;
      if (0xf < DAT_00d056d8) {
        DAT_00d056d8 = 0;
        return;
      }
      break;
    case '\x1a': ;
    case '\'': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056cc;
      DAT_00d056cc = DAT_00d056cc + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x103;
      if (0x1f < DAT_00d056cc) {
        DAT_00d056cc = 0;
        return;
      }
      break;
    case '\x1d': ;
    case '\x1e': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d0569c;
      DAT_00d0569c = DAT_00d0569c + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x10f;
      if (0x1f < DAT_00d0569c) {
        DAT_00d0569c = 0;
        return;
      }
      break;
    case '\x1f': ;
    case ' ': ;
    case '!': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05698;
      DAT_00d05698 = DAT_00d05698 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x110;
      if (0x1f < DAT_00d05698) {
        DAT_00d05698 = 0;
        return;
      }
      break;
    case '\"': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05694;
      DAT_00d05694 = DAT_00d05694 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x111;
      if (0xf < DAT_00d05694) {
        DAT_00d05694 = 0;
        return;
      }
      break;
    case '(': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056d4;
      DAT_00d056d4 = DAT_00d056d4 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x101;
      if (0x1f < DAT_00d056d4) {
        DAT_00d056d4 = 0;
        return;
      }
      break;
    case '*': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05678;
      DAT_00d05678 = DAT_00d05678 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x118;
      if (0x1f < DAT_00d05678) {
        DAT_00d05678 = 0;
        return;
      }
      break;
    case '+': ;
    case ',': ;
    case '-': ;
      switch((&DAT_00bcba32)[(short)(&DAT_00bcd658)[param_1 * 0xc2] * 0x90]) {
      case 0: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05664;
        DAT_00d05664 = DAT_00d05664 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x11d;
        if (0xf < DAT_00d05664) {
          DAT_00d05664 = 0;
          return;
        }
        break;
      case 1: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d0565c;
        DAT_00d0565c = DAT_00d0565c + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x11f;
        if (0xf < DAT_00d0565c) {
          DAT_00d0565c = 0;
          return;
        }
        break;
      case 2: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05660;
        DAT_00d05660 = DAT_00d05660 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x11e;
        if (0xf < DAT_00d05660) {
          DAT_00d05660 = 0;
          return;
        }
        break;
      case 3: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05658;
        DAT_00d05658 = DAT_00d05658 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x120;
        if (0x10 < DAT_00d05658) {
          DAT_00d05658 = 0;
          return;
        }
        break;
      case 4: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05668;
        DAT_00d05668 = DAT_00d05668 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x11c;
        if (0xf < DAT_00d05668) {
          DAT_00d05668 = 0;
          return;
        }
        break;
      case 5: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d0566c;
        DAT_00d0566c = DAT_00d0566c + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x11b;
        if (0xf < DAT_00d0566c) {
          DAT_00d0566c = 0;
          return;
        }
        break;
      case 6: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05670;
        DAT_00d05670 = DAT_00d05670 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x11a;
        if (0xf < DAT_00d05670) {
          DAT_00d05670 = 0;
          return;
        }
        break;
      case 7: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05674;
        DAT_00d05674 = DAT_00d05674 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x119;
        if (0xf < DAT_00d05674) {
          DAT_00d05674 = 0;
          return;
        }
        break;
      case 8: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d0564c;
        DAT_00d0564c = DAT_00d0564c + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 300;
        if (0xf < DAT_00d0564c) {
          DAT_00d0564c = 0;
          return;
        }
        break;
      case 9: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05648;
        DAT_00d05648 = DAT_00d05648 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x12d;
        if (0xf < DAT_00d05648) {
          DAT_00d05648 = 0;
          return;
        }
        break;
      case 10: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05644;
        DAT_00d05644 = DAT_00d05644 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x12e;
        if (0xf < DAT_00d05644) {
          DAT_00d05644 = 0;
          return;
        }
        break;
      case 0xb: ;
        *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05640;
        DAT_00d05640 = DAT_00d05640 + 1;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x12f;
        if (0xf < DAT_00d05640) {
          DAT_00d05640 = 0;
          return;
        }
        break;
default: ;
        *(undefined2 *)(&DAT_00bcd668 + iVar2) = 0xffff;
        *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0xffffffff;
        return;
      }
      break;
    case 'B': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056c8;
      DAT_00d056c8 = DAT_00d056c8 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x104;
      if (0xf < DAT_00d056c8) {
        DAT_00d056c8 = 0;
        return;
      }
      break;
    case 'I': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d056d0;
      DAT_00d056d0 = DAT_00d056d0 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x102;
      if (0xf < DAT_00d056d0) {
        DAT_00d056d0 = 0;
        return;
      }
      break;
    case 'L': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d0567c;
      DAT_00d0567c = DAT_00d0567c + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x117;
      if (0xf < DAT_00d0567c) {
        DAT_00d0567c = 0;
        return;
      }
      break;
    case 'X': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05688;
      DAT_00d05688 = DAT_00d05688 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x114;
      if (0xf < DAT_00d05688) {
        DAT_00d05688 = 0;
        return;
      }
      break;
    case 'Y': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05680;
      DAT_00d05680 = DAT_00d05680 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x116;
      if (0xf < DAT_00d05680) {
        DAT_00d05680 = 0;
        return;
      }
      break;
    case '\\': ;
    case ']': ;
    case 'd': ;
    case 'e': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05654;
      DAT_00d05654 = DAT_00d05654 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x121;
      if (0xf < DAT_00d05654) {
        DAT_00d05654 = 0;
        return;
      }
      break;
    case 'c': ;
      *(short *)(&DAT_00bcd668 + iVar2) = (short)DAT_00d05650;
      DAT_00d05650 = DAT_00d05650 + 1;
      *(undefined4 *)(&DAT_00bcd690 + iVar2) = 0x122;
      if (0xf < DAT_00d05650) {
        DAT_00d05650 = 0;
        return;
      }
    }
  }
  return;
}
