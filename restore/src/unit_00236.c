#include "restore_types.h"
#define DAT_005797d4 (*(undefined *)0x005797d4u)
#define DAT_005797d8 (*(undefined *)0x005797d8u)
#define DAT_005797f4 (*(undefined *)0x005797f4u)
#define DAT_005797f8 (*(undefined *)0x005797f8u)
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005c7e10 (*(undefined1 *)0x005c7e10u)
#define DAT_005c7e38 (*(undefined1 *)0x005c7e38u)
#define DAT_006fdc9c (*(undefined *)0x006fdc9cu)
#define DAT_006fdca0 (*(undefined *)0x006fdca0u)
#define DAT_007027a8 (*(undefined *)0x007027a8u)
#define DAT_00784c24 (*(undefined *)0x00784c24u)
#define DAT_00784c30 (*(undefined *)0x00784c30u)
#define DAT_00784c34 (*(undefined *)0x00784c34u)
#define DAT_00784c38 (*(undefined *)0x00784c38u)
#define DAT_00784c58 (*(undefined *)0x00784c58u)
#define DAT_00784c5c (*(undefined *)0x00784c5cu)
#define DAT_00d3b124 (*(undefined *)0x00d3b124u)
#define DAT_00d3b134 (*(undefined *)0x00d3b134u)
#define DAT_00d3b138 (*(undefined *)0x00d3b138u)
#define DAT_00d3b13c (*(undefined *)0x00d3b13cu)
#define DAT_00d3b148 (*(undefined *)0x00d3b148u)
#define DAT_00d3b14c (*(undefined *)0x00d3b14cu)
#define DAT_00d3b150 (*(undefined *)0x00d3b150u)
#define DAT_00d3b154 (*(undefined *)0x00d3b154u)
#define DAT_00d3b164 (*(undefined *)0x00d3b164u)
#define DAT_00d3b168 (*(undefined *)0x00d3b168u)
#define DAT_00d3b550 (*(undefined *)0x00d3b550u)
#define DAT_00d3b554 (*(undefined *)0x00d3b554u)
#define DAT_00d3b558 (*(undefined *)0x00d3b558u)
#define DAT_00d3b55c (*(undefined *)0x00d3b55cu)
#define DAT_00d3b568 (*(undefined *)0x00d3b568u)
#define DAT_00d3b56c (*(undefined *)0x00d3b56cu)
#define DAT_00d3b570 (*(undefined *)0x00d3b570u)
#define DAT_00d3b574 (*(undefined *)0x00d3b574u)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e209fc (*(undefined4 *)0x00e209fcu)
#define DAT_00e20a00 (*(undefined4 *)0x00e20a00u)
#define DAT_00e20a04 (*(undefined4 *)0x00e20a04u)
#define DAT_00e36c74 (*(uint32_t *)0x00e36c74u)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00f55c44 (*(undefined *)0x00f55c44u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
#define _DAT_00f53f28 (*(undefined4 *)0x00f53f28u)
extern int FUN_00418140();
extern uint8_t FUN_0041b8f0(uint32_t self);
extern void FUN_004c8070();
extern void FUN_004cc770();
extern void FUN_004ccbb0();
extern void FUN_004cce00();
extern undefined4 FUN_004e2530();
extern void FUN_00536820();
extern void FUN_00536a30();
extern void FUN_00537000();
extern int FUN_0053e590();
extern void FUN_005405b0();

void FUN_0041a180(void)

