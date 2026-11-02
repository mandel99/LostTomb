#include "restore_types.h"
#define DAT_005d2a3c (*(undefined4 *)0x005d2a3cu)
#define DAT_00605a44 (*(undefined *)0x00605a44u)
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_0077f761 (*(undefined4 *)0x0077f761u)
#define DAT_00784f14 (*(undefined *)0x00784f14u)
#define DAT_00d3a6c0 (*(undefined *)0x00d3a6c0u)
#define DAT_00d3a6cc (*(undefined *)0x00d3a6ccu)
#define DAT_00d3a6d0 (*(undefined *)0x00d3a6d0u)
#define DAT_00d3a6d4 (*(undefined *)0x00d3a6d4u)
#define DAT_00d3a6d8 (*(undefined *)0x00d3a6d8u)
#define DAT_00d3a6e4 (*(undefined *)0x00d3a6e4u)
#define DAT_00d3b130 (*(undefined *)0x00d3b130u)
#define DAT_00d3b138 (*(undefined *)0x00d3b138u)
#define DAT_00d3b13c (*(undefined *)0x00d3b13cu)
#define DAT_00d3b164 (*(undefined *)0x00d3b164u)
#define DAT_00d3b168 (*(undefined *)0x00d3b168u)
#define DAT_00d3b51c (*(undefined *)0x00d3b51cu)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e28 (*(undefined *)0x00e38e28u)
#define DAT_00e38e2c (*(undefined *)0x00e38e2cu)
#define DAT_00e92dfc (*(undefined *)0x00e92dfcu)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
extern void FUN_004c8070();
extern int FUN_004c92a0();
extern int FUN_004c9340();
extern int FUN_004c95d0();
extern void FUN_004ca9a0();
extern void FUN_004caf10();
extern void FUN_004cb0b0();
extern void FUN_004cb300();
extern void FUN_004cc110();
extern void FUN_004cd680();
extern void FUN_004cd9d0();

