#include "restore_types.h"
#define DAT_0070ce1c (*(undefined4 *)0x0070ce1cu)
#define DAT_0070ce20 (*(undefined4 *)0x0070ce20u)
#define DAT_00784cb0 (*(undefined1 *)0x00784cb0u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799ac0 (*(undefined4 *)0x00799ac0u)
#define DAT_00799acc (*(undefined4 *)0x00799accu)
#define DAT_00799ad0 (*(undefined4 *)0x00799ad0u)
#define DAT_00799ad4 (*(undefined4 *)0x00799ad4u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00d3b380 (*(undefined *)0x00d3b380u)
#define DAT_00d3b384 (*(undefined *)0x00d3b384u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b364 (*(undefined4 *)0x00d3b364u)
#define _DAT_00d3b368 (*(undefined4 *)0x00d3b368u)
#define _DAT_00d3b36c (*(undefined4 *)0x00d3b36cu)
#define _DAT_00d3b370 (*(undefined4 *)0x00d3b370u)
#define _DAT_00d3b378 (*(undefined4 *)0x00d3b378u)
extern int FUN_004eb800();
extern longlong FUN_0056133c();

void FUN_004e7f00(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int local_4;
  
  iVar1 = param_1;
  (&DAT_00d3b1c8)[DAT_00d3b1c4] = DAT_00d3b374;
  iVar2 = DAT_00eaac28;
  DAT_00d3b1c4 = DAT_00d3b1c4 + 1;
  if (99 < DAT_00d3b1c4) {
    DAT_00d3b1c4 = 0;
  }
  iVar8 = 0x1f;
  do {
    uVar3 = DAT_00d3b384 >> 4;
    uVar5 = DAT_00d3b384 & 1;
    uVar6 = DAT_00d3b380 >> 4;
    uVar4 = DAT_00d3b380 & 1;
    DAT_00d3b384 = DAT_00d3b384 >> 1;
    DAT_00d3b380 = DAT_00d3b380 >> 1;
    if ((uVar3 & 1) != uVar5) {
      DAT_00d3b384 = DAT_00d3b384 | 0x40000000;
    }
    if ((uVar6 & 1) != uVar4) {
      DAT_00d3b380 = DAT_00d3b380 | 0x40000000;
    }
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  _DAT_00d3b370 = DAT_00d3b384 & 0x7fff;
  DAT_00d3b374 = DAT_00d3b384 & 0x7f;
  _DAT_00d3b378 = DAT_00d3b384 & 7;
  _DAT_00d3b36c = DAT_00d3b380 & 7;
  _DAT_00d3b364 = DAT_00d3b380 & 0x7fff;
  _DAT_00d3b368 = DAT_00d3b380 & 0x7f;
  iVar7 = param_1 * 0x108;
  iVar8 = *(int *)(&DAT_00799ad4 + iVar7) +
          *(int *)(&DAT_0070ce20 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x58) +
          (0x32 - *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8)) * 2;
  *(int *)(&DAT_00799ad4 + iVar7) = iVar8;
  if (iVar8 < 0) {
    *(undefined4 *)(&DAT_00799ad4 + iVar7) = 0;
  }
  if (1000 < *(int *)(&DAT_00799ad4 + iVar7)) {
    *(undefined4 *)(&DAT_00799ad4 + iVar7) = 1000;
  }
  if ((char)param_2 == '\0') {
    local_4 = *(int *)(&DAT_00799ad0 + iVar7);
    iVar8 = *(int *)(&DAT_00799acc + iVar7);
  }
  else {
    iVar8 = DAT_00bca2d0;
    if (DAT_00bca2d0 < DAT_00bca2d4) {
      iVar8 = DAT_00bca2d4;
    }
    iVar2 = FUN_004eb800((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                         (int)*(short *)(&DAT_007999e0 + iVar7),0,0,0x40000,&param_2,&param_1,iVar8,
                         0);
    local_4 = iVar8;
    if (iVar2 != 0) {
      local_4 = FUN_0056133c();
    }
    iVar2 = FUN_004eb800((int)*(short *)((int)&DAT_007999dc + iVar7 + 2),
                         (int)*(short *)(&DAT_007999e0 + iVar7),0,0,4,&param_2,&param_1,iVar8,0);
    if (iVar2 != 0) {
      param_2 = *(short *)((int)&DAT_007999dc + iVar7 + 2) - param_2;
      param_2 = (*(short *)(&DAT_007999e0 + iVar7) - param_1) *
                (*(short *)(&DAT_007999e0 + iVar7) - param_1) + param_2 * param_2;
      iVar8 = FUN_0056133c();
    }
    iVar2 = DAT_00eaac28;
    *(int *)(&DAT_00799acc + iVar7) = iVar8;
    *(int *)(&DAT_00799ad0 + iVar7) = local_4;
  }
  iVar7 = 1;
  iVar9 = 0x10;
  if (DAT_00784cb0 == '\0') {
    iVar8 = 1;
  }
  else if (DAT_00784cb0 == '\x02') {
    iVar9 = 8;
    iVar7 = 2;
  }
  iVar2 = (&DAT_00799ac0)[iVar1 * 0x42] +
          ((((*(int *)(&DAT_0070ce1c + (short)(&DAT_007999ea)[iVar1 * 0x84] * 0x58) -
             *(int *)(&DAT_00ea1888 + iVar2 * 0x49d8)) + (0x37 - local_4) * 2) - iVar8) + iVar9) /
          iVar7;
  (&DAT_00799ac0)[iVar1 * 0x42] = iVar2;
  if (iVar2 < 0) {
    (&DAT_00799ac0)[iVar1 * 0x42] = 0;
  }
  if (1000 < (int)(&DAT_00799ac0)[iVar1 * 0x42]) {
    (&DAT_00799ac0)[iVar1 * 0x42] = 1000;
  }
  return;
}
