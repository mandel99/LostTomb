#include "restore_types.h"
#include "town_trade.h"
#define DAT_006fdc88 (*(undefined *)0x006fdc88u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea1880 (*(undefined4 *)0x00ea1880u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea18a8 (*(undefined4 *)0x00ea18a8u)
#define DAT_00ea3f98 (*(undefined4 *)0x00ea3f98u)
#define DAT_00ea3f9c (*(undefined4 *)0x00ea3f9cu)
#define DAT_00ea3fac (*(undefined4 *)0x00ea3facu)
#define DAT_00ea3fb0 (*(undefined4 *)0x00ea3fb0u)
#define DAT_00ea41dc (*(undefined4 *)0x00ea41dcu)
#define DAT_00ea4388 (*(undefined4 *)0x00ea4388u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea4474 (*(undefined4 *)0x00ea4474u)
#define DAT_00ea44ac (*(undefined4 *)0x00ea44acu)
#define DAT_00ea44b0 (*(undefined4 *)0x00ea44b0u)
#define DAT_00ea44b4 (*(undefined4 *)0x00ea44b4u)
#define DAT_00ea44b8 (*(undefined4 *)0x00ea44b8u)
#define DAT_00ea452c (*(undefined4 *)0x00ea452cu)
#define DAT_00ea5c00 (*(undefined4 *)0x00ea5c00u)
#define DAT_00ea5c80 (*(undefined4 *)0x00ea5c80u)
#define DAT_00ea5db0 (*(undefined4 *)0x00ea5db0u)
#define DAT_00ea5e1c (*(undefined4 *)0x00ea5e1cu)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eb4 (*(undefined4 *)0x00ea5eb4u)
#define DAT_00ea5ee8 (*(undefined4 *)0x00ea5ee8u)
#define DAT_00ea5fd0 (*(undefined4 *)0x00ea5fd0u)
#define DAT_00ea5fd8 (*(undefined4 *)0x00ea5fd8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define PTR_DAT_005f78f0 (*(unsigned char * *)0x005f78f0u)
extern void FUN_00425730();
extern undefined4 FUN_00446610();
extern int FUN_00447530();
extern int * FUN_0044baf0();

extern int FUN_004bd590();
extern void FUN_004cf090();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_00536820();

void FUN_00538050(void)

