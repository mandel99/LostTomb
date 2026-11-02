#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_006fdc88 (*(undefined *)0x006fdc88u)
#define DAT_0077af00 (*(undefined4 *)0x0077af00u)
#define DAT_0077af04 (*(undefined4 *)0x0077af04u)
#define DAT_0077af08 (*(undefined4 *)0x0077af08u)
#define DAT_0077af2c (*(undefined4 *)0x0077af2cu)
#define DAT_0077de03 (*(undefined4 *)0x0077de03u)
#define DAT_0077de06 (*(undefined4 *)0x0077de06u)
#define DAT_0077de2a (*(undefined4 *)0x0077de2au)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcba32 (*(undefined4 *)0x00bcba32u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd666 (*(undefined4 *)0x00bcd666u)
#define DAT_00bcd668 (*(undefined4 *)0x00bcd668u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00bcd675 (*(undefined4 *)0x00bcd675u)
#define DAT_00bcd677 (*(undefined4 *)0x00bcd677u)
#define DAT_00bcd690 (*(undefined4 *)0x00bcd690u)
#define DAT_00bcd729 (*(undefined4 *)0x00bcd729u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e39088 (*(char *)0x00e39088u)
#define DAT_00eadd84 (*(undefined *)0x00eadd84u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56fbc (*(undefined *)0x00f56fbcu)
#define DAT_00f571fc (*(undefined *)0x00f571fcu)
#define PTR_DAT_005f78f0 (*(unsigned char * *)0x005f78f0u)
#define _DAT_00eadd54 (*(undefined4 *)0x00eadd54u)
extern void FUN_00425730();
extern bool FUN_00447790();
extern undefined4 FUN_004c5120();
extern void FUN_004d6160();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();
extern int FUN_004fa8c0();
extern void FUN_004fbf00();
extern void FUN_004fbf30();
extern void FUN_004fc040();
extern int FUN_0053d280();
extern int FUN_0053e590();

void FUN_004fab20(int param_1)