void FUN_004cabd0(void)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  uint3 in_ECX;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uStack_4;
  
  iVar2 = DAT_0077591c;
  uStack_4 = (uint)in_ECX;
  if (DAT_00d3a6c0 == 1) {
    FUN_004caf10();
    return;
  }
  if (DAT_00d3a6c0 == 2) {
    FUN_004cb0b0();
    return;
  }
  if (DAT_00d3a6c0 == 3) {
    FUN_004cb300();
    return;
  }
  if (DAT_00d3a6c0 == 4) {
    FUN_004ca9a0();
    return;
  }
  DAT_00d3b168 = DAT_00f56f84;
  DAT_00d3b164 = DAT_00f56f84;
  DAT_00d3a6d0 = 200;
  DAT_00d3b130 = 0;
  DAT_00d3b13c = 0x1e;
  DAT_00d3b51c = 0x1c82;
  if (DAT_00e92dfc == 2) {
    DAT_00d3b51c = 0x3922;
  }
  iVar4 = 0;
  if (DAT_00e38dda == '\x14') {
    iVar4 = DAT_00784f14 + -1;
    iVar3 = FUN_004c9340((int)&uStack_4 + 3);
LAB_004cac9f: ;
    if (iVar3 == 0) goto LAB_004cacd8;
    if (iVar3 == 0x13) {
      if (RESTORE_SLICE1(uStack_4,3) == '\0') {
        uVar6 = 0x17;
        iVar3 = *(int *)(&DAT_005d2a3c + (uint)(byte)(&DAT_0077f761)[iVar4 * 0x62] * 8);
      }
      else {
        uVar6 = 0x17;
        iVar3 = 0x26;
      }
    }
    else {
      uVar6 = 0x17;
    }
  }
  else {
    if (DAT_00e38dda == '\x15') {
      iVar3 = FUN_004c95d0((int)&uStack_4 + 3);
      iVar4 = iVar2;
      goto LAB_004cac9f;
    }
    if (DAT_00e38dda == 'N') {
      uStack_4 = CONCAT13(1,in_ECX);
      iVar3 = FUN_004c92a0();
      goto LAB_004cac9f;
    }
LAB_004cacd8: ;
    uVar6 = 0x44;
    iVar3 = DAT_00d3a6e4;
  }
  FUN_004cc110(uVar6,iVar3);
  FUN_004c8070(DAT_00e20768,DAT_00d3a6d8 + 5,DAT_00d3a6d4 + 5,DAT_00d3a6d0 + -0x14,1,0);
  DAT_00d3a6cc = DAT_00d3b138 * 0x10 + 10;
  if (2 < DAT_00d3b138) {
    DAT_00d3a6d0 = 300;
    DAT_00d3b130 = 0;
    DAT_00d3b13c = 0x1e;
    FUN_004cc110(0x44,DAT_00d3a6e4);
    FUN_004c8070(DAT_00e20768,DAT_00d3a6d8 + 5,DAT_00d3a6d4 + 5,DAT_00d3a6d0 + -0x14,1,0);
    DAT_00d3a6cc = DAT_00d3b138 * 0x10 + 10;
  }
  bVar1 = DAT_00e38da4 + 200 <= DAT_00e38e28;
  if (DAT_00e38e2c < DAT_00e38da8 + 100 + DAT_00d3a6d0) {
    if (DAT_00e38dda != '\a') {
      DAT_00d3a6d8 = DAT_00e38e2c + 0x14;
      goto LAB_004cadf3;
    }
    DAT_00d3a6d8 = DAT_00e38e2c + 0x32;
  }
  else {
    DAT_00d3a6d8 = (DAT_00e38e2c - DAT_00d3a6d0) + -0x14;
LAB_004cadf3: ;
    if (DAT_00e38dda != '\a') {
      if (DAT_00e38dda == '-') {
        DAT_00d3a6d4 = DAT_00e38e28 + -0x34;
      }
      else if (DAT_00e38dda == '*') {
        DAT_00d3a6d4 = DAT_00e38e28 + 0x28;
        if (bVar1) {
          DAT_00d3a6d4 = DAT_00e38e28 + -0x48;
        }
        DAT_00d3a6d8 = (DAT_00e38e2c - DAT_00d3a6d0 / 2) + -10;
      }
      else {
        DAT_00d3a6d4 = DAT_00e38e28 + 0x28;
        if (bVar1) {
          DAT_00d3a6d4 = DAT_00e38e28 + -0x3e;
        }
      }
      goto LAB_004cae7a;
    }
  }
  DAT_00d3a6d4 = DAT_00e38da4 + 0x1b0;
  if (DAT_00e38e28 < DAT_00d3a6d4) {
    if (DAT_00605a44 == 1) {
      DAT_00d3a6d4 = DAT_00e38e28 + -0x4a;
    }
    else if (DAT_00605a44 == 6) {
      DAT_00d3a6d4 = DAT_00e38e28 + -0x36;
    }
    else if (DAT_00605a44 == 7) {
      DAT_00d3a6d4 = DAT_00e38e28 + -0x3a;
    }
    else {
      DAT_00d3a6d4 = DAT_00e38e28 + -0x40;
    }
  }
LAB_004cae7a: ;
  if (DAT_00e92dfc == 2) {
    uVar5 = 0xbb88;
    uVar6 = 0x58a0;
  }
  else {
    uVar5 = 0x5dc8;
    uVar6 = 0x2c40;
  }
  FUN_004cd680(DAT_00d3a6d8,DAT_00d3a6d4,DAT_00d3a6d0 + 0x14,DAT_00d3a6cc,uVar6);
  FUN_004cd9d0(DAT_00d3a6d8 + 1,DAT_00d3a6d4 + 1,DAT_00d3a6d0 + 0x12,DAT_00d3a6cc + -2,uVar5);
  FUN_004c8070(DAT_00e20768,DAT_00d3a6d8 + 5,DAT_00d3a6d4 + 7,DAT_00d3a6d0 + -0x14,0,0);
  return;
}
