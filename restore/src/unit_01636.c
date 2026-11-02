#include "restore_types.h"
#define DAT_005e0274 (*(undefined4 *)0x005e0274u)
#define DAT_00605a38 (*(undefined *)0x00605a38u)
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00605a44 (*(undefined *)0x00605a44u)
#define DAT_00705698 (*(undefined *)0x00705698u)
#define DAT_0070569c (*(undefined *)0x0070569cu)
#define DAT_007056a0 (*(undefined *)0x007056a0u)
#define DAT_007056a8 (*(undefined *)0x007056a8u)
#define DAT_007056b0 (*(undefined *)0x007056b0u)
#define DAT_007056b8 (*(undefined *)0x007056b8u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_007848e2 (*(undefined *)0x007848e2u)
#define DAT_007848e4 (*(undefined *)0x007848e4u)
#define DAT_007848e6 (*(undefined *)0x007848e6u)
#define DAT_007848e8 (*(undefined *)0x007848e8u)
#define DAT_007848ea (*(undefined *)0x007848eau)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2c4 (*(undefined *)0x00bca2c4u)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00d3a6b4 (*(undefined *)0x00d3a6b4u)
#define DAT_00d3a6b8 (*(undefined *)0x00d3a6b8u)
#define DAT_00d3a6bc (*(undefined *)0x00d3a6bcu)
#define DAT_00d3a6c0 (*(undefined *)0x00d3a6c0u)
#define DAT_00d3a6c4 (*(undefined *)0x00d3a6c4u)
#define DAT_00d3a6c8 (*(undefined *)0x00d3a6c8u)
#define DAT_00d3a6e4 (*(undefined *)0x00d3a6e4u)
#define DAT_00d3b130 (*(undefined *)0x00d3b130u)
#define DAT_00d3b138 (*(undefined *)0x00d3b138u)
#define DAT_00d3b13c (*(undefined *)0x00d3b13cu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38eac (*(undefined *)0x00e38eacu)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00e92e0c (*(undefined *)0x00e92e0cu)
#define DAT_00e94e0c (*(undefined *)0x00e94e0cu)
#define DAT_00e95120 (*(undefined *)0x00e95120u)
#define DAT_00e951e8 (*(undefined *)0x00e951e8u)
#define DAT_00e951ec (*(undefined *)0x00e951ecu)
#define DAT_00ea620c (*(undefined4 *)0x00ea620cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadd50 (*(undefined *)0x00eadd50u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00f53f78 (*(undefined *)0x00f53f78u)
#define _DAT_00d3a6e0 (*(undefined4 *)0x00d3a6e0u)
extern int FUN_004c92a0();
extern int FUN_004c9340();
extern int FUN_004c95d0();

undefined4 FUN_004c8750(void)

