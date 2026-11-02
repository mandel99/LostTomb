#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a82 (*(undefined4 *)0x00799a82u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00f565fc (*(undefined *)0x00f565fcu)
#define DAT_00f56a4c (*(undefined *)0x00f56a4cu)
#define DAT_00f56a54 (*(undefined *)0x00f56a54u)
#define DAT_00f56a5c (*(undefined *)0x00f56a5cu)
#define DAT_00f56a64 (*(undefined *)0x00f56a64u)
#define DAT_00f56a6c (*(undefined *)0x00f56a6cu)
#define DAT_00f56a74 (*(undefined *)0x00f56a74u)
#define DAT_00f56a7c (*(undefined *)0x00f56a7cu)
#define DAT_00f56a84 (*(undefined *)0x00f56a84u)
#define DAT_00f56a8c (*(undefined *)0x00f56a8cu)
#define DAT_00f56a94 (*(undefined *)0x00f56a94u)
#define DAT_00f56a9c (*(undefined *)0x00f56a9cu)
#define DAT_00f56aa4 (*(undefined *)0x00f56aa4u)
#define DAT_00f56aac (*(undefined *)0x00f56aacu)
#define DAT_00f572c4 (*(undefined *)0x00f572c4u)
#define DAT_00f572cc (*(undefined *)0x00f572ccu)
#define DAT_00f572d4 (*(undefined *)0x00f572d4u)
#define DAT_00f572f4 (*(undefined *)0x00f572f4u)
extern int FUN_0053e590();

