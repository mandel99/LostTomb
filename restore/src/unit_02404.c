#include "restore_types.h"
#include "town_trade.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005c5878 (*(undefined4 *)0x005c5878u)
#define DAT_005c5894 (*(undefined4 *)0x005c5894u)
#define DAT_005c59c8 (*(undefined4 *)0x005c59c8u)
#define DAT_005e03fc (*(undefined4 *)0x005e03fcu)
#define DAT_005f89cc (*(undefined4 *)0x005f89ccu)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_00d38360 (*(undefined *)0x00d38360u)
#define DAT_00d386d0 (*(int *)0x00d386d0u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
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
#define DAT_00ea5fd4 (*(undefined4 *)0x00ea5fd4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f5690c (*(undefined *)0x00f5690cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define PTR_DAT_005c58b0 (*(unsigned char * *)0x005c58b0u)
#define PTR_DAT_005c58cc (*(unsigned char * *)0x005c58ccu)
#define PTR_DAT_005c58e8 (*(unsigned char * *)0x005c58e8u)
#define PTR_DAT_005c5904 (*(unsigned char * *)0x005c5904u)
#define PTR_DAT_005c5920 (*(unsigned char * *)0x005c5920u)
#define PTR_DAT_005c593c (*(unsigned char * *)0x005c593cu)
#define PTR_DAT_005c5958 (*(unsigned char * *)0x005c5958u)
#define PTR_DAT_005c5974 (*(unsigned char * *)0x005c5974u)
#define PTR_DAT_005c5990 (*(unsigned char * *)0x005c5990u)
#define PTR_DAT_005c59ac (*(unsigned char * *)0x005c59acu)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_00446610();
extern int FUN_00447530();
extern int * FUN_0044baf0();

extern int FUN_004bd590();
extern void FUN_004cd290();
extern void FUN_004cd680();
extern void FUN_004d6200();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_0052c570(void)

