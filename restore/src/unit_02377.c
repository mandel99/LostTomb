#include "restore_types.h"
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
#define DAT_00bcd5fa (*(undefined4 *)0x00bcd5fau)
#define DAT_00bcd72a (*(undefined4 *)0x00bcd72au)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eb4 (*(undefined4 *)0x00ea5eb4u)
#define DAT_00ea5ed4 (*(undefined4 *)0x00ea5ed4u)
#define DAT_00ea5ed8 (*(undefined4 *)0x00ea5ed8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dd0 (*(undefined *)0x00f53dd0u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5643c (*(undefined *)0x00f5643cu)
#define DAT_00f56694 (*(undefined *)0x00f56694u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56834 (*(undefined *)0x00f56834u)
#define DAT_00f5690c (*(undefined *)0x00f5690cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_00446610();
extern int * FUN_0044baf0();
extern int FUN_004bc620();
extern void FUN_004ccc30();
extern void FUN_004cd680();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern char FUN_0050fc10();
extern char FUN_0050fc70();
extern void FUN_00521a60();
extern undefined1 FUN_00522d80();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00522e10(void)

{
  short sVar1;
  bool bVar2;
  bool bVar3;
  bool bVar4;
  char cVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  bool bVar12;
  undefined4 uVar13;
  char local_3;
  char local_2;
  char local_1;
  
  DAT_00f53f0c = DAT_00e38da8;
  iVar6 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1a;
  if (4 < iVar6) {
    DAT_00f53f00 = 0x1a;
    _DAT_00f53f04 = 0x28;
    DAT_00f53f08 = DAT_00e38da4;
    DAT_00f53f0c = DAT_00e38da8;
    return;
  }
  if (0 < iVar6) {
    FUN_004cd680(iVar6 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar6 * 8,(iVar6 * -2 + 0x28) * 0x10,
                 (iVar6 * -2 + 0x1a) * 0x10,0);
    return;
  }
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1a);
  FUN_0053e590(DAT_00f5681c + 1,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
  FUN_004d62f0(0x33,0x13,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,1,DAT_00f53f0c + 0x1a7,DAT_00f53f08 + 0x2b,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,0x14,DAT_00f53f0c + 0x1a7,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,3,DAT_00f53f0c + 0x1e0,DAT_00f53f08 + 0x2b,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,0x15,DAT_00f53f0c + 0x1e0,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,0x16,DAT_00f53f0c + 0x13b,DAT_00f53f08 + 0x2b,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,0x17,DAT_00f53f0c + 300,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,0x18,DAT_00f53f0c + 0xaa,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  DAT_00f53dd0 = (uint)(byte)(&DAT_00ea5ed8)[DAT_00eaac28 * 0x49d8];
  FUN_00521a60(&local_1,&local_3);
  local_2 = '\0';
  bVar2 = false;
  bVar3 = false;
  bVar4 = false;
  iVar6 = FUN_0044baf0(0);
  cVar5 = '\0';
  while (iVar6 != 0) {
    if (*(short *)(iVar6 + 4) == 0x25) {
      iVar8 = (int)*(short *)(iVar6 + 0x1c);
      bVar12 = false;
      if ((-1 < iVar8) && (iVar8 < 0x32)) {
        bVar12 = (&DAT_007770f0)[iVar8 * 0x144] == '\x02';
      }
      sVar1 = *(short *)(iVar6 + 0x30);
      if (sVar1 == 5) {
        if (bVar12) {
          bVar4 = true;
        }
        else {
          bVar3 = true;
        }
      }
      if ((sVar1 == 6) || (sVar1 == 7)) {
        if (bVar12) {
          bVar2 = true;
        }
        else {
          local_2 = '\x01';
        }
      }
    }
    iVar6 = FUN_0044baf0(iVar6);
    cVar5 = local_2;
  }
  if ((int)DAT_00f53dd0 < 1) {
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x167);
    if ((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] == 0) {
      if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) {
        iVar6 = FUN_00446610();
        if (iVar6 == 0) {
          iVar6 = DAT_00f53f08 + 0x166;
          iVar8 = DAT_00f53f0c + 0x46;
          uVar13 = 8;
        }
        else {
          iVar6 = DAT_00f53f08 + 0x166;
          iVar8 = DAT_00f53f0c + 0x46;
          uVar13 = 9;
        }
      }
      else {
        iVar6 = DAT_00f53f08 + 0x166;
        iVar8 = DAT_00f53f0c + 0x46;
        uVar13 = 10;
      }
    }
    else {
      iVar6 = DAT_00f53f08 + 0x166;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0xb;
    }
    FUN_004d62f0(0x33,uVar13,iVar8,iVar6,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x185);
    if (cVar5 != '\0') {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0xf;
      goto LAB_005237dc;
    }
    if (bVar2) {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1c;
      goto LAB_005237dc;
    }
    if (bVar3) {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0xe;
      goto LAB_005237dc;
    }
    if (bVar4) {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1b;
      goto LAB_005237dc;
    }
    if (local_1 != '\0') {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0xd;
      goto LAB_005237dc;
    }
    if (local_3 != '\0') {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1a;
      goto LAB_005237dc;
    }
  }
  else {
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x15d);
    DAT_00e20778 = 0;
    FUN_004ccc30(8,0x32,(&DAT_00ea5ed8)[DAT_00eaac28 * 0x49d8],DAT_00f53f0c + 0x46,
                 DAT_00f53f08 + 0x15c,DAT_00f56f8c,0);
    if ((&DAT_00ea5ed4)[DAT_00eaac28 * 0x49d8] != '\0') {
      FUN_004d62f0(0x33,0x20,DAT_00e20778 + 0x46 + DAT_00f53f0c,DAT_00f53f08 + 0x15c,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004ccc30(8,0x3a,(&DAT_00ea5ed4)[DAT_00eaac28 * 0x49d8],DAT_00e20778 + 0x46 + DAT_00f53f0c,
                   DAT_00f53f08 + 0x15c,DAT_00f56f8c,0);
    }
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x171);
    if ((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] == 0) {
      if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) {
        iVar6 = FUN_00446610();
        if (iVar6 == 0) {
          iVar8 = DAT_00f53f08 + 0x170;
          iVar6 = DAT_00f53f0c + 0x46;
          uVar13 = 8;
        }
        else {
          iVar8 = DAT_00f53f08 + 0x170;
          iVar6 = DAT_00f53f0c + 0x46;
          uVar13 = 9;
        }
      }
      else {
        iVar8 = DAT_00f53f08 + 0x170;
        iVar6 = DAT_00f53f0c + 0x46;
        uVar13 = 10;
      }
    }
    else {
      iVar8 = DAT_00f53f08 + 0x170;
      iVar6 = DAT_00f53f0c + 0x46;
      uVar13 = 0xb;
    }
    FUN_004d62f0(0x33,uVar13,iVar6,iVar8,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x185);
    if (cVar5 != '\0') {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0xf;
      goto LAB_005237dc;
    }
    if (bVar2) {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1c;
      goto LAB_005237dc;
    }
    if (bVar3) {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0xe;
      goto LAB_005237dc;
    }
    if (bVar4) {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1b;
      goto LAB_005237dc;
    }
    if (local_1 != '\0') {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0xd;
      goto LAB_005237dc;
    }
    if (local_3 != '\0') {
      iVar6 = DAT_00f53f08 + 0x184;
      iVar8 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1a;
      goto LAB_005237dc;
    }
  }
  iVar6 = DAT_00f53f08 + 0x184;
  iVar8 = DAT_00f53f0c + 0x46;
  uVar13 = 0xc;