void FUN_00436310(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int local_20 [8];
  
  iVar1 = ((short)(&DAT_00799a14)[param_1 * 0x84] + -100) / 100;
  iVar2 = (*(short *)(&DAT_00799aa4 + param_1 * 0x42) + -100) / 100;
  switch((&DAT_007999e8)[param_1 * 0x84]) {
  case 0x20: ;
    if (-1 < iVar1) {
      if (2 < iVar1) {
        iVar1 = 2;
      }
      FUN_0053e590(iVar1 + DAT_00f56a4c,param_2 + 0x6b,param_3 + 5);
      return;
    }
    break;
  case 0x2f: ;
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(iVar1 + DAT_00f56a54,param_2 + 0x31,param_3 + 0x10);
      return;
    }
    break;
  case 0x33: ;
    if (-1 < iVar1) {
      if (2 < iVar1) {
        iVar1 = 2;
      }
      FUN_0053e590(iVar1 + DAT_00f565fc,param_2 + 0x1c,param_3 + 0xf);
      return;
    }
    break;
  case 0x35: ;
    if (-1 < iVar1) {
      if (2 < iVar1) {
        iVar1 = 2;
      }
      FUN_0053e590(iVar1 + DAT_00f565fc,param_2 + 0x3d,param_3 + -0xd);
      return;
    }
    break;
  case 0x4a: ;
    if (-1 < iVar1) {
      local_20[0] = 0x46;
      local_20[1] = 0x19;
      local_20[2] = 0x78;
      iVar2 = (uint)*(byte *)(&DAT_00799a82 + param_1 * 0x84) - DAT_00bca2e0 / 2;
      local_20[3] = 0xffffffe7;
      local_20[4] = 0x5a;
      local_20[5] = 10;
      local_20[6] = 0x28;
      local_20[7] = 0xfffffffb;
      if (iVar2 < 0) {
        iVar2 = iVar2 + 4;
      }
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(iVar1 + DAT_00f56a6c,local_20[iVar2 * 2] + param_2,
                   local_20[iVar2 * 2 + 1] + param_3);
      return;
    }
    break;
  case 0x5f: ;
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(iVar1 + DAT_00f56a64,param_2 + 0x83,param_3 + 2);
    }
    if (-1 < iVar2) {
      if (1 < iVar2) {
        iVar2 = 1;
      }
      FUN_0053e590(iVar2 + DAT_00f56aa4,param_2 + 0x4a,param_3 + 10);
      return;
    }
    break;
  case 0x6c: ;
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(iVar1 + DAT_00f56a6c,param_2 + 0x41,param_3 + 3);
      return;
    }
    break;
  case 0x6e: ;
    if (-1 < iVar1) {
      if (2 < iVar1) {
        iVar1 = 2;
      }
      FUN_0053e590(DAT_00f56a9c + iVar1,param_2 + 0x2b,param_3 + -1);
      return;
    }
    break;
  case 0x6f: ;
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(DAT_00f56aac + iVar1,param_2 + 0x29,param_3 + 1);
      return;
    }
    break;
  case 0x70: ;
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(iVar1 + DAT_00f56a74,param_2 + 0x45,param_3 + 0x13);
      return;
    }
    break;
  case 0x71: ;
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(DAT_00f56a5c + iVar1,param_2 + 0x3f,param_3 + -1);
      return;
    }
    break;
  case 0x72: ;
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(DAT_00f56a94 + iVar1,param_2 + 0x41,param_3 + 0x14);
      return;
    }
    break;
  case 0x73: ;
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(iVar1 + DAT_00f56a84,param_2 + 0x3d,param_3 + 0xd);
      return;
    }
    break;
  case 0xb1: ;
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(iVar1 + DAT_00f56a6c,param_2 + 0x29,param_3 + 8);
      return;
    }
    break;
  case 0xc2: ;
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(iVar1 + DAT_00f56a7c,param_2 + 0x26,param_3 + 7);
      return;
    }
    break;
  case 0xc3: ;
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(DAT_00f56a8c + iVar1,param_2 + 0x22,param_3 + 0x11);
      return;
    }
    break;
  case 0xcb: ;
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(DAT_00f56a8c + iVar1,param_2 + 0x23,param_3 + 4);
      return;
    }
    break;
  case 0xcc: ;
    if (-1 < iVar2) {
      if (1 < iVar2) {
        iVar2 = 1;
      }
      FUN_0053e590(iVar2 + DAT_00f56a7c,param_2 + 0x5b,param_3 + 2);
    }
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(DAT_00f56a94 + iVar1,param_2 + 0x34,param_3 + -0x11);
      return;
    }
    break;
  case 0xcd: ;
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(iVar1 + DAT_00f56a6c,param_2 + 0xa6,param_3 + -0x17);
      return;
    }
    break;
  case 0xe2: ;
    if (-1 < iVar1) {
      if (2 < iVar1) {
        iVar1 = 2;
      }
      FUN_0053e590(iVar1 + DAT_00f56a84,param_2 + 200,param_3 + -10);
    }
    if (-1 < iVar2) {
      if (2 < iVar2) {
        iVar2 = 2;
      }
      FUN_0053e590(iVar2 + DAT_00f56a7c,param_2 + 0xdc,param_3 + 10);
    }
    break;
  case 0xe7: ;
    if (-1 < iVar2) {
      if (4 < iVar2) {
        iVar2 = 4;
      }
      FUN_0053e590(iVar2 + DAT_00f56a94,param_2 + 1,param_3 + 7);
    }
    if (-1 < iVar1) {
      if (4 < iVar1) {
        iVar1 = 4;
      }
      FUN_0053e590(iVar1 + DAT_00f572f4,param_2 + 0x2e,param_3 + 0xc);
      return;
    }
    break;
  case 0xe8: ;
    if (-1 < iVar2) {
      if (1 < iVar2) {
        iVar2 = 1;
      }
      FUN_0053e590(DAT_00f572c4 + iVar2,param_2 + 0x32,param_3 + 0x14);
    }
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(DAT_00f572cc + iVar1,param_2 + 0xf,param_3 + 8);
      return;
    }
    break;
  case 0xe9: ;
    if (-1 < iVar1) {
      if (1 < iVar1) {
        iVar1 = 1;
      }
      FUN_0053e590(DAT_00f572d4 + iVar1,param_2 + 0x2d,param_3 + 0x19);
      return;
    }
  }
  return;
}
