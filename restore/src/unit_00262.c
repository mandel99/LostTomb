#include "restore_types.h"
#define DAT_0057be78 (*(undefined4 *)0x0057be78u)
#define DAT_006fdc98 (*(int32_t *)0x006fdc98u)
#define DAT_007027ac (*(int32_t *)0x007027acu)
#define DAT_007027b0 (*(int32_t *)0x007027b0u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00784f0c (*(int32_t *)0x00784f0cu)
#define DAT_00d3b568 (*(int32_t *)0x00d3b568u)
#define DAT_00d3b56c (*(int32_t *)0x00d3b56cu)
#define DAT_00d3b570 (*(int32_t *)0x00d3b570u)
#define DAT_00d3b574 (*(int32_t *)0x00d3b574u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00e38db0 (*(int32_t *)0x00e38db0u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00f55c94 (*(int32_t *)0x00f55c94u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56f94 (*(int32_t *)0x00f56f94u)
#define DAT_00f56fa4 (*(int32_t *)0x00f56fa4u)
#define DAT_00f56fb4 (*(int32_t *)0x00f56fb4u)
#define DAT_00f56fbc (*(int32_t *)0x00f56fbcu)
#define DAT_00f57314 (*(int32_t *)0x00f57314u)
extern undefined4 FUN_0041ba00();
extern uint FUN_0041bad0();
extern void FUN_00448370();
extern void FUN_004cc110();
extern void FUN_004cc720();
extern void FUN_004ccb30();
extern void FUN_004ccbb0();
extern void FUN_004ccfd0();
extern void FUN_004cd1a0();
extern void FUN_004cd240();
extern void FUN_00536710();
extern int FUN_0053e590();

void FUN_0041be10(void)

{
  bool bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int iVar6;
  bool bVar7;
  bool bVar8;
  undefined4 uVar9;
  undefined4 local_dc;
  int local_d4 [3];
  undefined1 local_c8 [200];
  
  DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
  DAT_00d3b574 = DAT_00e38db0;
  DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
  DAT_00d3b570 = DAT_00e38dac;
  DAT_00e38dc9 = 1;
  if (DAT_006fdc98 == 0) {
    iVar3 = DAT_00e38da4 + 0x3c;
  }
  else {
    iVar3 = DAT_00e38da4 + 0x38;
  }
  if (DAT_007027ac < 5) {
    iVar4 = DAT_00f55c94 + DAT_007027ac;
  }
  else if (DAT_007027ac == 5) {
    iVar4 = DAT_00f57314 + 4;
  }
  else if (DAT_007027ac == 6) {
    iVar4 = DAT_00f57314 + 2;
  }
  else if (DAT_007027ac == 7) {
    iVar4 = DAT_00f57314 + 5;
  }
  else {
    iVar4 = DAT_00f57314 + 7;
  }
  FUN_0053e590(iVar4,DAT_00e38da8 + 0x4e,iVar3);
  if (DAT_006fdc98 == 0) {
    uVar9 = 5;
  }
  else {
    uVar9 = 6;
  }
  FUN_004cc110(0x125,uVar9);
  uVar9 = DAT_00784f0c;
  DAT_00784f0c = 0;
  FUN_00448370(DAT_00e20768,local_c8,200);
  DAT_00784f0c = uVar9;
  FUN_004cc720(local_c8,DAT_00e38da8 + 0x14,DAT_00e38da4 + 0x11,600,DAT_00f56fb4,0);
  cVar2 = FUN_0041ba00(DAT_007027ac);
  if (cVar2 != '\0') {
    FUN_004ccb30(0x126,DAT_007027ac * 4,DAT_00e38da8 + 0x153,DAT_00e38da4 + 0x3c,0x11b,DAT_00f56fbc,
                 0);
    if (DAT_006fdc98 == 0) {
      if (DAT_007027ac < DAT_007027b0) {
        iVar3 = 1;
      }
      else {
        iVar3 = (DAT_007027ac != DAT_007027b0) + 2;
      }
    }
    else {
      cVar2 = FUN_0041bad0(DAT_007027ac);
      iVar3 = (cVar2 != '\0') + 2;
    }
    FUN_004ccfd0(0x126,iVar3 + DAT_007027ac * 4,DAT_00e38da8 + 0x15b,DAT_00e38da4 + 0x74,0x10d,
                 DAT_00f56f94,0);
  }
  bVar1 = false;
  psVar5 = (short *)0x0057be78u;
  iVar3 = 0;
  do {
    local_d4[2] = *psVar5 + DAT_00e38da8;
    iVar4 = (int)psVar5[2] - (int)*psVar5;
    iVar6 = psVar5[1] + DAT_00e38da4;
    local_d4[0] = iVar3 + 1;
    bVar7 = DAT_007056c8 == local_d4[0];
    uVar9 = *(undefined4 *)(psVar5 + 10);
    if (DAT_006fdc98 == 0) {
      bVar8 = iVar3 != DAT_007027b0;
    }
    else {
      cVar2 = FUN_0041ba00(uVar9);
      if ((cVar2 == '\0') || (cVar2 = FUN_0041bad0(uVar9), cVar2 == '\0')) {
        bVar8 = false;
      }
      else {
        bVar8 = true;
      }
    }
    if (iVar3 == DAT_007027ac) {
      bVar1 = !bVar8;
    }
    local_dc = DAT_00f56f8c;
    if (bVar8) {
      local_dc = DAT_00f56fa4;
    }
    FUN_00536710(6,local_d4[2],iVar6,(int)(iVar4 + (iVar4 >> 0x1f & 0xfU)) >> 4,!bVar8 && bVar7);
    FUN_004cd240(0x1b,iVar3,psVar5,local_dc,0,0,6);
    psVar5 = psVar5 + 0xe;
    iVar3 = local_d4[0];
  } while ((int)psVar5 < 0x57bf74);
  if ((DAT_006fdc98 == 1) || (DAT_006fdc98 == 0)) {
    FUN_004ccfd0(0x126,0x29,DAT_00e38da8 + 0x1e,DAT_00e38da4 + 0x10a,0x90,DAT_00f56f8c,0);
    FUN_004ccfd0(0x126,0x2a,DAT_00e38da8 + 0x1e,DAT_00e38da4 + 0x16e,0x90,DAT_00f56f8c,0);
  }
  if (DAT_006fdc98 == 1) {
    local_d4[0] = 0x26;
    local_d4[1] = 0x27;
    iVar3 = 0;
    do {
      FUN_004cd1a0(0,(short *)(0x0057be78u+(iVar3+9)*28u),iVar3 == 1 || DAT_007056c8 == iVar3 + 10,0,0)
      ;
      FUN_004cd240(0x126,local_d4[iVar3],(short *)(0x0057be78u+(iVar3+9)*28u),DAT_00f56f8c,0,4,7);
      iVar3 = iVar3 + 1;
    } while (iVar3 < 2);
  }
  if (bVar1) {
    if (DAT_006fdc98 == 0) {
      FUN_004ccbb0(0x126,0x24,DAT_00e38da8 + 0x1a4,DAT_00e38da4 + 0x1bd,DAT_00f56f8c,0);
      DAT_00e38dca = 1;
      return;
    }
    FUN_004ccbb0(0x126,0x25,DAT_00e38da8 + 0x226,DAT_00e38da4 + 0x1bd,DAT_00f56f8c,0);
    FUN_004ccbb0(0x2c,0xd9,DAT_00e38da8 + 0x17c,DAT_00e38da4 + 0x1bd,DAT_00f56f8c,0);
  }
  DAT_00e38dca = 1;
  return;
}