LAB_005237dc: ;
  FUN_004d62f0(0x33,uVar13,iVar8,iVar6,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x46,0x24,0x11,DAT_00e38da8,DAT_00e38da4);
  if ((int)DAT_00f53dd0 < 1) {
    FUN_004d6160(0x33,0x19,DAT_00f53f0c + 0x40,DAT_00f53f08 + 200,0x1f0,DAT_00f56f94,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  else {
    iVar6 = 0;
    if (0 < (int)DAT_00f53dd0) {
      iVar8 = 0;
      do {
        iVar6 = iVar6 + 1;
        iVar7 = FUN_004bc620(iVar6);
        FUN_0053e590(DAT_00f56694,DAT_00f53f0c + 0x30,iVar8 + 0x52 + DAT_00f53f08);
        FUN_004d62f0(0x33,0x14,DAT_00f53f0c + 100,iVar8 + 0x53 + DAT_00f53f08,DAT_00f56f9c,0,
                     DAT_00e38da8,DAT_00e38da4);
        uVar13 = FUN_0050fc70(iVar7);
        FUN_004d6250(0xb8,uVar13,DAT_00f53f0c + 0x8c,iVar8 + 0x5b + DAT_00f53f08,0x96,DAT_00f56f94,0
                     ,DAT_00e38da8,DAT_00e38da4);
        iVar11 = iVar7 * 0x184;
        if (*(short *)(&DAT_00bcd72a + iVar11) < 0) {
          FUN_004d63d0(0,DAT_00f53f0c + 0x1b8,iVar8 + 0x53 + DAT_00f53f08,0x1e,0x1e);
          FUN_0053e590(DAT_00f56834,DAT_00f53f0c + 0x1bb,iVar8 + 0x56 + DAT_00f53f08);
          FUN_004d63d0(0,DAT_00f53f0c + 500,iVar8 + 0x53 + DAT_00f53f08,0x1e,0x1e,0,DAT_00e38da8,
                       DAT_00e38da4);
          cVar5 = FUN_00522d80(iVar7);
          if (cVar5 == '\0') {
            iVar10 = DAT_00f56834 + 1;
          }
          else {
            iVar10 = DAT_00f56834 + 2;
          }
          FUN_0053e590(iVar10,DAT_00f53f0c + 0x1f7,iVar8 + 0x56 + DAT_00f53f08);
          FUN_004d63d0(0,DAT_00f53f0c + 0x230,iVar8 + 0x53 + DAT_00f53f08,0x1e,0x1e,0,DAT_00e38da8,
                       DAT_00e38da4);
          iVar10 = DAT_00f53f08;
          if (local_3 == '\0') {
            iVar9 = DAT_00f53f0c + 0x233;
            (&DAT_00bcd5fa)[iVar11] = 0;
            FUN_0053e590(DAT_00f56834 + 5,iVar9,iVar8 + 0x56 + iVar10);
          }
          else if ((&DAT_00bcd5fa)[iVar11] == '\0') {
            FUN_0053e590(DAT_00f56834 + 4,DAT_00f53f0c + 0x233,iVar8 + 0x56 + DAT_00f53f08);
          }
          else {
            FUN_0053e590(DAT_00f56834 + 3,DAT_00f53f0c + 0x233,iVar8 + 0x56 + DAT_00f53f08);
          }
        }
        else {
          FUN_004d62f0(0x33,0x1d,DAT_00f53f0c + 500,iVar8 + 0x56 + DAT_00f53f08,DAT_00f56f94,0,
                       DAT_00e38da8,DAT_00e38da4);
        }
        uVar13 = FUN_0050fc10(iVar7);
        FUN_004d6250(0xb8,uVar13,DAT_00f53f0c + 0x111,iVar8 + 0x5b + DAT_00f53f08,0x96,DAT_00f56f94,
                     0,DAT_00e38da8,DAT_00e38da4);
        iVar8 = iVar8 + 0x2c;
      } while (iVar6 < (int)DAT_00f53dd0);
    }
  }
  FUN_0053e590(DAT_00f5643c,DAT_00f53f0c + 0x214,DAT_00f53f08 + 0x15b);
  return;
}
