#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005e03fc (*(undefined4 *)0x005e03fcu)
#define DAT_005f89d8 (*(undefined4 *)0x005f89d8u)
#define DAT_00784ddc (*(undefined *)0x00784ddcu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea1878 (*(undefined4 *)0x00ea1878u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea4488 (*(undefined4 *)0x00ea4488u)
#define DAT_00ea448c (*(undefined4 *)0x00ea448cu)
#define DAT_00ea44a8 (*(undefined4 *)0x00ea44a8u)
#define DAT_00ea44b4 (*(undefined4 *)0x00ea44b4u)
#define DAT_00ea44b8 (*(undefined4 *)0x00ea44b8u)
#define DAT_00ea44bc (*(undefined4 *)0x00ea44bcu)
#define DAT_00ea44c0 (*(undefined4 *)0x00ea44c0u)
#define DAT_00ea44d4 (*(undefined4 *)0x00ea44d4u)
#define DAT_00ea44d8 (*(undefined4 *)0x00ea44d8u)
#define DAT_00ea44dc (*(undefined4 *)0x00ea44dcu)
#define DAT_00ea44e0 (*(undefined4 *)0x00ea44e0u)
#define DAT_00ea44e8 (*(undefined4 *)0x00ea44e8u)
#define DAT_00ea44ec (*(undefined4 *)0x00ea44ecu)
#define DAT_00ea44f0 (*(undefined4 *)0x00ea44f0u)
#define DAT_00ea44f4 (*(undefined4 *)0x00ea44f4u)
#define DAT_00ea44f8 (*(undefined4 *)0x00ea44f8u)
#define DAT_00ea44fc (*(undefined4 *)0x00ea44fcu)
#define DAT_00ea4500 (*(undefined4 *)0x00ea4500u)
#define DAT_00ea4504 (*(undefined4 *)0x00ea4504u)
#define DAT_00ea4514 (*(undefined4 *)0x00ea4514u)
#define DAT_00ea4518 (*(undefined4 *)0x00ea4518u)
#define DAT_00ea451c (*(undefined4 *)0x00ea451cu)
#define DAT_00ea4520 (*(undefined4 *)0x00ea4520u)
#define DAT_00ea4524 (*(undefined4 *)0x00ea4524u)
#define DAT_00ea4528 (*(undefined4 *)0x00ea4528u)
#define DAT_00ea452c (*(undefined4 *)0x00ea452cu)
#define DAT_00ea4530 (*(undefined4 *)0x00ea4530u)
#define DAT_00ea5cc4 (*(undefined4 *)0x00ea5cc4u)
#define DAT_00ea5cc8 (*(undefined4 *)0x00ea5cc8u)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5d50 (*(undefined4 *)0x00ea5d50u)
#define DAT_00ea5d54 (*(undefined4 *)0x00ea5d54u)
#define DAT_00ea5e5c (*(undefined4 *)0x00ea5e5cu)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00ea5fc4 (*(undefined4 *)0x00ea5fc4u)
#define DAT_00ea5fc8 (*(undefined4 *)0x00ea5fc8u)
#define DAT_00ea601c (*(undefined4 *)0x00ea601cu)
#define DAT_00ea6020 (*(undefined4 *)0x00ea6020u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_004c5120();
extern void FUN_004ccc30();
extern void FUN_004cd510();
extern void FUN_004cd680();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern void FUN_00536820();
extern void FUN_00536ba0();
extern int FUN_0053e590();

