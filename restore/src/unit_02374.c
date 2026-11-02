#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_007770f0 (*(undefined4 *)0x007770f0u)
#define DAT_00bcb9ca (*(undefined4 *)0x00bcb9cau)
#define DAT_00bcb9cb (*(undefined4 *)0x00bcb9cbu)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f4 (*(undefined4 *)0x00bcb9f4u)
#define DAT_00bcba2f (*(undefined4 *)0x00bcba2fu)
#define DAT_00bcba42 (*(undefined4 *)0x00bcba42u)
#define DAT_00bcba4a (*(undefined4 *)0x00bcba4au)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eb4 (*(undefined4 *)0x00ea5eb4u)
#define DAT_00ea5edc (*(undefined4 *)0x00ea5edcu)
#define DAT_00ea5edf (*(undefined4 *)0x00ea5edfu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53dd4 (*(undefined *)0x00f53dd4u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5642c (*(undefined *)0x00f5642cu)
#define DAT_00f56434 (*(undefined *)0x00f56434u)
#define DAT_00f5643c (*(undefined *)0x00f5643cu)
#define DAT_00f56814 (*(undefined *)0x00f56814u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56834 (*(undefined *)0x00f56834u)
#define DAT_00f5690c (*(undefined *)0x00f5690cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_0040b5e0();
extern undefined4 FUN_00446610();
extern int * FUN_0044baf0();
extern int FUN_004b78b0();
extern int FUN_004b78f0();
extern void FUN_004ccc30();
extern void FUN_004cd680();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00521a60();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00521b50(void)

