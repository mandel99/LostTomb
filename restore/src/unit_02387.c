#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00605a40 (*(int *)0x00605a40u)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea18a8 (*(undefined4 *)0x00ea18a8u)
#define DAT_00ea3f98 (*(undefined4 *)0x00ea3f98u)
#define DAT_00ea3f9c (*(undefined4 *)0x00ea3f9cu)
#define DAT_00ea3fac (*(undefined4 *)0x00ea3facu)
#define DAT_00ea3fb0 (*(undefined4 *)0x00ea3fb0u)
#define DAT_00ea41dc (*(undefined4 *)0x00ea41dcu)
#define DAT_00ea4388 (*(undefined4 *)0x00ea4388u)
#define DAT_00ea438c (*(undefined4 *)0x00ea438cu)
#define DAT_00ea5c80 (*(undefined4 *)0x00ea5c80u)
#define DAT_00ea5e1c (*(undefined4 *)0x00ea5e1cu)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eb4 (*(undefined4 *)0x00ea5eb4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f55c5c (*(undefined *)0x00f55c5cu)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f5690c (*(undefined *)0x00f5690cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern int FUN_00447530();
extern void FUN_004ccc30();
extern void FUN_004cd680();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00527360();
extern void FUN_00527710();
extern void FUN_005278e0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_00525cb0(void)