{
  char cVar1;
  short sVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  char cVar6;
  int iVar7;
  undefined4 uVar8;
  int iVar9;
  char cVar10;
  undefined4 uVar11;
  undefined4 uVar12;
  int iVar13;
  
  iVar7 = 5 - DAT_00f53f48;
  DAT_00f53f0c = DAT_00e38da8;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x18;
  if (4 < iVar7) {
    DAT_00f53f00 = 0x18;
    _DAT_00f53f04 = 0x28;
    DAT_00f53f08 = DAT_00e38da4;
    DAT_00f53f0c = DAT_00e38da8;
    return;
  }
  if (0 < iVar7) {
    FUN_004cd680(iVar7 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar7 * 8,(iVar7 * -2 + 0x28) * 0x10,
                 (iVar7 * -2 + 0x18) * 0x10,0);
    return;
  }
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x18);
  FUN_0053e590(DAT_00f5681c + 0xb,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
  FUN_004d62f0(0x3d,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x3d,0xda,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x15e,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d65b0(DAT_00f53f0c + 0x20,DAT_00f53f08 + 0x3c,0x24,0x11,DAT_00e38da8,DAT_00e38da4);
  iVar7 = 0;
  do {
    FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x28,iVar7 + 0x43 + DAT_00f53f08);
    iVar7 = iVar7 + 0x14;
  } while (iVar7 < 0xf0);
  FUN_004d6200(0x3d,1,&DAT_005c5878,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  iVar7 = *(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8);
  if (iVar7 < 1) {
    FUN_004d6200(0x3d,0x14,&DAT_005c5878,DAT_00f56fa4,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
    if ((&DAT_00ea5db0)[DAT_00eaac28 * 0x1276] != 0) {
      iVar7 = (&DAT_00ea5db0)[DAT_00eaac28 * 0x1276] + 0x20;
      uVar8 = DAT_00f56fa4;
LAB_0052c86e: ;
      iVar13 = DAT_00e20778 + 0x8c;
LAB_0052c86f: ;
      FUN_004d6200(0x3d,iVar7,&DAT_005c5878,uVar8,0,iVar13,0,DAT_00e38da8,DAT_00e38da4);
    }
  }
  else {
    if (99 < iVar7) {
      iVar13 = 0x8c;
      iVar7 = 0x1f;
      uVar8 = DAT_00f56f94;
      goto LAB_0052c86f;
    }
    uVar8 = DAT_00f56fa4;
    if ((0x1d < iVar7) && (uVar8 = DAT_00f56f9c, 0x31 < iVar7)) {
      uVar8 = DAT_00f56f94;
    }
    FUN_004d6200(0x3d,iVar7 / 10 + 0x15,&DAT_005c5878,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
    if ((*(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8) < 0x32) &&
       ((&DAT_00ea5db0)[DAT_00eaac28 * 0x1276] != 0)) {
      iVar7 = (&DAT_00ea5db0)[DAT_00eaac28 * 0x1276] + 0x1f;
      goto LAB_0052c86e;
    }
  }
  FUN_004d6200(0x3d,2,&DAT_005c5894,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  iVar7 = DAT_00eaac28 * 0x49d8;
  DAT_00e20778 = 0;
  uVar8 = DAT_00f56fa4;
  if (*(int *)(&DAT_00ea3fb0 + iVar7) == 0) {
    if ((int)((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] + (&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]) < 4)
    {
      if (*(int *)(&DAT_00ea3fac + iVar7) < 5) {
        if ((*(int *)(&DAT_00ea3f98 + iVar7) == 0) && (0 < *(int *)(&DAT_00ea18a8 + iVar7))) {
          if (*(int *)(&DAT_00ea3f9c + iVar7) < 0x50) {
            iVar7 = (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] + 0x2e;
          }
          else {
            iVar7 = 0x2c;
            uVar8 = DAT_00f56f94;
          }
        }
        else {
          iVar7 = 0x2d;
        }
      }
      else {
        iVar7 = 0x2c;
        uVar8 = DAT_00f56f94;
      }
    }
    else {
      iVar7 = 0x2b;
    }
  }
  else if (*(int *)(&DAT_00ea3fb0 + iVar7) == 1) {
    if ((*(int *)(&DAT_00ea3f98 + iVar7) == 0) && (0 < *(int *)(&DAT_00ea18a8 + iVar7))) {
      iVar7 = (&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] + 0x35;
    }
    else {
      iVar7 = 0x34;
    }
  }
  else {
    iVar7 = 0x3b;
    uVar8 = DAT_00f56f94;
  }
  FUN_004d6200(0x3d,iVar7,&DAT_005c5894,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6200(0x3d,3,&PTR_DAT_005c58b0,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  iVar13 = (&DAT_00ea446c)[DAT_00eaac28 * 0x1276];
  uVar8 = DAT_00f56fa4;
  if (iVar13 < 0x12) {
    if (iVar13 < 0xb) {
      if (iVar13 < 6) {
        if (iVar13 < 1) {
          iVar9 = (&DAT_00ea4474)[DAT_00eaac28 * 0x1276];
          if (iVar9 < 0x15) {
            if (iVar9 < 0xb) {
              uVar8 = DAT_00f56f94;
              if (iVar9 < 6) {
                if (0 < iVar9) {
                  iVar7 = 0x53;
                }
              }
              else {
                iVar7 = 0x52;
              }
            }
            else {
              iVar7 = 0x51;
              uVar8 = DAT_00f56f9c;
            }
          }
          else {
            iVar7 = 0x50;
          }
        }
        else {
          iVar7 = 0x4f;
          uVar8 = DAT_00f56f94;
        }
      }
      else {
        iVar7 = 0x4e;
        uVar8 = DAT_00f56f94;
      }
    }
    else {
      iVar7 = 0x4d;
      uVar8 = DAT_00f56f9c;
    }
  }
  else {
    iVar7 = 0x4c;
  }
  if ((iVar13 < 6) && ((int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276] < 6)) {
    FUN_004d6200(0x3d,0x54,&PTR_DAT_005c58b0,DAT_00f56f94,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  }
  else {
    FUN_004d6200(0x3d,iVar7,&PTR_DAT_005c58b0,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
    if (iVar13 < 1) {
      if (0 < (int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276]) {
        FUN_004cd290((&DAT_00ea4474)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,&PTR_DAT_005c58b0,
                     uVar8,0,DAT_00e20778 + 0x8c,0);
        FUN_004d6200(8,0xd,&PTR_DAT_005c58b0,uVar8,0,DAT_00e20778 + 0x8c,0,DAT_00e38da8,DAT_00e38da4
                    );
      }
    }
    else {
      FUN_004cd290((&DAT_00ea446c)[DAT_00eaac28 * 0x1276],0x40,&DAT_005e03fc,&PTR_DAT_005c58b0,uVar8
                   ,0,DAT_00e20778 + 0x8c,0);
    }
  }
  FUN_004d6200(0x3d,4,&PTR_DAT_005c58cc,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  uVar8 = DAT_00f56f94;
  if (DAT_00784bbc == 0) {
    if ((int)(&DAT_00ea4388)[DAT_00eaac28 * 0x1276] < 1) {
      iVar7 = 0x8c;
      uVar12 = 0x5f;
      uVar11 = 0x3d;
      uVar8 = DAT_00f56fa4;
    }
    else if ((*(int *)(&DAT_00ea41dc + DAT_00eaac28 * 0x49d8) * 100) /
             (int)(&DAT_00ea5e1c)[DAT_00eaac28 * 0x1276] < 0x32) {
      iVar7 = 0x8c;
      uVar12 = 0x60;
      uVar11 = 0x3d;
      uVar8 = DAT_00f56f9c;
    }
    else {
      iVar7 = 0x8c;
      if ((int)(&DAT_00ea4388)[DAT_00eaac28 * 0x1276] < 1) {
        uVar12 = 0x61;
        uVar11 = 0x3d;
      }
      else {
        FUN_004d6200(0x3d,0x62,&PTR_DAT_005c58cc,DAT_00f56f94,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
        FUN_004cd290((&DAT_00ea4388)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,&PTR_DAT_005c58cc,
                     DAT_00f56f94,0,DAT_00e20778 + 0x8c,0);
        iVar7 = DAT_00e20778 + 0x8c;
        uVar12 = 5;
        uVar11 = 8;
        uVar8 = DAT_00f56f94;
      }
    }
  }
  else {
    iVar7 = 0x8c;
    uVar12 = 0x5e;
    uVar11 = 0x3d;
  }
  FUN_004d6200(uVar11,uVar12,&PTR_DAT_005c58cc,uVar8,0,iVar7,0,DAT_00e38da8,DAT_00e38da4);
  uVar8 = DAT_00f56f94;
  FUN_004d6200(0x3d,5,&PTR_DAT_005c58e8,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  if (*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) != 0) {
    iVar7 = FUN_00447530();
    iVar7 = (iVar7 * 100) / *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8);
    if (0x96 < iVar7) {
      cVar6 = '\r';
      goto LAB_0052cd8c;
    }
    if (0x6e < iVar7) {
      cVar6 = '\x0e';
      goto LAB_0052cd8c;
    }
    if (0x5f < iVar7) {
      cVar6 = '\x0f';
      goto LAB_0052cd8c;
    }
    if (0x55 < iVar7) {
      cVar6 = '\x10';
      goto LAB_0052cd8c;
    }
    if (0x46 < iVar7) {
      cVar6 = '\x11';
      uVar8 = DAT_00f56f9c;
      goto LAB_0052cd8c;
    }
  }
  cVar6 = '\x12';
  uVar8 = DAT_00f56fa4;
LAB_0052cd8c: ;
  (FUN_004d6200(0x3d,cVar6,&PTR_DAT_005c58e8,uVar8,0,DAT_00e20778 + 0x8c,0,DAT_00e38da8,DAT_00e38da4), restore_unresolved_return(0x004d6200u))
  ;
  FUN_004d6200(0x3d,6,&PTR_DAT_005c5904,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  iVar7 = *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8);
  uVar8 = DAT_00f56fa4;
  if ((0x13 < iVar7) && (uVar8 = DAT_00f56f9c, 0x1d < iVar7)) {
    uVar8 = DAT_00f56f94;
  }
  iVar13 = 0;
  FUN_004d6200(0x3d,iVar7 / 10 + 0x67,&PTR_DAT_005c5904,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6200(0x3d,7,&PTR_DAT_005c5920,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  iVar7 = 0;
  DAT_00e20778 = 0;
  cVar10 = 'd';
  do {
    cVar1 = *(char *)((int)&DAT_00ea5c00 + iVar7 + DAT_00eaac28 * 0x49d8);
    if (cVar1 < '\x1e') {
      iVar13 = iVar13 + 1;
    }
    if (cVar1 < cVar10) {
      cVar10 = cVar1;
    }
    iVar7 = iVar7 + 1;
  } while (iVar7 < 5);
  iVar9 = 0;
  iVar7 = 0;
  do {
    if ('F' < *(char *)((int)&DAT_00ea5c00 + iVar7 + DAT_00eaac28 * 0x49d8)) {
      iVar9 = iVar9 + 1;
    }
    iVar7 = iVar7 + 1;
  } while (iVar7 < 5);
  uVar8 = DAT_00f56f94;
  if (iVar13 == 0) {
    if (iVar9 == 0) {
      cVar6 = -0x7b;
    }
    else if (iVar9 == 1) {
      cVar6 = -0x7a;
    }
    else if (iVar9 == 2) {
      cVar6 = -0x79;
    }
    else {
      cVar6 = (iVar9 != 3) + -0x78;
    }
  }
  else if (iVar13 < 4) {
    if (iVar13 == 3) {
      if (cVar10 < '\x14') {
        cVar6 = -0x80;
        uVar8 = DAT_00f56f9c;
      }
      else {
        cVar6 = '\x7f';
        uVar8 = DAT_00f56f94;
      }
    }
    else if (iVar13 == 2) {
      if (cVar10 < '\x14') {
        cVar6 = -0x7e;
        uVar8 = DAT_00f56f9c;
      }
      else {
        cVar6 = -0x7f;
        uVar8 = DAT_00f56f94;
      }
    }
    else if (iVar13 == 1) {
      if (cVar10 < '\x14') {
        cVar6 = -0x7c;
        uVar8 = DAT_00f56f9c;
      }
      else {
        cVar6 = -0x7d;
      }
    }
  }
  else if (cVar10 < '\x14') {
    cVar6 = '~';
    uVar8 = DAT_00f56f9c;
  }
  else {
    cVar6 = '}';
    uVar8 = DAT_00f56f94;
  }
  FUN_004d6200(0x3d,cVar6,&PTR_DAT_005c5920,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6200(0x3d,8,&PTR_DAT_005c593c,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  iVar7 = -1;
  DAT_00e20778 = 0;
  uVar8 = DAT_00f56f9c;
  if (((&DAT_00ea44b8)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44b4)[DAT_00eaac28 * 0x1276] == 0) ||
     (0 < (int)((&DAT_00ea44b0)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44ac)[DAT_00eaac28 * 0x1276]) /
          (int)((&DAT_00ea44b8)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44b4)[DAT_00eaac28 * 0x1276]))) {
    iVar13 = *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8);
    iVar9 = (&DAT_00ea452c)[DAT_00eaac28 * 0x1276];
    if (iVar9 < iVar13) {
      iVar7 = iVar13 - iVar9;
      uVar11 = 0x98;
      uVar8 = DAT_00f56f94;
    }
    else if (iVar13 < iVar9) {
      iVar7 = iVar9 - iVar13;
      uVar11 = 0x9a;
    }
    else {
      uVar11 = 0x99;
      uVar8 = DAT_00f56f94;
    }
  }
  else {
    uVar11 = 0x97;
  }
  FUN_004d6200(0x3d,uVar11,&PTR_DAT_005c593c,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  if (iVar7 != -1) {
    FUN_004cd290(iVar7,0x40,&DAT_005f89cc,&PTR_DAT_005c593c,uVar8,0,DAT_00e20778 + 0x8c,0);
  }
  FUN_004d6200(0x3d,9,&PTR_DAT_005c5958,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  iVar9 = DAT_00eaac28 * 0x49d8;
  iVar13 = -1;
  iVar7 = *(int *)(&DAT_00ea5fd0 + iVar9);
  DAT_00e20778 = 0;
  if (iVar7 < 10) {
    uVar8 = DAT_00f56f94;
    if (iVar7 == 0) {
      uVar11 = 0xa3;
    }
    else if (iVar7 < 2) {
      iVar13 = *(int *)(&DAT_00ea5fd4 + iVar9);
      uVar11 = 0xa2;
    }
    else if (iVar7 < 5) {
      iVar13 = *(int *)(&DAT_00ea5fd4 + iVar9);
      uVar11 = 0xa1;
    }
    else {
      iVar13 = *(int *)(&DAT_00ea5fd4 + iVar9);
      uVar11 = 0xa0;
      uVar8 = DAT_00f56f9c;
    }
  }
  else {
    iVar13 = *(int *)(&DAT_00ea5fd4 + iVar9);
    uVar11 = 0x9f;
    uVar8 = DAT_00f56fa4;
  }
  FUN_004d6200(0x3d,uVar11,&PTR_DAT_005c5958,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  if (iVar13 != -1) {
    FUN_004cd290(iVar13,0x40,&DAT_0057c048,&PTR_DAT_005c5958,uVar8,0,DAT_00e20778 + 0x8c,0);
    FUN_004d6200(0x3d,0xa4,&PTR_DAT_005c5958,uVar8,0,DAT_00e20778 + 0x8c,0,DAT_00e38da8,DAT_00e38da4
                );
  }
  bVar3 = false;
  bVar5 = false;
  bVar4 = false;
  for (iVar7 = FUN_0044baf0(0); iVar7 != 0; iVar7 = FUN_0044baf0(iVar7)) {
    if (*(short *)(iVar7 + 4) == 0x25) {
      cVar6 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)iVar7);
      if (cVar6 != '\0') {
        bVar5 = true;
      }
      sVar2 = *(short *)(iVar7 + 0x30);
      if (sVar2 == 5) {
        bVar4 = true;
      }
      if ((sVar2 == 6) || (sVar2 == 7)) {
        bVar3 = true;
      }
    }
  }
  FUN_004d6200(0x3d,10,&PTR_DAT_005c5974,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  uVar8 = DAT_00f56f94;
  DAT_00e20778 = 0;
  if ((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] == 0) {
    if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) {
      iVar7 = FUN_00446610();
      if (iVar7 == 0) {
        if (bVar3) {
          cVar6 = -0x50;
        }
        else if (bVar4) {
          cVar6 = -0x50;
        }
        else if (bVar5) {
          cVar6 = -0x51;
        }
        else {
          cVar6 = (-1 < *(int *)(&DAT_00ea5ee8 + DAT_00eaac28 * 0x49d8)) + -0x56;
        }
      }
      else {
        cVar6 = -0x54;
        uVar8 = DAT_00f56fa4;
      }
    }
    else {
      cVar6 = -0x53;
      uVar8 = DAT_00f56fa4;
    }
  }
  else {
    cVar6 = -0x52;
    uVar8 = DAT_00f56fa4;
  }
  FUN_004d6200(0x3d,cVar6,&PTR_DAT_005c5974,uVar8,0,DAT_00e20778 + 0x8c,0,DAT_00e38da8,DAT_00e38da4)
  ;
  iVar7 = 0;
  for (iVar13 = FUN_0044baf0(0); iVar13 != 0; iVar13 = FUN_0044baf0(iVar13)) {
    cVar6 = FUN_0044d4c0((const struct RestoreTownRecord *)(uintptr_t)iVar13);
    if (cVar6 != '\0') {
      if (4 < iVar7) break;
      if (*(short *)(iVar13 + 4) != 0) {
        iVar7 = iVar7 + 1;
      }
    }
  }
  uVar8 = DAT_00f56f94;
  FUN_004d6200(0x3d,0xb,&PTR_DAT_005c5990,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  if (iVar7 == 0) {
    cVar6 = -0x45;
  }
  else if (iVar7 < 3) {
    cVar6 = -0x46;
  }
  else {
    cVar6 = (iVar7 < 5) + -0x48;
  }
  FUN_004d6200(0x3d,cVar6,&PTR_DAT_005c5990,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6200(0x3d,0xc,&PTR_DAT_005c59ac,DAT_00f56f9c,0,0,0,DAT_00e38da8,DAT_00e38da4);
  DAT_00e20778 = 0;
  iVar7 = FUN_004bd590();
  iVar7 = (iVar7 + 0x13) / 0x14;
  uVar8 = DAT_00f56fa4;
  if ((iVar7 != 0) && (uVar8 = DAT_00f56f9c, iVar7 != 1)) {
    uVar8 = DAT_00f56f94;
  }
  FUN_004d6200(0x3d,iVar7 + 0xc0,&PTR_DAT_005c59ac,uVar8,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  if (iVar7 != 0) {
    FUN_004d6200(0x3d,(DAT_00d38360 % 0x1e) / 0xf + 0xc2 + DAT_00d386d0 * 2,&DAT_005c59c8,
                 DAT_00f56f94,0,0x8c,0,DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