{
  byte bVar1;
  short sVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  bool bVar12;
  undefined4 uVar13;
  int iVar14;
  char local_3;
  char local_2;
  char local_1;
  
  DAT_00f53f0c = DAT_00e38da8;
  iVar7 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1a;
  if (4 < iVar7) {
    DAT_00f53f00 = 0x1a;
    _DAT_00f53f04 = 0x28;
    DAT_00f53f08 = DAT_00e38da4;
    DAT_00f53f0c = DAT_00e38da8;
    return;
  }
  if (0 < iVar7) {
    FUN_004cd680(iVar7 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar7 * 8,(iVar7 * -2 + 0x28) * 0x10,
                 (iVar7 * -2 + 0x1a) * 0x10,0);
    return;
  }
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1a);
  FUN_0053e590(DAT_00f5681c + 1,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
  FUN_004d62f0(0x33,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,1,DAT_00f53f0c + 0x1a9,DAT_00f53f08 + 0x2b,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,2,DAT_00f53f0c + 0x1a9,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,3,DAT_00f53f0c + 0x1ea,DAT_00f53f08 + 0x2b,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,4,DAT_00f53f0c + 0x1ea,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,0x11,DAT_00f53f0c + 0x15e,DAT_00f53f08 + 0x2b,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x33,0x12,DAT_00f53f0c + 0x14a,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x8a,0x24,DAT_00f53f0c + 0xfa,DAT_00f53f08 + 0x3a,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  DAT_00f53dd4 = FUN_004b78f0();
  FUN_00521a60(&local_2,&local_3);
  local_1 = '\0';
  bVar3 = false;
  bVar4 = false;
  bVar5 = false;
  iVar7 = FUN_0044baf0(0);
  cVar6 = '\0';
  while (iVar7 != 0) {
    if (*(short *)(iVar7 + 4) == 0x25) {
      iVar10 = (int)*(short *)(iVar7 + 0x1c);
      bVar12 = false;
      if ((-1 < iVar10) && (iVar10 < 0x32)) {
        bVar12 = (&DAT_007770f0)[iVar10 * 0x144] == '\x02';
      }
      sVar2 = *(short *)(iVar7 + 0x30);
      if (sVar2 == 5) {
        if (bVar12) {
          bVar5 = true;
        }
        else {
          bVar4 = true;
        }
      }
      if ((sVar2 == 6) || (sVar2 == 7)) {
        if (bVar12) {
          bVar3 = true;
        }
        else {
          local_1 = '\x01';
        }
      }
    }
    iVar7 = FUN_0044baf0(iVar7);
    cVar6 = local_1;
  }
  if (DAT_00f53dd4 < 1) {
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x167);
    if ((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] == 0) {
      if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) {
        iVar7 = FUN_00446610();
        if (iVar7 == 0) {
          iVar7 = DAT_00f53f08 + 0x166;
          iVar10 = DAT_00f53f0c + 0x46;
          uVar13 = 8;
        }
        else {
          iVar7 = DAT_00f53f08 + 0x166;
          iVar10 = DAT_00f53f0c + 0x46;
          uVar13 = 9;
        }
      }
      else {
        iVar7 = DAT_00f53f08 + 0x166;
        iVar10 = DAT_00f53f0c + 0x46;
        uVar13 = 10;
      }
    }
    else {
      iVar7 = DAT_00f53f08 + 0x166;
      iVar10 = DAT_00f53f0c + 0x46;
      uVar13 = 0xb;
    }
    FUN_004d62f0(0x33,uVar13,iVar10,iVar7,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x185);
    if (cVar6 != '\0') {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0xf;
      goto LAB_005224f7;
    }
    if (bVar3) {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1c;
      goto LAB_005224f7;
    }
    if (bVar4) {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0xe;
      goto LAB_005224f7;
    }
    if (bVar5) {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1b;
      goto LAB_005224f7;
    }
    if (local_2 != '\0') {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0xd;
      goto LAB_005224f7;
    }
    if (local_3 != '\0') {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1a;
      goto LAB_005224f7;
    }
  }
  else {
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x15d);
    DAT_00e20778 = 0;
    FUN_004ccc30(8,0x2e,(&DAT_00ea5edf)[DAT_00eaac28 * 0x49d8],DAT_00f53f0c + 0x46,
                 DAT_00f53f08 + 0x15c,DAT_00f56f8c,0);
    FUN_004d62f0(0x33,7,DAT_00e20778 + 0x46 + DAT_00f53f0c,DAT_00f53f08 + 0x15c,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004ccc30(8,0x30,(&DAT_00ea5edc)[DAT_00eaac28 * 0x49d8],DAT_00e20778 + 0x46 + DAT_00f53f0c,
                 DAT_00f53f08 + 0x15c,DAT_00f56f8c,0);
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x171);
    if ((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] == 0) {
      if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) {
        iVar7 = FUN_00446610();
        if (iVar7 == 0) {
          iVar7 = DAT_00f53f08 + 0x170;
          iVar10 = DAT_00f53f0c + 0x46;
          uVar13 = 8;
        }
        else {
          iVar7 = DAT_00f53f08 + 0x170;
          iVar10 = DAT_00f53f0c + 0x46;
          uVar13 = 9;
        }
      }
      else {
        iVar7 = DAT_00f53f08 + 0x170;
        iVar10 = DAT_00f53f0c + 0x46;
        uVar13 = 10;
      }
    }
    else {
      iVar7 = DAT_00f53f08 + 0x170;
      iVar10 = DAT_00f53f0c + 0x46;
      uVar13 = 0xb;
    }
    FUN_004d62f0(0x33,uVar13,iVar10,iVar7,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x32,DAT_00f53f08 + 0x185);
    if (cVar6 != '\0') {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0xf;
      goto LAB_005224f7;
    }
    if (bVar3) {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1c;
      goto LAB_005224f7;
    }
    if (bVar4) {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0xe;
      goto LAB_005224f7;
    }
    if (bVar5) {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1b;
      goto LAB_005224f7;
    }
    if (local_2 != '\0') {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0xd;
      goto LAB_005224f7;
    }
    if (local_3 != '\0') {
      iVar10 = DAT_00f53f08 + 0x184;
      iVar7 = DAT_00f53f0c + 0x46;
      uVar13 = 0x1a;
      goto LAB_005224f7;
    }
  }
  iVar10 = DAT_00f53f08 + 0x184;
  iVar7 = DAT_00f53f0c + 0x46;
  uVar13 = 0xc;