{
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  undefined4 uVar4;
  
  DAT_00f53f0c = DAT_00e38da8;
  iVar1 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1b;
  if (4 < iVar1) {
    DAT_00f53f00 = 0x1b;
    _DAT_00f53f04 = 0x28;
    DAT_00f53f08 = DAT_00e38da4;
    DAT_00f53f0c = DAT_00e38da8;
    return;
  }
  if (0 < iVar1) {
    FUN_004cd680(iVar1 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar1 * 8,(iVar1 * -2 + 0x28) * 0x10,
                 (iVar1 * -2 + 0x1b) * 0x10,0);
    return;
  }
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1b);
  FUN_0053e590(DAT_00f5681c + 5,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
  if (DAT_00605a40 < 4) {
    if (1 < DAT_00605a40) {
      uVar4 = 1;
      goto LAB_00525df5;
    }
    if (-1 < DAT_00605a40) {
      uVar4 = 0;
      goto LAB_00525df5;
    }
  }
  else {
    uVar4 = 2;
LAB_00525df5: ;
    FUN_004d62f0(0x37,uVar4,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  FUN_0053e590(DAT_00f55c5c + 0xe,DAT_00f53f0c + 0x38,DAT_00f53f08 + 0x3c);
  FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0x3c,0x6a,0x39,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0xa0,0x6a,0x39,0,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00605a40 == 0) {
    FUN_004d6250(0x37,6,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x118,400,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6250(0x37,4,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0x78,100,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6250(0x37,5,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0xdc,100,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_00527360(1,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x40);
    FUN_00527710(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0x3f);
  }
  else {
    if (DAT_00605a40 == 1) {
      FUN_004d6250(0x37,6,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x118,400,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      FUN_004d6250(0x37,5,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0x78,100,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      FUN_004d6250(0x37,4,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0xdc,100,DAT_00f56f8c,0,DAT_00e38da8,
                   DAT_00e38da4);
      FUN_00527360(1,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x40);
      FUN_005278e0(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0x3f);
LAB_00526354: ;
      FUN_00527710(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0xa3);
      goto LAB_00526483;
    }
    if (DAT_00605a40 != 2) {
      if (DAT_00605a40 == 3) {
        FUN_004d6250(0x37,7,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x118,400,DAT_00f56f8c,0,DAT_00e38da8
                     ,DAT_00e38da4);
        FUN_004d6250(0x37,5,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0x78,100,DAT_00f56f8c,0,DAT_00e38da8
                     ,DAT_00e38da4);
        FUN_004d6250(0x37,3,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0xdc,100,DAT_00f56f8c,0,DAT_00e38da8
                     ,DAT_00e38da4);
        FUN_00527710(1,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x40);
        FUN_005278e0(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0x3f);
      }
      else {
        if (DAT_00605a40 == 4) {
          FUN_004d6250(0x37,8,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x118,400,DAT_00f56f8c,0,
                       DAT_00e38da8,DAT_00e38da4);
          FUN_004d6250(0x37,3,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0x78,100,DAT_00f56f8c,0,
                       DAT_00e38da8,DAT_00e38da4);
          FUN_004d6250(0x37,4,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0xdc,100,DAT_00f56f8c,0,
                       DAT_00e38da8,DAT_00e38da4);
          FUN_005278e0(1,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x40);
          FUN_00527360(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0x3f);
          goto LAB_00526354;
        }
        if (DAT_00605a40 != 5) goto LAB_00526483;
        FUN_004d6250(0x37,8,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x118,400,DAT_00f56f8c,0,DAT_00e38da8
                     ,DAT_00e38da4);
        FUN_004d6250(0x37,4,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0x78,100,DAT_00f56f8c,0,DAT_00e38da8
                     ,DAT_00e38da4);
        FUN_004d6250(0x37,3,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0xdc,100,DAT_00f56f8c,0,DAT_00e38da8
                     ,DAT_00e38da4);
        FUN_005278e0(1,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x40);
        FUN_00527710(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0x3f);
      }
      FUN_00527360(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0xa3);
      goto LAB_00526483;
    }
    FUN_004d6250(0x37,7,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x118,400,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6250(0x37,3,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0x78,100,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6250(0x37,5,DAT_00f53f0c + 0x1f8,DAT_00f53f08 + 0xdc,100,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_00527710(1,DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x40);
    FUN_00527360(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0x3f);
  }
  FUN_005278e0(0,DAT_00f53f0c + 0x1f9,DAT_00f53f08 + 0xa3);
LAB_00526483: ;
  FUN_004d65b0(DAT_00f53f0c + 0x30,DAT_00f53f08 + 0x131,0x22,7,DAT_00e38da8,DAT_00e38da4);
  FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x38,DAT_00f53f08 + 0x136);
  FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x38,DAT_00f53f08 + 0x158);
  FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x38,DAT_00f53f08 + 0x16a);
  FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x38,DAT_00f53f08 + 0x17c);
  FUN_0053e590(DAT_00f5690c,DAT_00f53f0c + 0x38,DAT_00f53f08 + 0x18e);
  if (*(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8) == 0) {
    iVar1 = 0;
  }
  else {
    iVar1 = FUN_00447530();
    iVar1 = (iVar1 * 100) / *(int *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8);
  }
  uVar2 = FUN_00447530();
  uVar4 = DAT_00f56f94;
  if (iVar1 < 0x97) {
    if (iVar1 < 0x6f) {
      if (iVar1 < 0x60) {
        if (iVar1 < 0x56) {
          if (iVar1 < 0x47) {
            iVar1 = 0x24;
            uVar4 = DAT_00f56fa4;
          }
          else {
            iVar1 = 0x27;
            uVar4 = DAT_00f56f9c;
          }
        }
        else {
          iVar1 = 0x2a;
        }
      }
      else {
        iVar1 = 0x2d;
      }
    }
    else {
      iVar1 = 0x30;
    }
  }
  else {
    iVar1 = 0x33;
  }
  if (*(int *)(&DAT_00ea3f9c + DAT_00eaac28 * 0x49d8) < 0) {
    iVar1 = iVar1 + 1;
  }
  else if (0 < *(int *)(&DAT_00ea3f9c + DAT_00eaac28 * 0x49d8)) {
    iVar1 = iVar1 + 2;
  }
  DAT_00e20778 = 0;
  FUN_004d62f0(0x37,0x22,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x134,DAT_00f56f94,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6860(uVar2,0x40,&DAT_0057c048,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x134,
               DAT_00f56f94,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x37,0x23,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x134,DAT_00f56f94,0,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x37,iVar1,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x144,uVar4,0,DAT_00e38da8,DAT_00e38da4
              );
  if (DAT_00784bbc == 0) {
    DAT_00e20778 = 0;
    FUN_004ccc30(8,6,(&DAT_00ea438c)[DAT_00eaac28 * 0x1276],DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x156
                 ,DAT_00f56f9c,0);
    if ((int)(&DAT_00ea4388)[DAT_00eaac28 * 0x1276] < 1) {
      if ((int)(&DAT_00ea5e1c)[DAT_00eaac28 * 0x1276] / 2 <
          *(int *)(&DAT_00ea41dc + DAT_00eaac28 * 0x49d8)) {
        FUN_004d62f0(0x37,0xd,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x156,DAT_00f56f9c,0
                     ,DAT_00e38da8,DAT_00e38da4);
      }
      else if (*(int *)(&DAT_00ea41dc + DAT_00eaac28 * 0x49d8) < 1) {
        FUN_004d62f0(0x37,0xe,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x156,DAT_00f56f9c,0
                     ,DAT_00e38da8,DAT_00e38da4);
      }
      else {
        FUN_004d62f0(0x37,0xf,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x156,DAT_00f56f9c,0
                     ,DAT_00e38da8,DAT_00e38da4);
      }
    }
    else {
      FUN_004d62f0(0x37,0xc,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x156,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004ccc30(8,4,(&DAT_00ea4388)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0x4b + DAT_00f53f0c,
                   DAT_00f53f08 + 0x156,DAT_00f56f9c,0);
    }
  }
  else {
    FUN_004d62f0(0x37,0xb,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x156,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  uVar4 = *(undefined4 *)(&DAT_00ea18a8 + DAT_00eaac28 * 0x49d8);
  DAT_00e20778 = 0;
  FUN_004d62f0(0x37,0x10,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x168,DAT_00f56f9c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6860(uVar4,0x40,&DAT_0057c048,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x168,
               DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x37,0x11,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x168,DAT_00f56f9c,0,
               DAT_00e38da8,DAT_00e38da4);
  iVar3 = DAT_00eaac28 * 0x49d8;
  iVar1 = *(int *)(&DAT_00ea3fb0 + iVar3);
  if (iVar1 == 0) {
    DAT_00e20778 = 0;
    if (3 < (int)((&DAT_00ea5eb4)[DAT_00eaac28 * 0x1276] + (&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276]))
    {
      FUN_004d62f0(0x37,0x1a,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d62f0(0x3d,0x2b,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      return;
    }
    if (4 < *(int *)(&DAT_00ea3fac + iVar3)) {
      FUN_004d62f0(0x37,0x19,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d6860(*(undefined4 *)(&DAT_00ea3fac + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                   DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4
                  );
      FUN_004d62f0(0x37,0x12,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      return;
    }
    if ((*(int *)(&DAT_00ea3f98 + iVar3) != 0) || (*(int *)(&DAT_00ea18a8 + iVar3) < 1)) {
      FUN_004d62f0(0x37,0x19,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d62f0(0x37,0x14,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      return;
    }
    if (0x4f < *(int *)(&DAT_00ea3f9c + iVar3)) {
      FUN_004d62f0(0x37,0x19,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d6860(*(undefined4 *)(&DAT_00ea3fac + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                   DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4
                  );
      if (*(int *)(&DAT_00ea3fac + DAT_00eaac28 * 0x49d8) != 1) {
        FUN_004d62f0(0x37,0x12,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,
                     0,DAT_00e38da8,DAT_00e38da4);
        return;
      }
      FUN_004d62f0(0x37,0x13,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      return;
    }
    FUN_004d62f0(0x37,0x1a,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    DAT_00e20778 = 0;
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 0) {
      FUN_004d62f0(0x37,0x15,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 1) {
      FUN_004d62f0(0x37,0x16,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 2) {
      FUN_004d62f0(0x37,0x17,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 3) {
      FUN_004d62f0(0x37,0x18,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 4) {
      FUN_004d62f0(0x37,0x20,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 5) {
      FUN_004d62f0(0x37,0x21,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      return;
    }
  }
  else if (iVar1 == 1) {
    FUN_004d62f0(0x37,0x1a,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 0) {
      FUN_004d62f0(0x37,0x15,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 1) {
      FUN_004d62f0(0x37,0x16,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 2) {
      FUN_004d62f0(0x37,0x17,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 3) {
      FUN_004d62f0(0x37,0x18,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 4) {
      FUN_004d62f0(0x37,0x20,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((&DAT_00ea5c80)[DAT_00eaac28 * 0x1276] == 5) {
      FUN_004d62f0(0x37,0x21,DAT_00e20778 + 0x4b + DAT_00f53f0c,DAT_00f53f08 + 0x18c,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      return;
    }
  }
  else {
    if (iVar1 == 2) {
      FUN_004d6160(0x37,0x1e,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,0x1f0,DAT_00f56f9c,0,
                   DAT_00e38da8,DAT_00e38da4);
      return;
    }
    FUN_004d6160(0x37,0x1f,DAT_00f53f0c + 0x4b,DAT_00f53f08 + 0x17a,0x1f0,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