{
  short sVar1;
  int iVar2;
  int iVar3;
  undefined1 local_1;
  
  if (DAT_00e38dca != '\0') {
    return 0;
  }
  if (DAT_00e38eac == 0) {
    DAT_00d3a6c8 = DAT_00e92e0c;
    return 0;
  }
  iVar3 = DAT_00e92e0c - DAT_00d3a6c8;
  if (DAT_00e38dda == 1) {
    if (DAT_00605a3c == 1) {
      sVar1 = *(short *)(&DAT_00b350dc + DAT_00bca2c4 * 2);
      if (((0 < DAT_00605a38) && (0 < DAT_007056c8)) && (DAT_007056c8 < 9)) {
        DAT_00d3a6e4 = DAT_007056c8 + 0xa6;
      }
      if (DAT_007056b8 == 0xe) {
        DAT_00d3a6e4 = 0xb1;
      }
      else {
        if ((0 < DAT_007056b8) && (DAT_007056b8 < 0x11)) {
          DAT_00d3a6e4 = DAT_007056b8 + 0x90;
        }
        if (DAT_007056b8 == 0x14) {
          DAT_00d3a6e4 = 0xaf;
        }
        else if (DAT_007056b8 == 0x15) {
          DAT_00d3a6e4 = 0xb0;
        }
      }
      while( true ) {
        if (sVar1 == 0) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        iVar3 = sVar1 * 0x184;
        if ((&DAT_00bcd602)[iVar3] == '$') break;
        sVar1 = *(short *)(&DAT_00bcd600 + iVar3);
      }
      DAT_00d3a6bc = DAT_00d3b130;
      DAT_00d3a6e4 = 1;
      DAT_00d3a6c0 = 4;
      DAT_00d3a6c4 = 1;
      DAT_00d3a6b8 = DAT_00d3b13c;
      DAT_00d3a6b4 = DAT_00d3b138;
      DAT_00e38dca = 2;
      return 1;
    }
    if (DAT_00f53f78 != 0) {
      if (DAT_00f53f78 != 1) {
        return 0;
      }
      if (DAT_007056b8 == 0x14) {
        if (DAT_00705698 != 0) {
          DAT_00d3a6e4 = DAT_00705698 + 0x2b;
          goto LAB_004c9211;
        }
LAB_004c8a5d: ;
        DAT_00d3a6e4 = 0xc;
        goto LAB_004c9211;
      }
      if (DAT_007056b8 < 10) {
        if (0x1d < DAT_007056b8) goto LAB_004c8a71;
      }
      else {
        if (DAT_007056b8 < 0x1e) goto LAB_004c8a5d;
LAB_004c8a71: ;
        if (DAT_007056b8 < 0x32) {
          DAT_00d3a6e4 = DAT_007056b8 + -10;
          goto LAB_004c9211;
        }
      }
      if (DAT_007056a8 == 0) {
        if (DAT_0070569c != 0) {
          DAT_00d3a6e4 = DAT_0070569c + 0x3c;
          goto LAB_004c9211;
        }
        goto joined_r0x004c8ab7;
      }
      DAT_00d3a6e4 = DAT_007056a8 + 0x32;
      goto LAB_004c9211;
    }
    if (DAT_007056b8 == 0x14) {
      DAT_00d3a6e4 = DAT_007056b8;
      if (DAT_00705698 != 0) {
        DAT_00d3a6e4 = DAT_00705698 + 0x2b;
      }
      goto LAB_004c9211;
    }
    if (DAT_007056b8 < 10) {
      if (0x1d < DAT_007056b8) goto LAB_004c88f3;
      if (0x31 < DAT_007056b8) goto LAB_004c891b;
    }
    else {
      if (DAT_007056b8 < 0x1e) {
        DAT_00d3a6e4 = DAT_007056b8;
        goto LAB_004c9211;
      }
LAB_004c88f3: ;
      if (DAT_007056b8 < 0x32) {
        if ((DAT_007056b8 != 0x2d) || (DAT_00e38ebc == 0)) {
          DAT_00d3a6e4 = DAT_007056b8 + -10;
        }
        goto LAB_004c9211;
      }
LAB_004c891b: ;
      if (DAT_007056b8 < 0x46) goto LAB_004c8f7e;
    }
    if (DAT_007056a0 == 0) {
      if (DAT_007056a8 == 0) {
        if (DAT_0070569c == 0) {
joined_r0x004c8ab7: ;
          if (DAT_00bca2c4 == 0) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if ((DAT_00e94e0c != 0) && (DAT_00e94e0c != 0x33)) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if ((*(byte *)(&DAT_00af598c + DAT_00bca2c4) & 0x40) != 0) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          sVar1 = (&DAT_007999e8)[*(short *)(&DAT_00b4e6fc + DAT_00bca2c4 * 2) * 0x84];
          if (sVar1 == 0xd1) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (*(short *)(&DAT_00b4e6fc + DAT_00bca2c4 * 2) < 1) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if ((((sVar1 != 0x55) && (sVar1 != 0xbb)) && (sVar1 != 0xbc)) && (sVar1 != 0xbd)) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          DAT_00d3a6bc = DAT_00d3b130;
          DAT_00d3a6c0 = 2;
          DAT_00e38dca = 2;
          DAT_00d3a6e4 = 1;
          DAT_00d3a6c4 = 1;
          DAT_00d3a6b8 = DAT_00d3b13c;
          DAT_00d3a6b4 = DAT_00d3b138;
          return 1;
        }
        DAT_00d3a6e4 = DAT_0070569c + 0x3c;
      }
      else {
        DAT_00d3a6e4 = DAT_007056a8 + 0x32;
      }
    }
    else {
      DAT_00d3a6e4 = 0x24;
    }
    goto LAB_004c9211;
  }
  if (DAT_00e38dda != 7) {
    if (DAT_00e38dda == 0x2a) {
      if (DAT_007056c8 == 1) {
        DAT_00d3a6e4 = 0x5e;
      }
      else {
        if (DAT_007056c8 == 0) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        DAT_00d3a6e4 = 0x5f;
      }
      goto LAB_004c9211;
    }
    if (DAT_00e38dda == 0x2c) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    if (DAT_00e38dda == 0x4d) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    if (DAT_00e38dda == 0x2d) {
      if (DAT_007056c8 == 1) {
        DAT_00d3a6e4 = 100;
      }
      else if (DAT_007056c8 == 2) {
        DAT_00d3a6e4 = 0x65;
      }
      else if (DAT_007056c8 == 0) {
        if (DAT_007056b0 == 0) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        DAT_00d3a6e4 = 0x67;
      }
      else {
        DAT_00d3a6e4 = 0x66;
      }
      goto LAB_004c9211;
    }
    if (DAT_00e38dda == 0x28) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    if (DAT_00e38dda == 0x2b) {
      if (DAT_007056b8 == 10) {
        DAT_00d3a6e4 = 1;
        goto LAB_004c9211;
      }
      if (DAT_007056b8 != 0xb) {
        if (DAT_007056b8 == 0xc) {
          DAT_00d3a6e4 = 0x73;
          goto LAB_004c9211;
        }
        if (DAT_007056b8 == 0xd) {
          DAT_00d3a6e4 = 0x74;
          goto LAB_004c9211;
        }
joined_r0x004c8e17: ;
        if (DAT_007056c8 == 1) {
          if (DAT_007848e2 != 0) {
            DAT_00d3a6e4 = 0x75;
          }
        }
        else if (DAT_007056c8 == 2) {
          if (DAT_007848e4 != 0) {
            DAT_00d3a6e4 = 0x76;
          }
        }
        else if (DAT_007056c8 == 3) {
          if (DAT_007848e6 != 0) {
            DAT_00d3a6e4 = 0x77;
          }
        }
        else if (DAT_007056c8 == 4) {
          if (DAT_007848e8 != 0) {
            DAT_00d3a6e4 = 0x78;
          }
        }
        else {
          if (DAT_007056c8 != 5) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (DAT_007848ea != 0) {
            DAT_00d3a6e4 = 0x79;
          }
        }
        goto LAB_004c9211;
      }
LAB_004c920b: ;
      DAT_00d3a6e4 = 2;
      goto LAB_004c9211;
    }
    if (DAT_00e38dda == 0x52) {
      if (DAT_007056b8 == 10) {
        DAT_00d3a6e4 = 1;
        goto LAB_004c9211;
      }
      if (DAT_007056b8 != 0xb) {
        if (DAT_007056b8 == 0xc) {
          DAT_00d3a6e4 = 0x73;
          goto LAB_004c9211;
        }
        if (DAT_007056b8 == 0xd) {
          DAT_00d3a6e4 = 0x74;
          goto LAB_004c9211;
        }
        goto joined_r0x004c8e17;
      }
      goto LAB_004c920b;
    }
    if (DAT_00e38dda != 9) {
      if (DAT_00e38dda == 0xd) {
        return 0;
      }
      if (DAT_00e38dda == 0x10) {
        return 0;
      }
      if (DAT_00e38dda == 0x14) {
        iVar2 = FUN_004c9340(&local_1);
        if (iVar2 == 0) {
          if (DAT_007056b8 == 0xc) {
            DAT_00d3a6e4 = 0x46;
          }
          else {
            if ((DAT_007056b8 < 10) || (0x1d < DAT_007056b8)) {
              DAT_00d3a6c8 = DAT_00e92e0c;
              return 0;
            }
            DAT_00d3a6e4 = DAT_007056b8 + -9;
          }
        }
        else {
          DAT_00d3a6e4 = iVar2 + 0x85;
        }
        goto LAB_004c9211;
      }
      if (DAT_00e38dda == 0x15) {
        iVar2 = FUN_004c95d0(&local_1);
        if (iVar2 == 0) {
          if (DAT_007056b8 == 0xc) {
            DAT_00d3a6e4 = 0x46;
          }
          else if ((DAT_007056b8 < 10) || (0x1d < DAT_007056b8)) {
            if ((DAT_007056c8 < 1) || (8 < DAT_007056c8)) {
              if (DAT_007056c8 != 10) {
                DAT_00d3a6c8 = DAT_00e92e0c;
                return 0;
              }
              DAT_00d3a6e4 = 0xa6;
            }
            else {
              DAT_00d3a6e4 = DAT_007056c8 + 0x9d;
            }
          }
          else {
            DAT_00d3a6e4 = DAT_007056b8 + -9;
          }
        }
        else {
          DAT_00d3a6e4 = iVar2 + 0x85;
        }
        goto LAB_004c9211;
      }
      if (DAT_00e38dda != 0x11) {
        if (DAT_00e38dda == 5) {
          if (DAT_007056b8 == 10) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (DAT_007056b8 == 0xb) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (DAT_007056b8 == 0xc) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (DAT_007056b8 == 0xd) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (DAT_007056b8 == 0xe) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        if (DAT_00e38dda != 0x4e) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        iVar2 = FUN_004c92a0();
        if (iVar2 == 0) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        DAT_00d3a6e4 = iVar2 + 0x85;
        goto LAB_004c9211;
      }
      if (DAT_007056b8 == 10) {
        DAT_00d3a6e4 = 1;
        goto LAB_004c9211;
      }
      if (DAT_007056b8 != 0xb) {
        if (DAT_007056c8 == 0) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        DAT_00e951e8 = DAT_00e95120 + -1 + DAT_007056c8;
        if (DAT_00e951ec <= DAT_00e951e8) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        goto LAB_004c9211;
      }
      goto LAB_004c920b;
    }
    if (((DAT_00eadd50 != 4) && (DAT_00eadd50 != 6)) && (DAT_00eadd50 != 5)) {
      if ((DAT_007056b8 < 10) || (0x1d < DAT_007056b8)) {
        if ((DAT_007056b8 < 0x28) || (0x36 < DAT_007056b8)) {
          if (DAT_00eadd50 != 2) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if ((short)(&DAT_007999e8)[DAT_00eadd6c * 0x84] < 10) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (0x1d < (short)(&DAT_007999e8)[DAT_00eadd6c * 0x84]) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          if (DAT_007056c8 == 0) {
            DAT_00d3a6c8 = DAT_00e92e0c;
            return 0;
          }
          DAT_00d3a6e4 = 0x7b;
        }
        else {
          DAT_00d3a6e4 = *(int *)(&DAT_005e0274 + DAT_007056b8 * 4);
        }
      }
      else {
        DAT_00d3a6e4 = DAT_007056b8 + -9;
      }
      goto LAB_004c9211;
    }
    if ((DAT_007056b8 < 10) || (0x1d < DAT_007056b8)) goto LAB_004c9211;
LAB_004c8f7e: ;
    DAT_00d3a6e4 = DAT_007056b8 + -9;
    goto LAB_004c9211;
  }
  if (DAT_007056b8 < 10) {
    if (0x18 < DAT_007056b8) goto LAB_004c8af9;
  }
  else {
    if (DAT_007056b8 < 0x19) {
      DAT_00d3a6e4 = DAT_007056b8 + 0x3d;
      goto LAB_004c9211;
    }
LAB_004c8af9: ;
    if (DAT_007056b8 < 0x1e) {
      DAT_00d3a6e4 = DAT_007056b8 + -0x18;
      goto LAB_004c9211;
    }
  }
  if (DAT_00605a44 == 1) {
    if (DAT_007056c8 == 0) {
      if (DAT_007056b0 == 0) {
        DAT_00d3a6c8 = DAT_00e92e0c;
        return 0;
      }
      DAT_00d3a6e4 = 0x5d;
    }
    else {
      DAT_00d3a6e4 = 0x5c;
    }
  }
  else if (DAT_00605a44 == 4) {
    if (DAT_007056c8 == 2) {
      if (*(int *)(&DAT_00ea620c + DAT_00eaac28 * 0x49d8) == 0) {
        return 0;
      }
    }
    else {
      if (DAT_007056c8 == 0) {
        DAT_00d3a6c8 = DAT_00e92e0c;
        return 0;
      }
      if (2 < DAT_007056c8) {
        if (DAT_007056c8 != 3) {
          DAT_00d3a6c8 = DAT_00e92e0c;
          return 0;
        }
        DAT_00d3a6e4 = 0x85;
        goto LAB_004c9211;
      }
    }
    DAT_00d3a6e4 = DAT_007056c8 + 0x5f;
  }
  else if (DAT_00605a44 == 5) {
    if (DAT_007056c8 < 1) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    if (4 < DAT_007056c8) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    DAT_00d3a6e4 = DAT_007056c8 + 0x67;
  }
  else if (DAT_00605a44 == 2) {
    if (DAT_007056c8 != 0x13) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    DAT_00d3a6e4 = 0x90;
  }
  else if (DAT_00605a44 == 3) {
    if (DAT_007056c8 != 0x13) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    DAT_00d3a6e4 = 0x8f;
  }
  else if (DAT_00605a44 == 6) {
    if (DAT_007056c8 == 3) {
      DAT_00d3a6c0 = DAT_007056c8;
      DAT_00d3a6b8 = DAT_00d3b13c;
      DAT_00e38dca = 2;
      DAT_00d3a6e4 = 1;
      DAT_00d3a6c4 = 7;
      DAT_00d3a6bc = DAT_00d3b130;
      DAT_00d3a6b4 = DAT_00d3b138;
      return 1;
    }
    if (DAT_007056c8 == 1) {
      DAT_00d3a6e4 = 0x6c;
    }
    else if (DAT_007056c8 == 2) {
      DAT_00d3a6e4 = 0x2a;
    }
    else {
      if (DAT_007056c8 == 0) {
        DAT_00d3a6c8 = DAT_00e92e0c;
        return 0;
      }
      DAT_00d3a6e4 = 0x6d;
    }
  }
  else if (DAT_00605a44 == 7) {
    if (DAT_007056c8 == 0) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    DAT_00d3a6e4 = 0x71;
  }
  else if (DAT_00605a44 == 0xb) {
    if (DAT_007056c8 == 0) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    DAT_00d3a6e4 = 0x72;
  }
  else {
    if (DAT_00605a44 != 0xc) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    if (DAT_007056b0 == 0) {
      DAT_00d3a6c8 = DAT_00e92e0c;
      return 0;
    }
    DAT_00d3a6e4 = 0x7a;
  }
LAB_004c9211: ;
  if (DAT_00e38eac != 2) {
    DAT_00d3a6c8 = DAT_00e92e0c;
    DAT_00d3a6e4 = 0;
    return 0;
  }
  if (iVar3 < 0x96) {
    DAT_00d3a6e4 = 0;
    return 0;
  }
  DAT_00d3a6b8 = DAT_00d3b13c;
  DAT_00d3a6bc = DAT_00d3b130;
  DAT_00d3a6c4 = (uint)DAT_00e38dda;
  DAT_00d3a6b4 = DAT_00d3b138;
  _DAT_00d3a6e0 = 0;
  DAT_00d3a6c0 = 0;
  if (DAT_00e38dca == '\0') {
    DAT_00e38dca = '\x02';
  }
  return 1;
}
