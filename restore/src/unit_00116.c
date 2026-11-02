#include "restore_types.h"
#define DAT_00605a0c (*(undefined *)0x00605a0cu)
#define DAT_00605b38 (*(undefined4 *)0x00605b38u)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a0d (*(undefined4 *)0x00799a0du)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a80 (*(undefined4 *)0x00799a80u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00adc36c (*(undefined4 *)0x00adc36cu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_010d6a16 (*(undefined4 *)0x010d6a16u)
#define DAT_010d6a28 (*(undefined4 *)0x010d6a28u)
#define DAT_010d6a32 (*(undefined4 *)0x010d6a32u)
#define DAT_0126517c (*(undefined *)0x0126517cu)
extern undefined4 FUN_00459ef0();
extern int FUN_00475b50();
extern undefined4 FUN_0047fd30();
extern undefined4 FUN_00488050();

uint FUN_0040f2c0(int param_1,int param_2)

{
  byte bVar1;
  short sVar2;
  short sVar3;
  char cVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  bool bVar9;
  
  iVar7 = (int)*(short *)(&DAT_00b4e6fc + param_2 * 2);
  bVar1 = (&DAT_00adc36c)[param_2];
  if (iVar7 == 0) goto LAB_0040f674;
  iVar8 = iVar7 * 0x108;
  sVar2 = (&DAT_00799a18)[iVar7 * 0x84];
  cVar4 = FUN_00488050(iVar7);
  if (cVar4 != '\0') {
    if (sVar2 < 1) {
      return 0;
    }
    goto LAB_0040f674;
  }
  sVar3 = (&DAT_007999e8)[iVar7 * 0x84];
  if (sVar3 == 0xe8) {
    if (sVar2 < 1) {
      return 0;
    }
    if ((short)(&DAT_00799a14)[iVar7 * 0x84] < 1) {
      return 0;
    }
    if (*(short *)(&DAT_00799aa4 + iVar7 * 0x42) < 1) {
      return 0;
    }
    goto LAB_0040f674;
  }
  if (sVar3 == 0x5c) {
    iVar7 = FUN_0047fd30(iVar7,2);
    if (iVar7 == 1) {
      return 0;
    }
    if (iVar7 == 2) {
      return 0;
    }
    return 1;
  }
  if (sVar3 == 0xb4) {
    if (sVar2 < 1) {
      return 0;
    }
    if ((&DAT_00799a0d)[iVar8] == '\0') {
      return 0;
    }
    goto LAB_0040f674;
  }
  if (sVar3 == 0x5a) {
    if (sVar2 < 1) {
      return 0;
    }
    if ((&DAT_00799a0d)[iVar8] == '\0') {
      return 0;
    }
    goto LAB_0040f674;
  }
  iVar5 = FUN_00459ef0(iVar7);
  if (iVar5 == 0) {
    sVar3 = (&DAT_007999e8)[iVar7 * 0x84];
    if ((sVar3 < 0x6e) || (0x72 < sVar3)) {
      switch((int)(sVar3)) {
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
        goto switchD_0040f415_caseD_21;
      }
    }
    if (DAT_00605a0c == 0) {
      bVar9 = true;
      if (((sVar3 == 0x4a) && ((*(ushort *)(&DAT_00799a8c + iVar8) & 0xf000) == 0x1000)) &&
         ((*(ushort *)(&DAT_00799a8c + iVar8) & 0xfff) == 0x19)) {
        bVar9 = false;
      }
      if (((short)(&DAT_00799a14)[iVar7 * 0x84] < 1) && (bVar9)) {
        return 0;
      }
      if ((sVar3 == 0xcc) && (*(short *)(&DAT_00799aa4 + iVar7 * 0x42) < 1)) {
        return 0;
      }
      if ((sVar3 == 0xe2) && (*(short *)(&DAT_00799aa4 + iVar7 * 0x42) < 1)) {
        return 0;
      }
    }
  }
  else {
switchD_0040f415_caseD_21: ;
    sVar3 = (&DAT_007999e8)[iVar7 * 0x84];
    if ((((((((sVar3 != 0x37) && (sVar3 != 0x51)) && (sVar3 != 0x46)) &&
           ((sVar3 != 0xbe && (sVar3 != 0x48)))) && (sVar3 != 0xc1)) &&
         (((sVar3 != 0x4b && (sVar3 != 0xc0)) &&
          ((sVar3 != 0xa1 && (((sVar3 != 0xd9 && (sVar3 != 0xa2)) && (sVar3 != 0x6a)))))))) &&
        ((sVar3 != 0xdd && (sVar3 != 0x6b)))) &&
       ((sVar3 != 0xd8 &&
        (((sVar3 != 0x6c && (sVar3 != 0x6d)) &&
         ((sVar3 != 0x22 && (((sVar3 != 0x23 && (sVar3 != 0x24)) && (sVar3 != 0x25)))))))))) {
      if (sVar3 == 0x20) {
        if (sVar2 < 1) {
          return 0;
        }
        goto LAB_0040f674;
      }
      if (sVar3 != 0xe2) {
        if (sVar3 == 0x1f) {
          iVar7 = FUN_00475b50(iVar7);
          if ((short)(&DAT_00799a18)[iVar7 * 0x84] < 1) {
            return 0;
          }
          if (*(char *)((int)&DAT_00799a7e + iVar7 * 0x108 + 1) == '\0') {
            return 0;
          }
          goto LAB_0040f674;
        }
        if (sVar3 == 0x1e) {
          iVar7 = FUN_00475b50(iVar7);
          if ((short)(&DAT_00799a18)[iVar7 * 0x84] < 1) {
            return 0;
          }
          if (*(char *)(&DAT_00799a80 + iVar7 * 0x84) == '\0') {
            return 0;
          }
          goto LAB_0040f674;
        }
        if (sVar3 == 0x21) {
          iVar7 = FUN_00475b50(iVar7);
          if ((short)(&DAT_00799a18)[iVar7 * 0x84] < 1) {
            return 0;
          }
          if (*(char *)((int)&DAT_00799a80 + iVar7 * 0x108 + 1) == '\0') {
            return 0;
          }
          goto LAB_0040f674;
        }
        if (sVar3 == 0xd2) goto LAB_0040f674;
        if (sVar3 == 0xe4) {
          if ((&DAT_00799a30)[iVar7 * 0x42] != 9) {
            return 0;
          }
          goto LAB_0040f674;
        }
        if ((sVar3 != 0x47) && (sVar3 != 0xbf)) {
          if ((sVar2 < 1) && (*(int *)(&DAT_00705eb8 + sVar3 * 0x28) != 0)) {
            return 0;
          }
          goto LAB_0040f674;
        }
      }
    }
  }
  if (sVar2 < 1) {
    return 0;
  }
LAB_0040f674: ;
  iVar7 = param_1;
  if (DAT_0126517c <= param_1) {
    iVar7 = 0;
  }
  if ((&DAT_00605b38)[(byte)(&DAT_010d6a32)[iVar7 * 0x40]] != '\0') {
    iVar7 = param_1;
    if (DAT_0126517c <= param_1) {
      iVar7 = 0;
    }
    if ((&DAT_010d6a28)[iVar7 * 0x40] == '\0') {
      uVar6 = bVar1 + 1;
      if (DAT_0126517c <= param_1) {
        param_1 = 0;
      }
      if ((int)(short)(&DAT_010d6a16)[param_1 * 0x20] < (int)uVar6) {
        uVar6 = 1;
      }
      bVar9 = false;
    }
    else {
      bVar9 = (bVar1 & 0x80) != 0;
      if (bVar9) {
        uVar6 = (bVar1 & 0x7f) - 1;
        if ((int)uVar6 < 1) {
          uVar6 = 1;
          bVar9 = false;
        }
      }
      else {
        uVar6 = (bVar1 & 0x7f) + 1;
        iVar7 = param_1;
        if (DAT_0126517c <= param_1) {
          iVar7 = 0;
        }
        if ((int)(short)(&DAT_010d6a16)[iVar7 * 0x20] < (int)uVar6) {
          if (DAT_0126517c <= param_1) {
            param_1 = 0;
          }
          bVar9 = true;
          uVar6 = (uint)(short)(&DAT_010d6a16)[param_1 * 0x20];
        }
      }
    }
    (&DAT_00adc36c)[param_2] = (byte)uVar6;
    if (bVar9) {
      (&DAT_00adc36c)[param_2] = (byte)uVar6 | 0x80;
    }
    return uVar6;
  }
  return bVar1 & 0x7f;
}