LAB_005224f7: ;
  FUN_004d62f0(0x33,uVar13,iVar7,iVar10,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x46,0x24,0x11,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00f53dd4 < 1) {
    FUN_004d6160(0x33,0x10,DAT_00f53f0c + 0x40,DAT_00f53f08 + 200,0x1f0,DAT_00f56f94,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  else {
    iVar7 = 0;
    if (0 < DAT_00f53dd4) {
      iVar10 = 0;
      do {
        iVar7 = iVar7 + 1;
        iVar8 = FUN_004b78b0(iVar7);
        iVar9 = FUN_0040b5e0(iVar8);
        if (iVar9 == 0) {
          bVar1 = (&DAT_00bcb9ca)[iVar8 * 0x90];
          iVar11 = DAT_00f56814;
        }
        else {
          bVar1 = (&DAT_00bcb9ca)[iVar8 * 0x90];
          iVar11 = DAT_00f56434;
        }
        iVar8 = iVar8 * 0x90;
        FUN_0053e590((uint)bVar1 + iVar11,DAT_00f53f0c + 0x30,iVar10 + 0x52 + DAT_00f53f08);
        FUN_004d62f0(0x8a,(&DAT_00bcb9ca)[iVar8],DAT_00f53f0c + 100,iVar10 + 0x53 + DAT_00f53f08,
                     DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
        DAT_00e20778 = 0;
        FUN_004d6860((&DAT_00bcb9f0)[iVar8],0x40,&DAT_0057c048,DAT_00f53f0c + 100,
                     iVar10 + 100 + DAT_00f53f08,DAT_00f56f94,0,DAT_00e38da8,DAT_00e38da4);
        if (*(short *)(&DAT_00bcb9cc + iVar8) == 0xd) {
          iVar11 = iVar10 + 100 + DAT_00f53f08;
          iVar14 = DAT_00e20778 + 100 + DAT_00f53f0c;
          uVar13 = 0x21;
        }
        else if (*(short *)(&DAT_00bcb9cc + iVar8) == 0xb) {
          iVar11 = iVar10 + 100 + DAT_00f53f08;
          iVar14 = DAT_00e20778 + 100 + DAT_00f53f0c;
          uVar13 = 0x23;
        }
        else {
          iVar11 = iVar10 + 100 + DAT_00f53f08;
          iVar14 = DAT_00e20778 + 100 + DAT_00f53f0c;
          uVar13 = 0x22;
        }
        FUN_004d62f0(0x8a,uVar13,iVar14,iVar11,DAT_00f56f94,0,DAT_00e38da8,DAT_00e38da4);
        FUN_004d6250(0x8a,(int)*(short *)(&DAT_00bcb9f4 + iVar8) / 5 + 0x25,DAT_00f53f0c + 200,
                     iVar10 + 0x5b + DAT_00f53f08,0x96,DAT_00f56f94,0,DAT_00e38da8,DAT_00e38da4);
        if (*(short *)(&DAT_00bcba4a + iVar8) < 0) {
          FUN_004d63d0(0,DAT_00f53f0c + 0x1b8,iVar10 + 0x53 + DAT_00f53f08,0x1e,0x1e);
          FUN_0053e590(DAT_00f56834,DAT_00f53f0c + 0x1bb,iVar10 + 0x56 + DAT_00f53f08);
          FUN_004d63d0(0,DAT_00f53f0c + 500,iVar10 + 0x53 + DAT_00f53f08,0x1e,0x1e,0,DAT_00e38da8,
                       DAT_00e38da4);
          if ((&DAT_00bcb9cb)[iVar8] == '\0') {
            iVar11 = DAT_00f56834 + 1;
          }
          else {
            iVar11 = DAT_00f56834 + 2;
          }
          FUN_0053e590(iVar11,DAT_00f53f0c + 0x1f7,iVar10 + 0x56 + DAT_00f53f08);
          FUN_004d63d0(0,DAT_00f53f0c + 0x230,iVar10 + 0x53 + DAT_00f53f08,0x1e,0x1e,0,DAT_00e38da8,
                       DAT_00e38da4);
          iVar11 = DAT_00f53f08;
          cVar6 = local_2;
          if (iVar9 != 0) {
            cVar6 = local_3;
          }
          if (cVar6 == '\0') {
            iVar9 = DAT_00f53f0c + 0x233;
            (&DAT_00bcba2f)[iVar8] = 0;
            FUN_0053e590(DAT_00f56834 + 5,iVar9,iVar10 + 0x56 + iVar11);
          }
          else if ((&DAT_00bcba2f)[iVar8] == '\0') {
            FUN_0053e590(DAT_00f56834 + 4,DAT_00f53f0c + 0x233,iVar10 + 0x56 + DAT_00f53f08);
          }
          else {
            FUN_0053e590(DAT_00f56834 + 3,DAT_00f53f0c + 0x233,iVar10 + 0x56 + DAT_00f53f08);
          }
        }
        else {
          FUN_004d62f0(0x33,0x1d,DAT_00f53f0c + 500,iVar10 + 0x56 + DAT_00f53f08,DAT_00f56f94,0,
                       DAT_00e38da8,DAT_00e38da4);
        }
        FUN_004d63d0(0,DAT_00f53f0c + 0x168,iVar10 + 0x53 + DAT_00f53f08,0x1e,0x1e,0,DAT_00e38da8,
                     DAT_00e38da4);
        FUN_0053e590((*(short *)(&DAT_00bcba42 + iVar8) + 10) / 0x14 + DAT_00f5642c,
                     DAT_00f53f0c + 0x16b,iVar10 + 0x56 + DAT_00f53f08);
        iVar10 = iVar10 + 0x2c;
      } while (iVar7 < DAT_00f53dd4);
    }
  }
  FUN_0053e590(DAT_00f5643c + 1,DAT_00f53f0c + 0x214,DAT_00f53f08 + 0x15b);
  return;
}