{
  int iVar1;
  int iVar2;
  char cVar3;
  undefined4 *puVar4;
  int *piVar5;
  uint uVar6;
  bool bVar7;
  int local_4;
  
  if (DAT_007027a8 == 0) {
    DAT_007027a8 = 1;
    FUN_005405b0(&DAT_00e37078,0);
    if (DAT_00e38dda == '\x01') {
      return;
    }
  }
  DAT_00d3b124 = FUN_00418140();
  DAT_006fdc9c = DAT_00e38da4 + 0x10;
  DAT_006fdca0 = DAT_00e38da8 + 0x10;
  DAT_00d3b574 = DAT_00e38db0;
  DAT_00d3b570 = DAT_00e38dac;
  DAT_00d3b56c = (0x400 - DAT_00e38db0) / 2;
  DAT_00d3b14c = 0x23;
  DAT_00d3b568 = (0x300 - DAT_00e38dac) / 2;
  if (DAT_00e38dca == '\x01') {
    FUN_0053e590(DAT_00f55c44 + 1,0,0);
  }
  FUN_00536820(DAT_006fdca0,DAT_006fdc9c,0x26,0x1c);
  FUN_004cc770(*(int *)(&DAT_00e209fc + DAT_00d3b124 * 0x50) + DAT_00e36c74,DAT_006fdca0 + 0x10,
               DAT_006fdc9c + 0x10,DAT_00f56fb4,0);
  FUN_004cc770(*(int *)(&DAT_00e20a00 + DAT_00d3b124 * 0x50) + DAT_00e36c74,DAT_006fdca0 + 0x10,
               DAT_006fdc9c + 0x2e,DAT_00f56f8c,0);
  FUN_004ccbb0(0x3e,7,DAT_006fdca0 + 0x1dc,DAT_006fdc9c + 0x1a1,DAT_00f56f8c,0);
  if ((DAT_00e38dda == '5') && (cVar3 = FUN_0041b8f0(0x006fdd08u), cVar3 != '\0')) {
    FUN_004ccbb0(0xd,4,DAT_006fdca0 + 0x32,DAT_006fdc9c + 0x1a3,DAT_00f56f8c,0);
  }
  DAT_00e20778 = 0;
  FUN_004ccbb0(0x2c,0xd8,DAT_006fdca0 + 0x96,DAT_006fdc9c + 0x1a1,DAT_00f56f8c,0);
  FUN_004ccbb0(0x99,DAT_00e39080 + 1,DAT_00e20778 + 0x96 + DAT_006fdca0,DAT_006fdc9c + 0x1a1,
               DAT_00f56f8c,0);
  FUN_00536a30(DAT_006fdca0 + 0x10,DAT_006fdc9c + 0x40,DAT_00d3b14c + 1,6);
  FUN_004ccbb0(0x3e,10,DAT_006fdca0 + 0x20,DAT_006fdc9c + 0x48,DAT_00f56f9c,0);
  iVar2 = DAT_005797d8;
  iVar1 = DAT_005797d4;
  bVar7 = DAT_00784c58 != 0;
  _DAT_00f53f28 = 100;
  if (bVar7) {
    FUN_00537000(6,DAT_006fdca0 + DAT_005797d4,DAT_006fdc9c + DAT_005797d8,0xf,0,1);
    DAT_00e20778 = 0;
    FUN_004ccbb0(0x3e,0xb,DAT_006fdca0 + 8 + iVar1,DAT_006fdc9c + 3 + iVar2,DAT_00f56fa4,0);
    FUN_004cce00(DAT_00784c5c,0x40,&DAT_0057c048,DAT_00e20778 + DAT_006fdca0 + 8 + iVar1,
                 DAT_006fdc9c + 3 + iVar2,DAT_00f56fa4,0);
  }
  uVar6 = (uint)bVar7;
  if (DAT_00784c34 != 0) {
    iVar1 = (&DAT_005797d8)[uVar6 * 2];
    iVar2 = (&DAT_005797d4)[uVar6 * 2];
    uVar6 = uVar6 + 1;
    FUN_00537000(6,DAT_006fdca0 + iVar2,DAT_006fdc9c + iVar1,0xf,0,1);
    DAT_00e20778 = 0;
    FUN_004cce00(DAT_00784c34,0x40,&DAT_0057c048,DAT_006fdca0 + 8 + iVar2,DAT_006fdc9c + 3 + iVar1,
                 DAT_00f56fa4,0);
    FUN_004ccbb0(0x1d,DAT_00784c38 + 0x14,DAT_00e20778 + DAT_006fdca0 + 8 + iVar2,
                 DAT_006fdc9c + 3 + iVar1,DAT_00f56fa4,0);
  }
  local_4 = 0xc;
  puVar4 = &DAT_00784c24;
  piVar5 = &DAT_005797d8 + uVar6 * 2;
  do {
    if (*(char *)((int)&DAT_00784c30 + local_4) != '\0') {
      iVar1 = *piVar5;
      iVar2 = piVar5[-1];
      piVar5 = piVar5 + 2;
      FUN_00537000(6,DAT_006fdca0 + iVar2,DAT_006fdc9c + iVar1,0xf,0,1);
      DAT_00e20778 = 0;
      FUN_004ccbb0(0x3e,local_4,DAT_006fdca0 + 8 + iVar2,DAT_006fdc9c + 3 + iVar1,DAT_00f56fa4,0);
      FUN_004cce00(*puVar4,0x40,&DAT_0057c048,DAT_00e20778 + DAT_006fdca0 + 8 + iVar2,
                   DAT_006fdc9c + 3 + iVar1,DAT_00f56fa4,0);
    }
    puVar4 = puVar4 + 1;
    local_4 = local_4 + 1;
  } while ((int)puVar4 < 0x784c34);
  cVar3 = FUN_004e2530();
  iVar2 = DAT_005797f8;
  iVar1 = DAT_005797f4;
  if (cVar3 != '\0') {
    FUN_00537000(6,DAT_006fdca0 + DAT_005797f4,DAT_006fdc9c + DAT_005797f8,DAT_00d3b14c + -1,0,1);
    FUN_004cc770(DAT_00e20768,DAT_006fdca0 + 8 + iVar1,DAT_006fdc9c + 3 + iVar2,DAT_00f56fa4,0);
  }
  FUN_00536a30(DAT_006fdca0 + 0x10,DAT_006fdc9c + 0xa8,DAT_00d3b14c + -2,0xf);
  DAT_00d3b168 = DAT_00f56f9c;
  DAT_00d3b164 = DAT_00f56fa4;
  DAT_00d3b154 = DAT_006fdca0 + -6;
  DAT_00d3b150 = DAT_006fdc9c + 0xaa;
  if (DAT_00d3b138 == 0) {
    DAT_00d3b148 = 0xf;
    DAT_00d3b13c = 0xe;
    DAT_005c7e10 = 1;
    DAT_005c7e38 = 1;
    FUN_004c8070(*(int *)(&DAT_00e20a04 + DAT_00d3b124 * 0x50) + DAT_00e36c74,DAT_006fdca0 + 0x20,
                 DAT_006fdc9c + 0xb8,DAT_00d3b14c * 0x10 + -0x10,1,0);
    if (DAT_00d3b13c < DAT_00d3b138) {
      DAT_00d3b134 = DAT_00d3b138 - DAT_00d3b13c;
    }
    else {
      DAT_00d3b134 = 0;
      DAT_005c7e10 = 0;
      DAT_005c7e38 = 0;
    }
  }
  if (DAT_00d3b13c < DAT_00d3b138) {
    FUN_00536a30(DAT_006fdca0 + 0x22d,DAT_006fdc9c + 0xc0,2,0xc);
  }
  DAT_00d3b55c = DAT_006fdca0 + 0x13;
  DAT_00d3b558 = DAT_00d3b14c * 0x10 + 0x13 + DAT_006fdca0;
  DAT_00d3b554 = DAT_006fdc9c + 0xab;
  DAT_00d3b550 = DAT_006fdc9c + 0x195;
  FUN_004c8070(*(int *)(&DAT_00e20a04 + DAT_00d3b124 * 0x50) + DAT_00e36c74,DAT_006fdca0 + 0x20,
               DAT_006fdc9c + 0xb4,DAT_00d3b14c * 0x10 + -0x10,0,0);
  DAT_00d3b55c = 0;
  DAT_00d3b558 = DAT_00e38db0;
  DAT_00d3b554 = 0;
  DAT_00d3b550 = DAT_00e38dac;
  return;
}
