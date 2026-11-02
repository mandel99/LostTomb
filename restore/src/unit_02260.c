#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define DAT_00eadd88 (*(undefined *)0x00eadd88u)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
extern bool FUN_004a4300();
extern int FUN_004cde60();
extern void FUN_004cf090();
extern void FUN_004d6160();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();

void FUN_0050f890(int param_1)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  bVar1 = false;
  iVar4 = 1;
  FUN_004d65b0(DAT_00eadd90 + 0x10,DAT_00eadd8c + 0xe6,DAT_00eadd88 + -2,5,DAT_00eadd90,DAT_00eadd8c
              );
  iVar6 = param_1 * 0x184;
  iVar3 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + iVar6),*(undefined4 *)(&DAT_00bcd724 + iVar6)
                      );
  if ((((&DAT_00bcd630)[iVar6] != '\v') && ((&DAT_00bcd630)[iVar6] != '\x0f')) || (0xf < iVar3)) {
    iVar7 = 0;
    iVar5 = iVar4;
    do {
      iVar4 = iVar5;
      switch(DAT_007056c8 + -1) {
      case 0: ;
      case 1: ;
      case 2: ;
        if (iVar7 == DAT_007056c8 + -1) {
          iVar4 = iVar7;
        }
        break;
      case 3: ;
        if ((iVar7 == 3) &&
           ((((&DAT_00bcd630)[iVar6] == '\v' || ((&DAT_00bcd630)[iVar6] == '\x0f')) ||
            (iVar4 = iVar7, iVar3 == 100)))) goto LAB_0050f97a;
        break;
      case 4: ;
        if (((iVar7 != 4) || (cVar2 = FUN_004a4300(param_1), iVar4 = iVar7, cVar2 != '\0')) &&
           (iVar4 = iVar5, iVar7 == DAT_007056c8 + -1)) goto LAB_0050f97a;
        break;
default: ;
        iVar5 = 1;
LAB_0050f97a: ;
        bVar1 = true;
        iVar4 = iVar5;
      }
      iVar7 = iVar7 + 1;
      iVar5 = iVar4;
    } while (iVar7 < 5);
    if (bVar1) {
      switch((&DAT_00bcd630)[iVar6]) {
      case 10: ;
        iVar4 = 1;
        break;
      case 0xb: ;
        iVar4 = 3;
        break;
      case 0xc: ;
      case 0xd: ;
        iVar4 = 2;
        break;
default: ;
        iVar4 = 4;
        break;
      case 0x14: ;
        iVar4 = 0;
      }
    }
  }
  iVar5 = 0;
  do {
    FUN_004d63d0(0,iVar5 * 0x55 + 0x13 + DAT_00eadd90,DAT_00eadd8c + 0x8b,0x54,0x54,iVar4 == iVar5,
                 DAT_00eadd90,DAT_00eadd8c);
    cVar2 = (&DAT_00bcd630)[iVar6];
    if (((cVar2 == '\v') || (cVar2 == '\x0f')) && (iVar3 < 0x10)) {
      iVar7 = iVar5 * 0x55 + 0x13 + DAT_00eadd90;
LAB_0050faa1: ;
      FUN_004cf090(iVar7,DAT_00eadd8c + 0x8b,0x54,0x54,0);
    }
    else if (iVar5 == 3) {
      if (((cVar2 == '\v') || (cVar2 == '\x0f')) || (iVar3 == 100)) {
        iVar7 = DAT_00eadd90 + 0x112;
        goto LAB_0050faa1;
      }
    }
    else if ((iVar5 == 4) && (cVar2 = FUN_004a4300(param_1), cVar2 != '\0')) {
      iVar7 = DAT_00eadd90 + 0x167;
      goto LAB_0050faa1;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 5);
  if (DAT_007056c8 == 0) {
    switch((&DAT_00bcd630)[iVar6]) {
    case 10: ;
      goto switchD_0050fb16_caseD_a;
    case 0xb: ;
      cVar2 = '\x0f';
      break;
    case 0xc: ;
      cVar2 = '\x17';
      break;
    case 0xd: ;
      cVar2 = '\x19';
      break;
default: ;
      cVar2 = '\x11';
      break;
    case 0x14: ;
      cVar2 = '\x13';
    }
  }
  else {
    if (DAT_007056c8 == 1) {
      cVar2 = '\x13';
      goto LAB_0050fb3e;
    }
    if (DAT_007056c8 != 2) {
      if (DAT_007056c8 == 3) {
        cVar2 = (-((&DAT_00bcd728)[iVar6] != '\0') & 2U) + 0x17;
      }
      else {
        cVar2 = (-(DAT_007056c8 != 4) & 2U) + 0xf;
      }
      goto LAB_0050fb3e;
    }
switchD_0050fb16_caseD_a: ;
    cVar2 = '\x15';
  }
LAB_0050fb3e: ;
  FUN_004d62f0(0xb8,cVar2,DAT_00eadd90 + 0x18,DAT_00eadd8c + 0xec,DAT_00f56f9c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6160(0xb8,cVar2 + '\x01',DAT_00eadd90 + 0x18,DAT_00eadd8c + 0xfc,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
