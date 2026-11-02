#include "restore_types.h"
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_00784de0 (*(undefined *)0x00784de0u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00ea5fac (*(undefined4 *)0x00ea5facu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004df040();
extern void FUN_004f1310();
extern int FUN_00542ad0();

int FUN_004f57b0(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  undefined1 local_d8 [32];
  int local_b8;
  int local_b4;
  undefined1 local_68 [28];
  int local_4c;
  undefined1 local_38 [44];
  int local_c;
  
  iVar4 = 0;
  iVar3 = 0;
  do {
    iVar2 = *(int *)(&DAT_00ea5fac + (DAT_00eaac28 * 0x1276 + iVar3) * 4);
    if ((iVar2 == 0) || ((&DAT_007999d8)[iVar2 * 0x108] != '\x01')) {
      switch((&DAT_00784de0)[iVar3]) {
      case 1: ;
      case 0xd: ;
        if (param_1 == 0x18) {
          iVar4 = iVar4 + 0x12c0;
        }
        else {
LAB_004f5a45: ;
          if (param_1 == 0x19) {
            iVar4 = iVar4 + 0x4b00;
          }
        }
        break;
      case 2: ;
      case 0xe: ;
        if (param_1 == 0x18) {
          iVar4 = iVar4 + 0x5460;
        }
        else {
LAB_004f5a69: ;
          if (param_1 == 0x19) {
            iVar4 = iVar4 + 0xa8c0;
          }
        }
        break;
      case 3: ;
        if (param_1 != 0xc) goto LAB_004f5a45;
        iVar4 = iVar4 + 0x12c0;
        break;
      case 4: ;
        if (param_1 != 0xc) goto LAB_004f5a69;
        iVar4 = iVar4 + 0x5460;
        break;
      case 5: ;
        if (param_1 == 0xc) {
          iVar4 = iVar4 + 0x10680;
        }
        else {
LAB_004f5a8d: ;
          if (param_1 == 0x19) {
            iVar4 = iVar4 + 0x12c00;
          }
        }
        break;
      case 6: ;
        if (param_1 == 0xc) {
          iVar4 = iVar4 + 0x23280;
        }
        else {
LAB_004f5ab1: ;
          if (param_1 == 0x19) {
            iVar4 = iVar4 + 120000;
          }
        }
        break;
      case 7: ;
        if (param_1 == 0xc) {
          iVar4 = iVar4 + 0x40740;
        }
        else {
LAB_004f5acb: ;
          if (param_1 == 0x19) {
            iVar4 = iVar4 + 160000;
          }
        }
        break;
      case 8: ;
        if (param_1 == 0x18) {
          iVar4 = iVar4 + 0x12c0;
        }
        break;
      case 9: ;
        if (param_1 == 0x18) {
          iVar4 = iVar4 + 0x5460;
        }
        break;
      case 10: ;
        if (param_1 == 0x18) {
          iVar4 = iVar4 + 0x10680;
        }
        break;
      case 0xb: ;
        if (param_1 == 0x18) {
          iVar4 = iVar4 + 0x23280;
        }
        break;
      case 0xc: ;
        if (param_1 == 0x18) {
          iVar4 = iVar4 + 0x40740;
        }
        break;
      case 0xf: ;
        if (param_1 != 0x18) goto LAB_004f5a8d;
        iVar4 = iVar4 + 0x10680;
        break;
      case 0x10: ;
        if (param_1 != 0x18) goto LAB_004f5ab1;
        iVar4 = iVar4 + 0x23280;
        break;
      case 0x11: ;
        if (param_1 != 0x18) goto LAB_004f5acb;
        iVar4 = iVar4 + 0x40740;
        break;
      case 0x12: ;
        if (param_1 == 0xc) {
          iVar4 = iVar4 + 0x2a30;
        }
        break;
      case 0x13: ;
        if (param_1 == 0xc) {
          iVar4 = iVar4 + 24000;
        }
        break;
      case 0x14: ;
        if (param_1 == 0xc) {
          iVar4 = iVar4 + 42000;
        }
        break;
      case 0x16: ;
        if (param_1 == 0x1a) {
          iVar4 = iVar4 + 10000;
        }
        break;
      case 0x17: ;
        if (param_1 == 0x1a) {
          iVar4 = iVar4 + 20000;
        }
        break;
      case 0x18: ;
        if (param_1 == 0x1e) {
          iVar4 = iVar4 + 38000;
        }
        break;
      case 0x19: ;
      case 0x1a: ;
      case 0x1b: ;
        if (param_1 == 0x1e) {
          iVar4 = iVar4 + 0xc1c0;
        }
      }
    }
    else {
      sVar1 = (&DAT_007999e8)[iVar2 * 0x84];
      if (sVar1 == 0xb7) {
        FUN_004f1310(iVar2,local_d8);
        switch((int)(DAT_005e4ef4)) {
        case 1: ;
        case 4: ;
          iVar2 = local_b4;
          if (param_1 == 0x19) {
LAB_004f58de: ;
            if (0 < iVar2) {
              iVar4 = iVar4 + iVar2 * 400;
            }
          }
          else if ((param_1 == 0x18) && (0 < local_b8)) {
            iVar4 = iVar4 + local_b8 * 400;
          }
          break;
        case 2: ;
          if (param_1 == 0x19) {
            if (0 < local_b4) {
              iVar4 = iVar4 + local_b4 * 400;
            }
          }
          else {
            iVar2 = local_b8;
            if (param_1 == 0xc) goto LAB_004f58de;
          }
          break;
        case 3: ;
          if ((param_1 == 0x18) && ((0 < local_b8 || (0 < local_b4)))) {
            iVar4 = iVar4 + (local_b8 + local_b4) * 400;
          }
          break;
        case 5: ;
          if ((param_1 == 0xc) && ((0 < local_b8 || (0 < local_b4)))) {
            iVar4 = iVar4 + (local_b8 + local_b4) * 400;
          }
        }
      }
      else if (sVar1 == 0xd5) {
        FUN_00542ad0(iVar2,local_38);
        if (param_1 == 0x1e) {
          iVar4 = iVar4 + local_c;
        }
      }
      else if ((sVar1 == 0xde) && (FUN_004df040(iVar2,local_68), param_1 == 0x1e)) {
        iVar4 = iVar4 + local_4c;
      }
    }
    iVar3 = iVar3 + 1;
    if (2 < iVar3) {
      return iVar4;
    }
  } while( true );
}