{
  char cVar1;
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
  
  iVar10 = -1;
  iVar11 = 0;
  bVar3 = false;
  bVar5 = false;
  bVar4 = false;
  switch((int)(DAT_00d3b198)) {
  case 0: ;
    switch((&DAT_00ea5db0)[DAT_00eaac28 * 0x1276]) {
    case 1: ;
    case 5: ;
      iVar11 = 7;
      break;
    case 2: ;
    case 4: ;
      iVar11 = 1;
      break;
    case 3: ;
      iVar11 = 0xc;
    }
    if (*(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8) < 0x32) {
      iVar10 = (&DAT_00ea5db0)[DAT_00eaac28 * 0x1276] + 0x25;
    }
    else if (*(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8) < 100) {
      iVar10 = 0x2a;
      iVar11 = 0;
    }
    break;
  case 1: ;
    iVar7 = DAT_00eaac28 * 0x49d8;
    iVar10 = (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276];
    switch((int)(iVar10)) {
    case 0: ;
    case 1: ;
      iVar11 = 1;
      break;
    case 2: ;
    case 3: ;
      iVar11 = 7;
    }
    if (*(int *)(&DAT_00ea3fb0 + iVar7) == 1) {
      if ((int)((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] + (&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]) < 4
         ) {
        if (*(int *)(&DAT_00ea3fac + iVar7) < 5) {
          if ((*(int *)(&DAT_00ea3f98 + iVar7) == 0) && (0 < *(int *)(&DAT_00ea18a8 + iVar7))) {
            if (*(int *)(&DAT_00ea3f9c + iVar7) < 0x50) {
              iVar10 = iVar10 + 0x3d;
            }
            else {
              iVar10 = 0x44;
              iVar11 = 0;
            }
          }
          else {
            iVar10 = 0x3c;
            iVar11 = 7;
          }
        }
        else {
          iVar10 = 0x44;
          iVar11 = 0;
        }
      }
      else {
        iVar10 = -1;
      }
    }
    else if (*(int *)(&DAT_00ea3fb0 + iVar7) == 0) {
      if ((*(int *)(&DAT_00ea3f98 + iVar7) == 0) && (0 < *(int *)(&DAT_00ea18a8 + iVar7))) {
        iVar10 = iVar10 + 0x46;
      }
      else {
        iVar11 = 7;
        iVar10 = 0x45;
      }
    }
    else {
      iVar10 = 0x43;
      iVar11 = 0;
    }
    break;
  case 2: ;
    iVar11 = 1;
    iVar7 = (&DAT_00ea446c)[DAT_00eaac28 * 0x1276];
    if (iVar7 < 0x12) {
      if (iVar7 < 0xb) {
        if (iVar7 < 6) {
          if (iVar7 < 1) {
            iVar9 = (&DAT_00ea4474)[DAT_00eaac28 * 0x1276];
            if (iVar9 < 0x15) {
              if (iVar9 < 0xb) {
                if (iVar9 < 6) {
                  if (0 < iVar9) {
                    iVar10 = 0x5c;
                  }
                }
                else {
                  iVar10 = 0x5b;
                }
              }
              else {
                iVar10 = 0x5a;
              }
            }
            else {
              iVar10 = 0x59;
            }
          }
          else {
            iVar10 = 0x58;
          }
        }
        else {
          iVar10 = 0x57;
        }
      }
      else {
        iVar10 = 0x56;
      }
    }
    else {
      iVar10 = 0x55;
    }
    if ((iVar7 < 6) && ((int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276] < 6)) {
      iVar10 = 0x5d;
    }
    break;
  case 3: ;
    if ((int)(&DAT_00ea4388)[DAT_00eaac28 * 0x1276] < 1) {
      iVar10 = 99;
      iVar11 = 7;
    }
    else if ((*(int *)(&DAT_00ea41dc + DAT_00eaac28 * 0x49d8) * 100) /
             (int)(&DAT_00ea5e1c)[DAT_00eaac28 * 0x1276] < 0x32) {
      iVar10 = 100;
      iVar11 = 7;
    }
    else {
      iVar10 = (0 < (int)(&DAT_00ea4388)[DAT_00eaac28 * 0x1276]) + 0x65;
      iVar11 = 7;
    }
    break;
  case 4: ;
    if (*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) == 0) {
LAB_00538343: ;
      iVar10 = 0xd4;
    }
    else {
      iVar10 = FUN_00447530();
      iVar11 = (iVar10 * 100) / *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8);
      if (0x96 < iVar11) {
        iVar10 = 0xd9;
        iVar11 = 7;
        break;
      }
      if (0x6e < iVar11) {
        iVar10 = 0xd8;
        iVar11 = 7;
        break;
      }
      if (0x5f < iVar11) {
        iVar10 = 0xd7;
        iVar11 = 7;
        break;
      }
      if (0x55 < iVar11) {
        iVar10 = 0xd6;
        iVar11 = 7;
        break;
      }
      iVar10 = 0xd5;
      if (iVar11 < 0x47) goto LAB_00538343;
    }
    iVar11 = 7;
    break;
  case 5: ;
    iVar11 = 8;
    iVar10 = *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8) / 10 + 0x72;
    break;
  case 6: ;
    iVar9 = 0;
    iVar7 = 0;
    iVar11 = 0xb;
    cVar6 = 'd';
    do {
      cVar1 = *(char *)((int)&DAT_00ea5c00 + iVar7 + DAT_00eaac28 * 0x49d8);
      if (cVar1 < '(') {
        iVar9 = iVar9 + 1;
      }
      if (cVar1 < cVar6) {
        cVar6 = cVar1;
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < 5);
    iVar8 = 0;
    iVar7 = 0;
    do {
      if ('<' < *(char *)((int)&DAT_00ea5c00 + iVar7 + DAT_00eaac28 * 0x49d8)) {
        iVar8 = iVar8 + 1;
      }
      iVar7 = iVar7 + 1;
    } while (iVar7 < 5);
    if (iVar9 == 0) {
      if (iVar8 == 0) {
        iVar10 = 0x92;
      }
      else if (iVar8 == 1) {
        iVar10 = 0x93;
      }
      else if (iVar8 == 2) {
        iVar10 = 0x94;
      }
    }
    else if (iVar9 < 4) {
      if (iVar9 == 3) {
        iVar10 = (cVar6 < '\x14') + 0x8c;
      }
      else if (iVar9 == 2) {
        iVar10 = (cVar6 < '\x14') + 0x8e;
      }
      else if (iVar9 == 1) {
        iVar10 = (cVar6 < '\x14') + 0x90;
      }
    }
    else {
      iVar10 = (cVar6 < '\x14') + 0x8a;
    }
    break;
  case 7: ;
    iVar11 = 0xc;
    if (((&DAT_00ea44b8)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44b4)[DAT_00eaac28 * 0x1276] == 0) ||
       (0 < (int)((&DAT_00ea44b0)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44ac)[DAT_00eaac28 * 0x1276]) /
            (int)((&DAT_00ea44b8)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44b4)[DAT_00eaac28 * 0x1276])))
    {
      if ((int)(&DAT_00ea452c)[DAT_00eaac28 * 0x1276] <
          *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8)) {
        iVar10 = 0x9c;
      }
      else {
        iVar10 = (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) <
                 (int)(&DAT_00ea452c)[DAT_00eaac28 * 0x1276]) + 0x9d;
      }
    }
    else {
      iVar10 = 0x9b;
    }
    break;
  case 8: ;
    iVar10 = *(int *)(&DAT_00ea5fd0 + DAT_00eaac28 * 0x49d8);
    if (iVar10 < 10) {
      if (iVar10 == 0) {
        iVar10 = 0xa9;
        iVar11 = 0xc;
      }
      else if (iVar10 < 2) {
        iVar10 = 0xa8;
        iVar11 = 0xc;
      }
      else {
        iVar10 = (iVar10 < 5) + 0xa6;
        iVar11 = 0xc;
      }
    }
    else {
      iVar10 = 0xa5;
      iVar11 = 0xc;
    }
    break;
  case 9: ;
    for (iVar10 = FUN_0044baf0(0); iVar10 != 0; iVar10 = FUN_0044baf0(iVar10)) {
      if (*(short *)(iVar10 + 4) == 0x25) {
        cVar6 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)iVar10);
        if (cVar6 != '\0') {
          bVar5 = true;
        }
        sVar2 = *(short *)(iVar10 + 0x30);
        if (sVar2 == 5) {
          bVar4 = true;
        }
        if ((sVar2 == 6) || (sVar2 == 7)) {
          bVar3 = true;
        }
      }
    }
    if ((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] == 0) {
      if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) {
        iVar10 = FUN_00446610();
        if (iVar10 == 0) {
          if (bVar3) {
            iVar10 = 0xb7;
            iVar11 = 2;
          }
          else if (bVar4) {
            iVar10 = 0xb7;
            iVar11 = 2;
          }
          else if (bVar5) {
            iVar10 = 0xb6;
            iVar11 = 2;
          }
          else {
            iVar11 = 2;
            iVar10 = (-1 < *(int *)(&DAT_00ea5ee8 + DAT_00eaac28 * 0x49d8)) + 0xb1;
          }
        }
        else {
          iVar10 = 0xb3;
          iVar11 = 2;
        }
      }
      else {
        iVar10 = 0xb4;
        iVar11 = 2;
      }
    }
    else {
      iVar10 = 0xb5;
      iVar11 = 2;
    }
    break;
  case 10: ;
    iVar10 = 0;
    for (iVar11 = FUN_0044baf0(0); iVar11 != 0; iVar11 = FUN_0044baf0(iVar11)) {
      cVar6 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)iVar11);
      if (cVar6 != '\0') {
        if (4 < iVar10) break;
        if (*(short *)(iVar11 + 4) != 0) {
          iVar10 = iVar10 + 1;
        }
      }
    }
    iVar11 = 4;
    if (iVar10 == 0) {
      iVar10 = 0xbf;
    }
    else if (iVar10 < 3) {
      iVar10 = 0xbe;
    }
    else {
      iVar10 = (iVar10 < 5) + 0xbc;
    }
    break;
  case 0xb: ;
    iVar11 = 7;
    iVar10 = FUN_004bd590();
    iVar10 = (iVar10 + 0x13) / 0x14 + 0xc6;
  }
  *(int *)(&DAT_00ea5fd8 + DAT_00eaac28 * 0x49d8) = iVar11;
  if (-1 < iVar10) {
    FUN_004cf090(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x3c,0x240,0x110,0);
    FUN_00536820(DAT_00f53f0c + 0x10,DAT_00f53f08 + 0x90,0x26,0xb);
    FUN_004d6160(0x3d,iVar10,DAT_00f53f0c + 0x20,DAT_00f53f08 + 0xa0,0x264,DAT_00f56f8c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d6250(0xd,1,DAT_00f53f0c + 0x10,DAT_00f53f08 + 0x128,0x260,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    if ((&DAT_006fdc88)[iVar11] != '\0') {
      FUN_00425730(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x110,(&PTR_DAT_005f78f0)[iVar11],1);
    }
  }
  return;
}
