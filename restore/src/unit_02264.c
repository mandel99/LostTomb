#include "restore_types.h"
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
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

void FUN_0050fe50(int param_1)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  FUN_004d65b0(DAT_00eadd90 + 0x10,DAT_00eadd8c + 0xe6,DAT_00eadd88 + -2,5,DAT_00eadd90,DAT_00eadd8c
              );
  bVar1 = false;
  iVar6 = param_1 * 0x184;
  iVar3 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + iVar6),*(undefined4 *)(&DAT_00bcd724 + iVar6)
                      );
  iVar4 = 1;
  if ((((&DAT_00bcd630)[iVar6] != '\r') && ((&DAT_00bcd630)[iVar6] != '\x0f')) || (0xf < iVar3)) {
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
           ((((&DAT_00bcd630)[iVar6] == '\r' || ((&DAT_00bcd630)[iVar6] == '\x0f')) ||
            (iVar4 = iVar7, iVar3 == 100)))) goto LAB_0050ff3a;
        break;
      case 4: ;
        if (((iVar7 != 4) || (cVar2 = FUN_004a4300(param_1), iVar4 = iVar7, cVar2 != '\0')) &&
           (iVar4 = iVar5, iVar7 == DAT_007056c8 + -1)) goto LAB_0050ff3a;
        break;
default: ;
        iVar5 = 1;
LAB_0050ff3a: ;
        bVar1 = true;
        iVar4 = iVar5;
      }
      iVar7 = iVar7 + 1;
      iVar5 = iVar4;
    } while (iVar7 < 5);
    if (bVar1) {
      switch((&DAT_00bcd630)[iVar6]) {
      case 10: ;
        iVar4 = 2;
        break;
default: ;
        iVar4 = 4;
        break;
      case 0xd: ;
        iVar4 = 3;
        break;
      case 0x14: ;
        iVar4 = 0;
        break;
      case 0x17: ;
        iVar4 = 1;
      }
    }
  }
  iVar5 = 0;
  do {
    FUN_004d63d0(0,iVar5 * 0x55 + 0x13 + DAT_00eadd90,DAT_00eadd8c + 0x8b,0x54,0x54,iVar4 == iVar5,
                 DAT_00eadd90,DAT_00eadd8c);
    cVar2 = (&DAT_00bcd630)[iVar6];
    if (((cVar2 == '\r') || (cVar2 == '\x0f')) && (iVar3 < 0x10)) {
      iVar7 = iVar5 * 0x55 + 0x13 + DAT_00eadd90;
LAB_00510061: ;
      FUN_004cf090(iVar7,DAT_00eadd8c + 0x8b,0x54,0x54,0);
    }
    else if (iVar5 == 3) {
      if (((cVar2 == '\r') || (cVar2 == '\x0f')) || (iVar3 == 100)) {
        iVar7 = DAT_00eadd90 + 0x112;
        goto LAB_00510061;
      }
    }
    else if ((iVar5 == 4) && (cVar2 = FUN_004a4300(param_1), cVar2 != '\0')) {
      iVar7 = DAT_00eadd90 + 0x167;
      goto LAB_00510061;
    }
    iVar5 = iVar5 + 1;
  } while (iVar5 < 5);
  if (DAT_007056c8 == 0) {
    switch((&DAT_00bcd630)[iVar6]) {
    case 10: ;
      goto switchD_005100c9_caseD_a;
default: ;
      cVar2 = '\x11';
      break;
    case 0xd: ;
      cVar2 = '\x0f';
      break;
    case 0x14: ;
      cVar2 = '\t';
      break;
    case 0x17: ;
switchD_005100c9_caseD_17: ;
      cVar2 = '\v';
    }
  }
  else {
    if (DAT_007056c8 == 1) {
      cVar2 = '\t';
      goto LAB_005100e3;
    }
    if (DAT_007056c8 == 2) goto switchD_005100c9_caseD_17;
    if (DAT_007056c8 != 3) {
      cVar2 = (-(DAT_007056c8 != 4) & 2U) + 0xf;
      goto LAB_005100e3;
    }
switchD_005100c9_caseD_a: ;
    cVar2 = '\r';
  }
LAB_005100e3: ;
  FUN_004d62f0(0xb8,cVar2,DAT_00eadd90 + 0x18,DAT_00eadd8c + 0xec,DAT_00f56f9c,0,DAT_00eadd90,
               DAT_00eadd8c);
  FUN_004d6160(0xb8,cVar2 + '\x01',DAT_00eadd90 + 0x18,DAT_00eadd8c + 0xfc,
               (DAT_00eadd88 + -4) * 0x10,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