void FUN_0052a250(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  
  uVar2 = DAT_00784ddc;
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) {
    iVar1 = FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]);
    if (iVar1 == 1) {
      uVar2 = ((int)(uVar2 - 5) < 0) - 1 & uVar2 - 5;
    }
  }
  iVar1 = 5 - DAT_00f53f48;
  DAT_00f53f0c = DAT_00e38da8;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1b;
  if (iVar1 < 5) {
    if (0 < iVar1) {
      FUN_004cd680(iVar1 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar1 * 8,(iVar1 * -2 + 0x28) * 0x10,
                   (iVar1 * -2 + 0x1b) * 0x10,0);
      return;
    }
    FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1b);
    FUN_0053e590(DAT_00f5681c + 10,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    FUN_004d62f0(0x3c,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d65b0(DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x30,0x22,4,DAT_00e38da8,DAT_00e38da4);
    FUN_00536ba0(DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x68,0x22);
    DAT_00e20778 = 0;
    if (*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) < 0) {
      FUN_004d62f0(0x3c,3,DAT_00f53f0c + 0x46,DAT_00f53f08 + 0x35,DAT_00f56fa4,0,DAT_00e38da8,
                   DAT_00e38da4);
      iVar1 = DAT_00e20778 + 0x48 + DAT_00f53f0c;
      iVar3 = -*(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8);
      uVar4 = DAT_00f56fa4;
    }
    else {
      FUN_004d62f0(0x3c,2,DAT_00f53f0c + 0x46,DAT_00f53f08 + 0x35,DAT_00f56f9c,0,DAT_00e38da8,
                   DAT_00e38da4);
      iVar1 = DAT_00e20778 + 0x48 + DAT_00f53f0c;
      iVar3 = *(int *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8);
      uVar4 = DAT_00f56f9c;
    }
    FUN_004ccc30(8,0,iVar3,iVar1,DAT_00f53f08 + 0x35,uVar4,0);
    FUN_004d62f0(0x3c,1,DAT_00f53f0c + 0x46,DAT_00f53f08 + 0x4c,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860(*(undefined4 *)(&DAT_00ea1878 + DAT_00eaac28 * 0x49d8),0x40,&DAT_005e03fc,
                 DAT_00f53f0c + 0xf0,DAT_00f53f08 + 0x4c,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,4,DAT_00e20778 + 0xf0 + DAT_00f53f0c,DAT_00f53f08 + 0x4c,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004ccc30(8,0,(&DAT_00ea5e5c)[DAT_00eaac28 * 0x1276],DAT_00e20778 + 0xf0 + DAT_00f53f0c,
                 DAT_00f53f08 + 0x4c,DAT_00f56f9c,0);
    DAT_00e20778 = 0;
    FUN_004d6860((&DAT_00ea44a8)[DAT_00eaac28 * 0x1276],0x40,&DAT_005e03fc,DAT_00f53f0c + 0x46,
                 DAT_00f53f08 + 0x62,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,5,DAT_00e20778 + 0x46 + DAT_00f53f0c,DAT_00f53f08 + 0x62,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d6860((&DAT_00ea44b8)[DAT_00eaac28 * 0x1276] + (&DAT_00ea44b4)[DAT_00eaac28 * 0x1276],
                 0x40,&DAT_005f89d8,DAT_00f53f0c + 0x16d,DAT_00f53f08 + 0x62,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x17,DAT_00e20778 + 0x16d + DAT_00f53f0c,DAT_00f53f08 + 0x62,DAT_00f56f9c,0,
                 DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,6,DAT_00f53f0c + 0x10e,DAT_00f53f08 + 0x7d,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x3c,7,DAT_00f53f0c + 400,DAT_00f53f08 + 0x7d,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d62f0(0x3c,8,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x8c,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44c0)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x8c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44bc)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x8c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,9,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x9b,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44d8)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x9b,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44d4)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x9b,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x14,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0xaa,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5cc4)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0xaa,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5cc8)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0xaa,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x18,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0xb9,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860(*(undefined4 *)(&DAT_00ea601c + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                 DAT_00f53f0c + 0x122,DAT_00f53f08 + 0xb9,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860(*(undefined4 *)(&DAT_00ea6020 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                 DAT_00f53f0c + 0x1ae,DAT_00f53f08 + 0xb9,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004cd510(DAT_00f53f0c + 0x118,DAT_00f53f08 + 0xc6,DAT_00f53f0c + 0x15e,DAT_00f53f08 + 0xc6,0
                );
    FUN_004cd510(DAT_00f53f0c + 0x1a4,DAT_00f53f08 + 0xc6,DAT_00f53f0c + 0x1ea,DAT_00f53f08 + 0xc6,0
                );
    FUN_004d62f0(0x3c,10,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0xcb,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4514)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0xcb,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4518)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0xcb,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0xb,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0xe3,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44e0)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0xe3,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44dc)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0xe3,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0xc,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0xf2,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea448c)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0xf2,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4488)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0xf2,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0xd,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x101,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44f8)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x101,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44fc)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x101,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x3c,0xe,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x110,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860(uVar2,0x40,&DAT_005e03fc,DAT_00e20778 + 0x50 + DAT_00f53f0c,DAT_00f53f08 + 0x110,
                 DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44e8)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x110,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44ec)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x110,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0xf,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x11f,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4500)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x11f,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4504)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x11f,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x10,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x12e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44f0)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x12e,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea44f4)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x12e,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x15,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x13d,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5d54)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x13d,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5d50)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x13d,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x16,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x14c,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860(*(undefined4 *)(&DAT_00ea5fc4 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                 DAT_00f53f0c + 0x122,DAT_00f53f08 + 0x14c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4)
    ;
    FUN_004d6860(*(undefined4 *)(&DAT_00ea5fc8 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                 DAT_00f53f0c + 0x1ae,DAT_00f53f08 + 0x14c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4)
    ;
    FUN_004cd510(DAT_00f53f0c + 0x118,DAT_00f53f08 + 0x159,DAT_00f53f0c + 0x15e,DAT_00f53f08 + 0x159
                 ,0);
    FUN_004cd510(DAT_00f53f0c + 0x1a4,DAT_00f53f08 + 0x159,DAT_00f53f0c + 0x1ea,DAT_00f53f08 + 0x159
                 ,0);
    FUN_004d62f0(0x3c,0x11,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x15e,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea451c)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x15e,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4520)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x15e,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x12,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x175,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4524)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x175,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4528)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x175,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d62f0(0x3c,0x13,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x18c,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860((&DAT_00ea452c)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x122,
                 DAT_00f53f08 + 0x18c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea4530)[DAT_00eaac28 * 0x1276],0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,
                 DAT_00f53f08 + 0x18c,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