{
  char cVar1;
  undefined1 uVar2;
  byte bVar3;
  short sVar4;
  bool bVar5;
  undefined4 uVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  int iVar11;
  byte bVar12;
  undefined4 uVar13;
  int iVar14;
  undefined4 uVar15;
  
  iVar11 = param_1 * 0x184;
  bVar12 = (&DAT_00bcd602)[iVar11];
  iVar8 = FUN_0053d280(param_1);
  uVar6 = DAT_00f56f94;
  if (bVar12 == 0x5b) {
    bVar12 = (&DAT_00bcd603)[iVar11];
  }
  _DAT_00eadd54 = 0x2a;
  iVar9 = FUN_004fa8c0(param_1);
  FUN_004d63d0(0,DAT_00eadd90 + 0x18,DAT_00eadd8c + 0x66,(DAT_00eadd88 + -3) * 0x10,0x7a,0,
               DAT_00eadd90,DAT_00eadd8c);
  if (((bVar12 == 0x13) || (bVar12 == 0x15)) || (bVar12 == 0x14)) {
    if ((&DAT_00bcd602)[iVar11] == '\x15') {
      param_1 = (int)*(short *)(&DAT_00bcd660 + iVar11);
    }
    if ((&DAT_00bcd602)[param_1 * 0x184] == '\x15') {
      param_1 = (int)*(short *)(&DAT_00bcd660 + param_1 * 0x184);
    }
    iVar11 = param_1 * 0x184;
    uVar10 = (uint)(byte)(&DAT_00bcd666)[iVar11];
    DAT_00e20778 = 0;
    FUN_004d62f0(0x40,bVar12,DAT_00eadd90 + 0x28,DAT_00eadd8c + 0x6e,uVar6,0,DAT_00eadd90,
                 DAT_00eadd8c);
    if (DAT_00e39088 == '\0') {
      uVar2 = (&DAT_0077de03)[uVar10 * 0x6a];
      uVar13 = 0xc3;
    }
    else {
      uVar2 = (&DAT_0077de03)[uVar10 * 0x6a];
      uVar13 = 0x15;
    }
    FUN_004d62f0(uVar13,uVar2,DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 0x6e,uVar6,0,
                 DAT_00eadd90,DAT_00eadd8c);
    bVar3 = (&DAT_00bcd677)[iVar11];
    cVar7 = (&DAT_00bcd630)[iVar11];
    if (bVar12 == 0x14) {
      if (cVar7 == '\f') {
        uVar13 = 6;
      }
      else if (cVar7 == '\n') {
        uVar13 = 7;
      }
      else {
        if (cVar7 == '\r') goto LAB_004fbaf9;
        uVar13 = 9;
      }
    }
    else if (cVar7 == '\n') {
      uVar13 = 10;
    }
    else if (cVar7 == '\v') {
      iVar8 = (uint)bVar3 * 0x58;
      if ((*(int *)(&DAT_0077af04 + iVar8) == 0) && (*(int *)(&DAT_0077af00 + iVar8) == 0)) {
        uVar13 = 0xd;
      }
      else {
LAB_004fbaf9: ;
        uVar13 = 8;
      }
    }
    else {
      if (cVar7 != '\t') goto LAB_004fbaf9;
      uVar13 = 0xc;
    }
    FUN_004d62f0(0x81,uVar13,DAT_00eadd90 + 0x28,DAT_00eadd8c + 0x96,uVar6,0,DAT_00eadd90,
                 DAT_00eadd8c);
    iVar8 = (uint)bVar3 * 0x58;
    if ((*(int *)(&DAT_0077af04 + iVar8) == 0) && (*(int *)(&DAT_0077af00 + iVar8) == 0)) {
      DAT_00e20778 = 0;
      FUN_004d62f0(0x81,2,DAT_00eadd90 + 0x28,DAT_00eadd8c + 0xaa,uVar6,0,DAT_00eadd90,DAT_00eadd8c)
      ;
      iVar8 = 1;
      do {
        if (((&DAT_0077de06)[uVar10 * 0x6a + iVar8] != '\0') &&
           (cVar7 = FUN_00447790(uVar10,iVar8), cVar7 != '\0')) {
          FUN_0053e590(DAT_00f571fc + iVar8,DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 0xa7);
          DAT_00e20778 = DAT_00e20778 + 0x19;
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 < 0x24);
      DAT_00e20778 = 0;
      FUN_004d62f0(0x81,3,DAT_00eadd90 + 0x28,DAT_00eadd8c + 200,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
      iVar8 = 1;
      do {
        if (((&DAT_0077de2a)[uVar10 * 0x6a + iVar8] != '\0') &&
           (cVar7 = FUN_00447790(uVar10,iVar8), cVar7 != '\0')) {
          FUN_0053e590(DAT_00f571fc + iVar8,DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 0xc5);
          DAT_00e20778 = DAT_00e20778 + 0x19;
        }
        iVar8 = iVar8 + 1;
      } while (iVar8 < 0x24);
    }
    else {
      DAT_00e20778 = 0;
      FUN_004d62f0(0x81,4,DAT_00eadd90 + 0x28,DAT_00eadd8c + 0xaa,uVar6,0,DAT_00eadd90,DAT_00eadd8c)
      ;
      iVar9 = 1;
      do {
        uVar10 = (uint)(byte)(&DAT_0077af08)[iVar9 + iVar8];
        if (uVar10 != 0) {
          switch((int)(iVar9)) {
          case 0x18: ;
          case 0x19: ;
          case 0x1a: ;
          case 0x1e: ;
          case 0x23: ;
            break;
default: ;
            if (((iVar9 != 0x1c) && (iVar9 != 10)) && (iVar9 != 0x24)) {
              uVar10 = uVar10 * 100;
            }
          }
          FUN_004d6860(uVar10,0x40,&DAT_0057c048,DAT_00e20778 + 0x28 + DAT_00eadd90,
                       DAT_00eadd8c + 0xaa,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
          FUN_0053e590(DAT_00f571fc + iVar9,DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 0xa7);
          DAT_00e20778 = DAT_00e20778 + 0x19;
        }
        iVar9 = iVar9 + 1;
      } while (iVar9 < 0x24);
      DAT_00e20778 = 0;
      FUN_004d62f0(0x81,5,DAT_00eadd90 + 0x28,DAT_00eadd8c + 200,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
      iVar9 = 1;
      do {
        uVar10 = (uint)(byte)(&DAT_0077af2c)[iVar9 + iVar8];
        if (uVar10 != 0) {
          switch((int)(iVar9)) {
          case 0x18: ;
          case 0x19: ;
          case 0x1a: ;
          case 0x1e: ;
          case 0x23: ;
            break;
default: ;
            if (((iVar9 != 0x1c) && (iVar9 != 10)) && (iVar9 != 0x24)) {
              uVar10 = uVar10 * 100;
            }
          }
          FUN_004d6860(uVar10,0x40,&DAT_0057c048,DAT_00e20778 + 0x28 + DAT_00eadd90,
                       DAT_00eadd8c + 200,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
          FUN_0053e590(DAT_00f571fc + iVar9,DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 0xc5);
          DAT_00e20778 = DAT_00e20778 + 0x19;
        }
        iVar9 = iVar9 + 1;
      } while (iVar9 < 0x24);
    }
    goto LAB_004fbe2b;
  }
  if (((0x2a < bVar12) && (bVar12 < 0x36)) || (bVar12 == 99)) {
    DAT_00e20778 = 0;
    sVar4 = (&DAT_00bcd658)[param_1 * 0xc2];
    if (*(int *)(&DAT_00bcd690 + iVar11) != -1) {
      FUN_004d62f0(*(int *)(&DAT_00bcd690 + iVar11),(int)*(short *)(&DAT_00bcd668 + iVar11),
                   DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x6c,DAT_00f56fbc,0,DAT_00eadd90,DAT_00eadd8c)
      ;
    }
    DAT_00e20778 = 0;
    FUN_004d62f0(0x25,(byte)(&DAT_00bcba32)[sVar4 * 0x90] + 0xe,DAT_00eadd90 + 0x1c,
                 DAT_00eadd8c + 0x95,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
    goto LAB_004fbe2b;
  }
  cVar7 = (&DAT_00bcd602)[iVar11];
  if (((cVar7 == '\\') || (cVar7 == ']')) || ((cVar7 == 'e' || (cVar7 == 'd')))) {
    DAT_00e20778 = 0;
    FUN_004d62f0(0x40,cVar7,DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x6c,DAT_00f56fbc,0,DAT_00eadd90,
                 DAT_00eadd8c);
    goto LAB_004fbe2b;
  }
  if (((cVar7 == '\x19') || (cVar7 == 'C')) ||
     ((cVar7 == 'D' || ((cVar7 == 'E' || (cVar7 == 'F')))))) {
    if (-1 < iVar9) {
      FUN_0053e590(iVar9,DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x70);
    }
    DAT_00e20778 = 0;
    FUN_004d62f0(0x40,bVar12,DAT_00eadd90 + 0x5c,DAT_00eadd8c + 0x8b,uVar6,0,DAT_00eadd90,
                 DAT_00eadd8c);
    goto LAB_004fbe2b;
  }
  if (((bVar12 != 4) && (bVar12 != 9)) && (bVar12 != 0x26)) {
    if (bVar12 == 0x27) {
      if (-1 < iVar9) {
        FUN_0053e590(iVar9,DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x70);
      }
      DAT_00e20778 = 0;
      if (*(int *)(&DAT_00bcd690 + iVar11) != -1) {
        FUN_004d62f0(*(int *)(&DAT_00bcd690 + iVar11),(int)*(short *)(&DAT_00bcd668 + iVar11),
                     DAT_00eadd90 + 0x5a,DAT_00eadd8c + 0x6c,DAT_00f56fbc,0,DAT_00eadd90,
                     DAT_00eadd8c);
      }
      DAT_00e20778 = 0;
      FUN_004d62f0(0x40,0x27,DAT_00eadd90 + 0x5c,DAT_00eadd8c + 0x8b,uVar6,0,DAT_00eadd90,
                   DAT_00eadd8c);
      if ((&DAT_00bcd630)[iVar11] == '\b') {
        FUN_004d62f0(0x81,0x11,DAT_00e20778 + 0x5a + DAT_00eadd90,DAT_00eadd8c + 0x8b,uVar6,0,
                     DAT_00eadd90,DAT_00eadd8c);
        FUN_004fbf30(param_1,DAT_00e20778 + 0x5a + DAT_00eadd90,DAT_00eadd8c + 0x87);
      }
      else if ((&DAT_00bcd630)[iVar11] == '\t') {
        FUN_004d62f0(0x81,0x12,DAT_00e20778 + 0x5a + DAT_00eadd90,DAT_00eadd8c + 0x8b,uVar6,0,
                     DAT_00eadd90,DAT_00eadd8c);
        FUN_004fc040(param_1,DAT_00e20778 + 0x5a + DAT_00eadd90,DAT_00eadd8c + 0x87);
      }
      if ((iVar8 != -1) && (-1 < (char)(&DAT_00bcd675)[iVar11])) {
        FUN_004d6160(iVar8,(int)(char)(&DAT_00bcd675)[iVar11],DAT_00eadd90 + 0x5a,
                     DAT_00eadd8c + 0xa0,(DAT_00eadd88 + -9) * 0x10,uVar6,0,DAT_00eadd90,
                     DAT_00eadd8c);
      }
    }
    else if (bVar12 == 0x42) {
      if (-1 < iVar9) {
        FUN_0053e590(iVar9,DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x70);
      }
      switch((&DAT_00bcd672)[iVar11]) {
      case 0: ;
        uVar13 = 1;
        break;
      case 1: ;
        uVar13 = 2;
        break;
      case 2: ;
        uVar13 = 3;
        break;
      case 3: ;
        uVar13 = 4;
        break;
      case 4: ;
        uVar13 = 5;
        break;
      case 5: ;
        uVar13 = 6;
        break;
      case 6: ;
        uVar13 = 7;
        break;
      case 7: ;
        uVar13 = 8;
        break;
      case 8: ;
        uVar13 = 0xf;
        break;
      case 9: ;
        uVar13 = 0xd;
        break;
      case 10: ;
        uVar13 = 0x11;
        break;
      case 0xb: ;
        uVar13 = 0x13;
        break;
default: ;
        uVar13 = 0xffffffff;
      }
      DAT_00e20778 = 0;
      if (*(int *)(&DAT_00bcd690 + iVar11) != -1) {
        FUN_004d62f0(*(int *)(&DAT_00bcd690 + iVar11),(int)*(short *)(&DAT_00bcd668 + iVar11),
                     DAT_00eadd90 + 0x5a,DAT_00eadd8c + 0x6c,DAT_00f56fbc,0,DAT_00eadd90,
                     DAT_00eadd8c);
      }
      DAT_00e20778 = 0;
      FUN_004d62f0(0x40,0x42,DAT_00eadd90 + 0x5c,DAT_00eadd8c + 0x8b,uVar6,0,DAT_00eadd90,
                   DAT_00eadd8c);
      FUN_004d62f0(0x81,0x15,DAT_00e20778 + 0x5a + DAT_00eadd90,DAT_00eadd8c + 0x8b,uVar6,0,
                   DAT_00eadd90,DAT_00eadd8c);
      FUN_004fbf00(uVar13,DAT_00e20778 + 0x5a + DAT_00eadd90,DAT_00eadd8c + 0x87);
      if ((iVar8 != -1) && (-1 < (char)(&DAT_00bcd675)[iVar11])) {
        FUN_004d6160(iVar8,(int)(char)(&DAT_00bcd675)[iVar11],DAT_00eadd90 + 0x5a,
                     DAT_00eadd8c + 0xa0,(DAT_00eadd88 + -9) * 0x10,uVar6,0,DAT_00eadd90,
                     DAT_00eadd8c);
      }
    }
    else {
      if (bVar12 == 0x49) {
        DAT_00e20778 = 0;
        if (*(int *)(&DAT_00bcd690 + iVar11) != -1) {
          FUN_004d62f0(*(int *)(&DAT_00bcd690 + iVar11),(int)*(short *)(&DAT_00bcd668 + iVar11),
                       DAT_00eadd90 + 0x5a,DAT_00eadd8c + 0x6c,DAT_00f56fbc,0,DAT_00eadd90,
                       DAT_00eadd8c);
        }
        DAT_00e20778 = 0;
        FUN_004d62f0(0x40,0x49,DAT_00eadd90 + 0x5c,DAT_00eadd8c + 0x8b,uVar6,0,DAT_00eadd90,
                     DAT_00eadd8c);
        if (-1 < iVar9) {
          FUN_0053e590(iVar9,DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x70);
        }
        if ((&DAT_00bcd630)[iVar11] == '\f') {
          FUN_004d62f0(0x81,0x12,DAT_00e20778 + 0x5e + DAT_00eadd90,DAT_00eadd8c + 0x8b,uVar6,0,
                       DAT_00eadd90,DAT_00eadd8c);
          FUN_0053e590(DAT_00f571fc + 8,DAT_00e20778 + 0x5e + DAT_00eadd90,DAT_00eadd8c + 0x87);
        }
      }
      else {
        if (cVar7 == 'V') {
          iVar9 = -1;
        }
        if (cVar7 == 'U') {
          bVar12 = (-(*(short *)(&DAT_00bcd660 + iVar11) != 0) & 0xbU) + 0x55;
        }
        if (-1 < iVar9) {
          FUN_0053e590(iVar9,DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x70);
        }
        iVar9 = *(int *)(&DAT_00bcd690 + iVar11);
        DAT_00e20778 = 0;
        if ((iVar9 != -1) && (-1 < iVar9)) {
          FUN_004d62f0(iVar9,(int)*(short *)(&DAT_00bcd668 + iVar11),DAT_00eadd90 + 0x5a,
                       DAT_00eadd8c + 0x6c,DAT_00f56fbc,0,DAT_00eadd90,DAT_00eadd8c);
        }
        DAT_00e20778 = 0;
        if ((bVar12 == 0x1b) &&
           (iVar9 = FUN_004c5120((int)(short)(&DAT_00bcd652)[param_1 * 0xc2]), iVar9 == 4)) {
          FUN_004d62f0(0x3b,0x26,DAT_00e20778 + 0x5c + DAT_00eadd90,DAT_00eadd8c + 0x8b,uVar6,0,
                       DAT_00eadd90,DAT_00eadd8c);
        }
        else {
          FUN_004d62f0(0x40,bVar12,DAT_00e20778 + 0x5c + DAT_00eadd90,DAT_00eadd8c + 0x8b,uVar6,0,
                       DAT_00eadd90,DAT_00eadd8c);
        }
      }
      if ((iVar8 != -1) && (-1 < (char)(&DAT_00bcd675)[iVar11])) {
        FUN_004d6160(iVar8,(int)(char)(&DAT_00bcd675)[iVar11],DAT_00eadd90 + 0x5a,
                     DAT_00eadd8c + 0xa0,(DAT_00eadd88 + -9) * 0x10,uVar6,0,DAT_00eadd90,
                     DAT_00eadd8c);
      }
    }
    goto LAB_004fbe2b;
  }
  if (-1 < iVar9) {
    FUN_0053e590(iVar9,DAT_00eadd90 + 0x1c,DAT_00eadd8c + 0x70);
  }
  DAT_00e20778 = 0;
  if (*(int *)(&DAT_00bcd690 + iVar11) != -1) {
    FUN_004d62f0(*(int *)(&DAT_00bcd690 + iVar11),(int)*(short *)(&DAT_00bcd668 + iVar11),
                 DAT_00eadd90 + 0x5a,DAT_00eadd8c + 0x6c,DAT_00f56fbc,0,DAT_00eadd90,DAT_00eadd8c);
  }
  DAT_00e20778 = 0;
  FUN_004d62f0(0x29,(int)(short)(&DAT_007999e8)[(short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x84],
               DAT_00eadd90 + 0x5c,DAT_00eadd8c + 0x8b,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
  FUN_004d62f0(0x40,4,DAT_00e20778 + 0x5c + DAT_00eadd90,DAT_00eadd8c + 0x8b,uVar6,0,DAT_00eadd90,
               DAT_00eadd8c);
  DAT_00e20778 = 0;
  if (((((&DAT_00bcd602)[iVar11] != '&') || ((&DAT_00bcd630)[iVar11] != '\b')) &&
      (*(short *)(&DAT_00bcd66c + iVar11) != 0)) && ((&DAT_00bcd603)[iVar11] != 0)) {
    FUN_0053e590((uint)(byte)(&DAT_00bcd603)[iVar11] + DAT_00f571fc,DAT_00eadd90 + 0x5c,
                 DAT_00eadd8c + 0x96);
  }
  iVar9 = (int)*(short *)(&DAT_00bcd66c + iVar11);
  cVar7 = (&DAT_00bcd603)[iVar11];
  switch((int)(cVar7)) {
  case '\x18': ;
  case '\x19': ;
  case '\x1a': ;
  case '\x1e': ;
  case '#': ;
switchD_004fb445_caseD_18: ;
    iVar9 = iVar9 / 100;
    break;
default: ;
    if (((cVar7 == '\x1c') || (cVar7 == '\n')) || (cVar7 == '$')) goto switchD_004fb445_caseD_18;
  }
  if (iVar9 != 0) {
    FUN_004d6860(iVar9,0x40,&DAT_0057c048,DAT_00e20778 + 0x6e + DAT_00eadd90,DAT_00eadd8c + 0x98,
                 uVar6,0,DAT_00eadd90,DAT_00eadd8c);
  }
  cVar7 = (&DAT_00bcd603)[iVar11];
  switch((int)(cVar7)) {
  case '\x18': ;
  case '\x19': ;
  case '\x1a': ;
  case '\x1e': ;
  case '#': ;
    if (iVar9 == 1) {
      FUN_004d62f0(99,0x17,DAT_00e20778 + 0x6e + DAT_00eadd90,DAT_00eadd8c + 0x98,uVar6,0,
                   DAT_00eadd90,DAT_00eadd8c);
      goto LAB_004fb56d;
    }
    if (iVar9 != 0) {
      uVar15 = 0x18;
      uVar13 = 99;
LAB_004fb55b: ;
      FUN_004d62f0(uVar13,uVar15,DAT_00e20778 + 0x6e + DAT_00eadd90,DAT_00eadd8c + 0x98,uVar6,0,
                   DAT_00eadd90,DAT_00eadd8c);
      goto LAB_004fb569;
    }
    break;
default: ;
    if ((cVar7 != '\x1c') && (cVar7 != '\n')) {
      if (iVar9 == 0) break;
      if (cVar7 != '\0') {
        uVar15 = 0x14;
        uVar13 = 0x81;
        goto LAB_004fb55b;
      }
    }
LAB_004fb569: ;
    if (iVar9 != 0) {
LAB_004fb56d: ;
      bVar12 = (&DAT_00bcd603)[iVar11];
      if ((bVar12 == 0x1c) || (bVar12 == 10)) {
        if (iVar9 == 1) {
          uVar10 = (uint)bVar12;
          iVar9 = DAT_00e20778 + 0x6e + DAT_00eadd90;
        }
        else {
          iVar9 = DAT_00e20778 + 0x6e + DAT_00eadd90;
          uVar10 = bVar12 + 0x36;
        }
      }
      else {
        iVar9 = DAT_00e20778 + 0x6e + DAT_00eadd90;
        uVar10 = (uint)bVar12;
      }
      FUN_004d62f0(0x17,uVar10,iVar9,DAT_00eadd8c + 0x98,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
    }
  }
  if ((iVar8 != -1) && (-1 < (char)(&DAT_00bcd675)[iVar11])) {
    FUN_004d6160(iVar8,(int)(char)(&DAT_00bcd675)[iVar11],DAT_00eadd90 + 0x5a,DAT_00eadd8c + 0xaa,
                 (DAT_00eadd88 + -9) * 0x10,uVar6,0,DAT_00eadd90,DAT_00eadd8c);
  }
  if ((&DAT_00bcd652)[param_1 * 0xc2] != 0) {
    iVar8 = (int)(short)(&DAT_00bcd652)[param_1 * 0xc2];
    iVar9 = (int)(short)(&DAT_00bcd656)[param_1 * 0xc2];
    if (iVar9 != 0) {
      DAT_00e20778 = 0;
      cVar7 = (&DAT_00bcd602)[iVar11];
      if ((cVar7 == '\x04') && ((&DAT_00bcd630)[iVar11] == '\x0f')) {
        bVar5 = true;
      }
      else if ((cVar7 == '\t') &&
              (((cVar1 = (&DAT_00bcd630)[iVar11], cVar1 == '\v' || (cVar1 == '\x0e')) ||
               (cVar1 == '\x11')))) {
        bVar5 = true;
      }
      else if ((cVar7 == '&') &&
              (((cVar1 = (&DAT_00bcd630)[iVar11], cVar1 == '\n' || (cVar1 == '\r')) ||
               (cVar1 == '\x0e')))) {
        bVar5 = true;
      }
      else {
        bVar5 = false;
      }
      if ((cVar7 != '&') || ((&DAT_00bcd630)[iVar11] != '\b')) {
        if (bVar5) {
          FUN_004d62f0(0x81,0x10,DAT_00eadd90 + 0x28,DAT_00eadd8c + 200,uVar6,0,DAT_00eadd90,
                       DAT_00eadd8c);
          FUN_004d62f0(0x29,(int)(short)(&DAT_007999e8)[iVar8 * 0x84],
                       DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 200,uVar6,0,DAT_00eadd90,
                       DAT_00eadd8c);
          FUN_004d62f0(0x81,0xe,DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 200,uVar6,0,
                       DAT_00eadd90,DAT_00eadd8c);
          iVar14 = DAT_00e20778 + 0x28 + DAT_00eadd90;
          iVar8 = iVar9;
        }
        else {
          FUN_004d62f0(0x81,0xf,DAT_00eadd90 + 0x28,DAT_00eadd8c + 200,uVar6,0,DAT_00eadd90,
                       DAT_00eadd8c);
          FUN_004d62f0(0x29,(int)(short)(&DAT_007999e8)[iVar9 * 0x84],
                       DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 200,uVar6,0,DAT_00eadd90,
                       DAT_00eadd8c);
          FUN_004d62f0(0x81,0xe,DAT_00e20778 + 0x28 + DAT_00eadd90,DAT_00eadd8c + 200,uVar6,0,
                       DAT_00eadd90,DAT_00eadd8c);
          iVar14 = DAT_00e20778 + 0x28 + DAT_00eadd90;
        }
        FUN_004d62f0(0x29,(int)(short)(&DAT_007999e8)[iVar8 * 0x84],iVar14,DAT_00eadd8c + 200,uVar6,
                     0,DAT_00eadd90,DAT_00eadd8c);
      }
    }
  }
LAB_004fbe2b: ;
  param_1 = iVar11;
  bVar12 = (&DAT_00bcd729)[param_1];
  if ((bVar12 != 0) && ((&DAT_006fdc88)[bVar12] != '\0')) {
    FUN_00425730(DAT_00eadd90 + 0x28,DAT_00eadd84 * 0x10 + -0x28 + DAT_00eadd8c,
                 (&PTR_DAT_005f78f0)[bVar12],1);
  }
  return;
}
