#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005f76c0 (*(undefined4 *)0x005f76c0u)
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00784b60 (*(undefined *)0x00784b60u)
#define DAT_00784c24 (*(undefined *)0x00784c24u)
#define DAT_00784c28 (*(undefined *)0x00784c28u)
#define DAT_00784c2c (*(undefined *)0x00784c2cu)
#define DAT_00784c30 (*(undefined *)0x00784c30u)
#define DAT_00784c34 (*(undefined *)0x00784c34u)
#define DAT_00784c38 (*(undefined *)0x00784c38u)
#define DAT_00784c3c (*(undefined *)0x00784c3cu)
#define DAT_00784c3d (*(char *)0x00784c3du)
#define DAT_00784c3e (*(undefined1 *)0x00784c3eu)
#define DAT_00784c3f (*(char *)0x00784c3fu)
#define DAT_00784c58 (*(undefined *)0x00784c58u)
#define DAT_00784c5c (*(undefined *)0x00784c5cu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5b3c (*(undefined4 *)0x00ea5b3cu)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5b44 (*(undefined4 *)0x00ea5b44u)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea5b54 (*(undefined4 *)0x00ea5b54u)
#define DAT_00ea5ca0 (*(undefined4 *)0x00ea5ca0u)
#define DAT_00ea5ca4 (*(undefined4 *)0x00ea5ca4u)
#define DAT_00ea5ca8 (*(undefined4 *)0x00ea5ca8u)
#define DAT_00ea5cac (*(undefined4 *)0x00ea5cacu)
#define DAT_00ea5cb0 (*(undefined4 *)0x00ea5cb0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadb28 (*(undefined4 *)0x00eadb28u)
#define DAT_00f53f00 (*(undefined *)0x00f53f00u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f53f48 (*(undefined *)0x00f53f48u)
#define DAT_00f55bc4 (*(int *)0x00f55bc4u)
#define DAT_00f5681c (*(undefined *)0x00f5681cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f04 (*(undefined4 *)0x00f53f04u)
extern undefined4 FUN_004381a0();
extern void FUN_004cce90();
extern void FUN_004cd680();
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d65b0();
extern void FUN_004d6860();
extern uint FUN_004ea930();
extern void FUN_004f7720();
extern void FUN_004f77e0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_004f6af0(void)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  undefined4 uVar10;
  int iVar11;
  int local_8;
  
  DAT_00f53f0c = DAT_00e38da8;
  iVar3 = 5 - DAT_00f53f48;
  DAT_00f53f08 = DAT_00e38da4;
  _DAT_00f53f04 = 0x28;
  DAT_00f53f00 = 0x1b;
  if (iVar3 < 5) {
    if (0 < iVar3) {
      FUN_004cd680(iVar3 * 0x10 + DAT_00e38da8,DAT_00e38da4 + iVar3 * 8,(iVar3 * -2 + 0x28) * 0x10,
                   (iVar3 * -2 + 0x1b) * 0x10,0);
      return;
    }
    FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x28,0x1b);
    FUN_0053e590(DAT_00f5681c + 3,DAT_00f53f0c + 10,DAT_00f53f08 + 10);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x35,0,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0xc,DAT_00f56fb4,0,DAT_00e38da8,
                 DAT_00e38da4);
    DAT_00e20778 = 0;
    FUN_004d62f0(0x35,6,DAT_00f53f0c + 0x118,DAT_00f53f08 + 0x11,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6860(*(undefined4 *)(&DAT_00ea1894 + DAT_00eaac28 * 0x49d8),0x40,&DAT_0057c048,
                 DAT_00e20778 + 0x118 + DAT_00f53f0c,DAT_00f53f08 + 0x11,DAT_00f56f8c,0,DAT_00e38da8
                 ,DAT_00e38da4);
    if ((DAT_00784c58 != 0) && (DAT_00784b60 == 0)) {
      DAT_00e20778 = 0;
      FUN_004d6860(DAT_00784c5c,0x40,&DAT_0057c048,DAT_00f53f0c + 0x118,DAT_00f53f08 + 0x22,
                   DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x35,5,DAT_00e20778 + 0x118 + DAT_00f53f0c,DAT_00f53f08 + 0x22,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    if ((DAT_00784c34 != 0) && (DAT_00784b60 == 0)) {
      DAT_00e20778 = 0;
      FUN_004d62f0(0x1d,DAT_00784c38 + 0x28,DAT_00f53f0c + 0x1ae,DAT_00f53f08 + 0x11,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
      FUN_004d6860(DAT_00eadb28,0x40,&DAT_0057c048,DAT_00e20778 + 0x1ae + DAT_00f53f0c,
                   DAT_00f53f08 + 0x11,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      DAT_00e20778 = 0;
      FUN_004d6860(DAT_00784c34,0x40,&DAT_0057c048,DAT_00f53f0c + 0x1ae,DAT_00f53f08 + 0x22,
                   DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
      FUN_004d62f0(0x35,5,DAT_00e20778 + 0x1ae + DAT_00f53f0c,DAT_00f53f08 + 0x22,DAT_00f56f8c,0,
                   DAT_00e38da8,DAT_00e38da4);
    }
    FUN_0053e590(DAT_00f55bc4,DAT_00f53f0c + 0x3c,DAT_00f53f08 + 0x30);
    iVar11 = DAT_00f53f0c;
    iVar7 = DAT_00f53f08;
    iVar5 = (&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276];
    iVar6 = DAT_00f53f08 + 0x126;
    iVar3 = DAT_00f53f0c + 0x50;
    FUN_004d63d0(0,DAT_00f53f0c + 0x50,DAT_00f53f08 + 0x11e,0x78,0x42,DAT_007056c8 == 1,DAT_00e38da8
                 ,DAT_00e38da4);
    FUN_004d6250(0x35,1,iVar3,iVar6,0x78,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004cce90(iVar5,0x40,&DAT_005f76c0,iVar3,iVar7 + 0x135,0x6e,DAT_00f56fb4,0);
    DAT_00e20778 = 0;
    iVar6 = DAT_00e38da8;
    iVar9 = DAT_00e38da4;
    if (DAT_00784c3c == '\0') {
      uVar10 = 0;
      iVar8 = 0;
      uVar4 = DAT_00f56f8c;
    }
    else {
      uVar10 = 0;
      uVar4 = FUN_004381a0(0x27b);
      iVar8 = DAT_00784c24;
    }
    iVar7 = iVar7 + 0x14e;
    FUN_004d6860(iVar8,0x40,&DAT_007027d4,iVar11 + 0x55,iVar7,uVar4,uVar10,iVar6,iVar9);
    uVar10 = 0;
    iVar6 = DAT_00e38da8;
    iVar11 = DAT_00e38da4;
    uVar4 = FUN_004381a0(0x27b);
    FUN_004d62f0(0x35,5,DAT_00e20778 + 5 + iVar3,iVar7,uVar4,uVar10,iVar6,iVar11);
    if ((DAT_00784c3c == '\0') || (DAT_00784c24 <= iVar5)) {
      uVar4 = 1;
    }
    else {
      uVar4 = 0;
    }
    if (iVar5 < 0x65) {
      if (iVar5 < 0) {
        iVar5 = 0;
      }
    }
    else {
      iVar5 = 100;
    }
    FUN_004f77e0(DAT_00f53f0c + 0x6e,DAT_00f53f08 + 0x112,iVar5,uVar4);
    iVar11 = DAT_00f53f0c;
    iVar7 = DAT_00f53f08;
    iVar5 = (&DAT_00ea5b40)[DAT_00eaac28 * 0x1276];
    iVar3 = DAT_00f53f08 + 0x126;
    iVar6 = DAT_00f53f0c + 200;
    FUN_004d63d0(0,DAT_00f53f0c + 200,DAT_00f53f08 + 0x11e,0x78,0x42,DAT_007056c8 == 2,DAT_00e38da8,
                 DAT_00e38da4);
    FUN_004d6250(0x35,2,iVar6,iVar3,0x78,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    uVar10 = 0;
    uVar4 = FUN_004381a0(0x280);
    FUN_004cce90(iVar5,0x40,&DAT_005f76c0,iVar6,iVar7 + 0x135,0x6e,uVar4,uVar10);
    DAT_00e20778 = 0;
    iVar3 = DAT_00e38da8;
    iVar6 = DAT_00e38da4;
    if (DAT_00784c3d == '\0') {
      uVar10 = 0;
      uVar4 = FUN_004381a0(0x27b);
      iVar9 = 0;
    }
    else {
      uVar10 = 0;
      uVar4 = FUN_004381a0(0x27b);
      iVar9 = DAT_00784c28;
    }
    iVar7 = iVar7 + 0x14e;
    FUN_004d6860(iVar9,0x40,&DAT_007027d4,iVar11 + 0xcd,iVar7,uVar4,uVar10,iVar3,iVar6);
    uVar10 = 0;
    iVar3 = DAT_00e38da8;
    iVar6 = DAT_00e38da4;
    uVar4 = FUN_004381a0(0x27b);
    FUN_004d62f0(0x35,5,iVar11 + 0xcd + DAT_00e20778,iVar7,uVar4,uVar10,iVar3,iVar6);
    if ((DAT_00784c3d == '\0') || (DAT_00784c28 <= iVar5)) {
      uVar4 = 1;
    }
    else {
      uVar4 = 0;
    }
    if (iVar5 < 0x65) {
      if (iVar5 < 0) {
        iVar5 = 0;
      }
    }
    else {
      iVar5 = 100;
    }
    FUN_004f77e0(DAT_00f53f0c + 0xe6,DAT_00f53f08 + 0x112,iVar5,uVar4);
    iVar11 = DAT_00f53f0c;
    iVar7 = DAT_00f53f08;
    iVar5 = (&DAT_00ea5b44)[DAT_00eaac28 * 0x1276];
    iVar3 = DAT_00f53f08 + 0x126;
    iVar6 = DAT_00f53f0c + 0x140;
    FUN_004d63d0(0,DAT_00f53f0c + 0x140,DAT_00f53f08 + 0x11e,0x78,0x42,DAT_007056c8 == 3,
                 DAT_00e38da8,DAT_00e38da4);
    uVar10 = 0;
    iVar9 = DAT_00e38da8;
    iVar8 = DAT_00e38da4;
    uVar4 = FUN_004381a0(0x27b);
    FUN_004d6250(0x35,3,iVar6,iVar3,0x78,uVar4,uVar10,iVar9,iVar8);
    uVar10 = 0;
    uVar4 = FUN_004381a0(0x280);
    FUN_004cce90(iVar5,0x40,&DAT_005f76c0,iVar6,iVar7 + 0x135,0x6e,uVar4,uVar10);
    DAT_00e20778 = 0;
    iVar3 = DAT_00e38da8;
    iVar6 = DAT_00e38da4;
    if (DAT_00784c3e == '\0') {
      uVar10 = 0;
      uVar4 = FUN_004381a0(0x27b);
      iVar9 = 0;
    }
    else {
      uVar10 = 0;
      uVar4 = FUN_004381a0(0x27b);
      iVar9 = DAT_00784c2c;
    }
    iVar7 = iVar7 + 0x14e;
    FUN_004d6860(iVar9,0x40,&DAT_007027d4,iVar11 + 0x145,iVar7,uVar4,uVar10,iVar3,iVar6);
    uVar10 = 0;
    iVar3 = DAT_00e38da8;
    iVar6 = DAT_00e38da4;
    uVar4 = FUN_004381a0(0x27b);
    FUN_004d62f0(0x35,5,iVar11 + 0x145 + DAT_00e20778,iVar7,uVar4,uVar10,iVar3,iVar6);
    if ((DAT_00784c3e == '\0') || (DAT_00784c2c <= iVar5)) {
      uVar4 = 1;
    }
    else {
      uVar4 = 0;
    }
    cVar2 = FUN_004ea930();
    if (cVar2 == '\0') {
      uVar4 = 0;
    }
    if (iVar5 < 0x65) {
      if (iVar5 < 0) {
        iVar5 = 0;
      }
    }
    else {
      iVar5 = 100;
    }
    FUN_004f77e0(DAT_00f53f0c + 0x15e,DAT_00f53f08 + 0x112,iVar5,uVar4);
    iVar11 = DAT_00f53f0c;
    iVar7 = DAT_00f53f08;
    iVar5 = (&DAT_00ea5b48)[DAT_00eaac28 * 0x1276];
    iVar3 = DAT_00f53f0c + 0x1b8;
    iVar6 = DAT_00f53f08 + 0x126;
    FUN_004f7720(4);
    uVar10 = 0;
    iVar9 = DAT_00e38da8;
    iVar8 = DAT_00e38da4;
    uVar4 = FUN_004381a0(0x27b);
    FUN_004d6250(0x35,4,iVar3,iVar6,0x78,uVar4,uVar10,iVar9,iVar8);
    uVar10 = 0;
    uVar4 = FUN_004381a0(0x280);
    FUN_004cce90(iVar5,0x40,&DAT_005f76c0,iVar3,iVar7 + 0x135,0x6e,uVar4,uVar10);
    DAT_00e20778 = 0;
    iVar3 = DAT_00e38da8;
    iVar6 = DAT_00e38da4;
    if (DAT_00784c3f == '\0') {
      uVar10 = 0;
      uVar4 = FUN_004381a0(0x27b);
      iVar9 = 0;
    }
    else {
      uVar10 = 0;
      uVar4 = FUN_004381a0(0x27b);
      iVar9 = DAT_00784c30;
    }
    iVar7 = iVar7 + 0x14e;
    FUN_004d6860(iVar9,0x40,&DAT_007027d4,iVar11 + 0x1bd,iVar7,uVar4,uVar10,iVar3,iVar6);
    uVar10 = 0;
    iVar3 = DAT_00e38da8;
    iVar6 = DAT_00e38da4;
    uVar4 = FUN_004381a0(0x27b);
    FUN_004d62f0(0x35,5,iVar11 + 0x1bd + DAT_00e20778,iVar7,uVar4,uVar10,iVar3,iVar6);
    if ((DAT_00784c3f == '\0') || (DAT_00784c30 <= iVar5)) {
      uVar4 = 1;
    }
    else {
      uVar4 = 0;
    }
    if (iVar5 < 0x65) {
      if (iVar5 < 0) {
        iVar5 = 0;
      }
    }
    else {
      iVar5 = 100;
    }
    FUN_004f77e0(DAT_00f53f0c + 0x1d6,DAT_00f53f08 + 0x112,iVar5,uVar4);
    FUN_004d65b0(DAT_00f53f0c + 0x40,DAT_00f53f08 + 0x164,0x20,4,DAT_00e38da8,DAT_00e38da4);
    iVar6 = DAT_00f53f0c + 0x48;
    local_8 = DAT_00f53f08 + 0x176;
    iVar3 = -1;
    iVar5 = -1;
    iVar7 = -1;
    bVar1 = false;
    switch((&DAT_00ea5ca0)[DAT_00eaac28 * 0x1276]) {
    case 0: ;
      local_8 = DAT_00f53f08 + 0x17c;
      iVar3 = 8;
      bVar1 = true;
      break;
    case 1: ;
      iVar5 = 1;
      if ((int)(&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276] < 0x5b) {
        iVar3 = (&DAT_00ea5ca4)[DAT_00eaac28 * 0x1276] + 9;
      }
      else {
        iVar3 = 0x41;
      }
      break;
    case 2: ;
      iVar5 = 2;
      if ((int)(&DAT_00ea5b40)[DAT_00eaac28 * 0x1276] < 0x5b) {
        iVar7 = (&DAT_00ea5b54)[DAT_00eaac28 * 0x1276] + 0x14;
        if ((&DAT_00ea5ca8)[DAT_00eaac28 * 0x1276] != 0) {
          iVar3 = (&DAT_00ea5ca8)[DAT_00eaac28 * 0x1276] + 0x14;
        }
      }
      else {
        iVar3 = 0x42;
      }
      break;
    case 3: ;
      iVar5 = 3;
      if ((int)(&DAT_00ea5b44)[DAT_00eaac28 * 0x1276] < 0x5b) {
        iVar3 = (&DAT_00ea5cac)[DAT_00eaac28 * 0x1276] + 0x37;
      }
      else {
        iVar3 = 0x43;
      }
      break;
    case 4: ;
      iVar5 = 4;
      if ((int)(&DAT_00ea5b48)[DAT_00eaac28 * 0x1276] < 0x5b) {
        iVar3 = (&DAT_00ea5cb0)[DAT_00eaac28 * 0x1276] + 0x20;
      }
      else {
        iVar3 = 0x44;
      }
    }
    DAT_00e20778 = 0;
    if (-1 < iVar5) {
      FUN_004d62f0(0x35,iVar5,iVar6,DAT_00f53f08 + 0x167,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
    }
    if (-1 < iVar7) {
      uVar10 = 0;
      iVar5 = DAT_00e38da8;
      iVar11 = DAT_00e38da4;
      uVar4 = FUN_004381a0(0x27d);
      FUN_004d62f0(0x35,iVar7,DAT_00e20778 + 0x48 + DAT_00f53f0c,DAT_00f53f08 + 0x167,uVar4,uVar10,
                   iVar5,iVar11);
    }
    DAT_00e20778 = 0;
    if (-1 < iVar3) {
      uVar4 = 0;
      if (bVar1) {
        iVar5 = DAT_00e38da8;
        iVar7 = DAT_00e38da4;
        uVar10 = FUN_004381a0(0x27d);
        FUN_004d6250(0x35,iVar3,iVar6,local_8,0x1f0,uVar10,uVar4,iVar5,iVar7);
        return;
      }
      uVar4 = FUN_004381a0(0x27d);
      FUN_004d6160(0x35,iVar3,iVar6,local_8,0x1f0,uVar4);
    }
  }
  return;
}
