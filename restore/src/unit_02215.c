#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00eadd4c (*(undefined *)0x00eadd4cu)
#define DAT_00eadd50 (*(undefined *)0x00eadd50u)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
extern int FUN_00488030();

byte FUN_004f8960(void)

{
  short sVar1;
  char cVar2;
  
  if (DAT_00eadd50 != 0) {
    if (DAT_00eadd50 == 1) {
      if (DAT_00eadd4c == 0xc) {
        return 1;
      }
      if (DAT_00eadd4c == 9) {
        return 1;
      }
      if (DAT_00eadd4c == 0x17) {
        return 1;
      }
      if (DAT_00eadd4c == 0x18) {
        return 1;
      }
      if (DAT_00eadd4c == 8) {
        return 1;
      }
      return (DAT_00eadd4c != 7) - 1U & 4;
    }
    if ((((DAT_00eadd50 != 4) && (DAT_00eadd50 != 6)) && (DAT_00eadd50 != 5)) && (DAT_00eadd50 == 2)
       ) {
      sVar1 = (&DAT_007999e8)[DAT_00eadd6c * 0x84];
      if ((0x8b < sVar1) && (sVar1 < 0x96)) {
        return 1;
      }
      if (sVar1 == 0x3c) {
        return 1;
      }
      cVar2 = FUN_00488030(DAT_00eadd6c);
      if (cVar2 != '\0') {
        return 5;
      }
      sVar1 = (&DAT_007999e8)[DAT_00eadd6c * 0x84];
      if (sVar1 == 0x3d) {
        return 1;
      }
      if (sVar1 == 0x3e) {
        return 1;
      }
      if (sVar1 == 0x3f) {
        return 1;
      }
      if (sVar1 == 0x40) {
        return 1;
      }
      if (sVar1 == 0x41) {
        return 1;
      }
      if (sVar1 == 0x42) {
        return 1;
      }
      if (sVar1 == 0x43) {
        return 1;
      }
      if (sVar1 == 0x44) {
        return 1;
      }
      if (sVar1 == 0x45) {
        return 1;
      }
      if (sVar1 == 0x29) {
        return 1;
      }
      if (sVar1 == 0x2a) {
        return 1;
      }
      if (sVar1 == 0x2b) {
        return 1;
      }
      if (sVar1 == 0x22) {
        return 1;
      }
      if (sVar1 == 0x23) {
        return 1;
      }
      if (sVar1 == 0x24) {
        return 1;
      }
      if (sVar1 == 0x25) {
        return 1;
      }
      if (sVar1 == 0xd1) {
        return 1;
      }
      if (sVar1 == 0x2e) {
        return 1;
      }
      if (sVar1 == 0x2f) {
        return 1;
      }
      if (sVar1 == 0xce) {
        return 1;
      }
      if (sVar1 == 0x31) {
        return 1;
      }
      if (sVar1 == 0x62) {
        return 1;
      }
      if (sVar1 == 99) {
        return 1;
      }
      if (sVar1 == 0x5a) {
        return 1;
      }
      if (sVar1 == 0x5c) {
        return 4;
      }
      if (sVar1 == 0xb4) {
        return 1;
      }
      if (sVar1 == 0x58) {
        return 1;
      }
      if (sVar1 == 0x59) {
        return 1;
      }
      if (sVar1 == 0x5d) {
        return 1;
      }
      if (sVar1 == 0x50) {
        return 1;
      }
      if (sVar1 == 0x1e) {
        return 3;
      }
      if (sVar1 == 0x1f) {
        return 2;
      }
      if (sVar1 == 0x20) {
        return 2;
      }
      if (sVar1 == 0xe2) {
        return 2;
      }
      if (sVar1 == 0x21) {
        return 2;
      }
      if (sVar1 == 0x37) {
        return 1;
      }
      if (sVar1 == 0x51) {
        return 1;
      }
      if (sVar1 == 0x33) {
        return 1;
      }
      if (sVar1 == 0x35) {
        return 1;
      }
      if (sVar1 == 0x3a) {
        return 1;
      }
      if (sVar1 == 0xaa) {
        return 1;
      }
      if (sVar1 == 0xab) {
        return 1;
      }
      if (sVar1 == 200) {
        return 1;
      }
      if (sVar1 == 0xc9) {
        return 1;
      }
      if (sVar1 == 0xca) {
        return 1;
      }
      if (sVar1 == 0x3b) {
        return 1;
      }
      if (sVar1 == 0xac) {
        return 1;
      }
      if (sVar1 != 0xad) {
        if (sVar1 == 0x39) {
          return 1;
        }
        if (sVar1 == 0x5e) {
          return 1;
        }
        if (sVar1 == 0xb9) {
          return 1;
        }
        if (sVar1 == 0xba) {
          return 1;
        }
        if (sVar1 == 0x5f) {
          return 1;
        }
        if (sVar1 == 0x46) {
          return 1;
        }
        if (sVar1 == 0x47) {
          return 2;
        }
        if (sVar1 == 0xbe) {
          return 1;
        }
        if (sVar1 == 0xbf) {
          return 2;
        }
        if (sVar1 == 0x4a) {
          return 1;
        }
        if (sVar1 == 0x4b) {
          return 1;
        }
        if (sVar1 == 0xc0) {
          return 1;
        }
        if (sVar1 == 0x4c) {
          return 1;
        }
        if (sVar1 == 0xb5) {
          return 1;
        }
        if (sVar1 == 0xb6) {
          return 1;
        }
        if (sVar1 == 0x4d) {
          return 1;
        }
        if (sVar1 == 0x4e) {
          return 1;
        }
        if (sVar1 == 0x4f) {
          return 1;
        }
        if (sVar1 == 0xbb) {
          return 1;
        }
        if (sVar1 == 0xbc) {
          return 1;
        }
        if (sVar1 == 0xbd) {
          return 1;
        }
        if (sVar1 == 0x54) {
          return 2;
        }
        if (sVar1 == 0x55) {
          return 2;
        }
        if (sVar1 == 0x56) {
          return 1;
        }
        if (sVar1 == 0x57) {
          return 1;
        }
        if (sVar1 == 0x6e) {
          return 1;
        }
        if (sVar1 == 0x6f) {
          return 1;
        }
        if (sVar1 == 0x70) {
          return 1;
        }
        if (sVar1 == 0x71) {
          return 1;
        }
        if (sVar1 == 0x72) {
          return 1;
        }
        if (sVar1 == 0xcb) {
          return 1;
        }
        if (sVar1 == 0xcc) {
          return 1;
        }
        if (sVar1 == 0xe8) {
          return 1;
        }
        if (sVar1 == 0xe9) {
          return 1;
        }
        if (sVar1 == 0xcd) {
          return 1;
        }
        if (sVar1 == 0xb1) {
          return 1;
        }
        if (sVar1 == 0xb2) {
          return 1;
        }
        if (sVar1 == 0xb3) {
          return 1;
        }
        if (sVar1 == 0xe7) {
          return 1;
        }
      }
    }
  }
  return 0;
}
