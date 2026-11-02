#include "restore_types.h"
#define DAT_0070cdf8 (*(undefined4 *)0x0070cdf8u)
#define DAT_0070cdfc (*(undefined4 *)0x0070cdfcu)
#define DAT_0070ce00 (*(undefined4 *)0x0070ce00u)
#define DAT_0070ce04 (*(undefined4 *)0x0070ce04u)
#define DAT_0078d7c4 (*(undefined *)0x0078d7c4u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a3e (*(undefined4 *)0x00799a3eu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a42 (*(undefined4 *)0x00799a42u)

void FUN_00454b40(int param_1)

{
  byte bVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  int iVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  int iVar14;
  int aiStack_10 [4];
  
  bVar1 = (&DAT_007999db)[param_1 * 0x108];
  iVar4 = (short)(&DAT_007999ea)[param_1 * 0x84] * 0x58;
  uVar2 = *(uint *)(&DAT_0070cdf8 + iVar4);
  if (1 < bVar1) {
    uVar2 = uVar2 << 2;
  }
  uVar12 = *(uint *)(&DAT_0070cdfc + iVar4);
  if (1 < bVar1) {
    uVar12 = uVar12 << 2;
  }
  uVar8 = *(uint *)(&DAT_0070ce00 + iVar4);
  if (1 < bVar1) {
    uVar8 = uVar8 << 2;
  }
  uVar5 = *(uint *)(&DAT_0070ce04 + iVar4);
  if (1 < bVar1) {
    uVar5 = uVar5 * 4;
  }
  uVar7 = uVar2 & 0x80000003;
  if ((int)uVar7 < 0) {
    uVar7 = (uVar7 - 1 | 0xfffffffc) + 1;
  }
  uVar13 = uVar12 & 0x80000003;
  if ((int)uVar13 < 0) {
    uVar13 = (uVar13 - 1 | 0xfffffffc) + 1;
  }
  uVar9 = uVar8 & 0x80000003;
  if ((int)uVar9 < 0) {
    uVar9 = (uVar9 - 1 | 0xfffffffc) + 1;
  }
  uVar11 = uVar5 & 0x80000003;
  if ((int)uVar11 < 0) {
    uVar11 = (uVar11 - 1 | 0xfffffffc) + 1;
  }
  uVar6 = DAT_0078d7c4 & 0x80000003;
  if ((int)uVar6 < 0) {
    uVar6 = (uVar6 - 1 | 0xfffffffc) + 1;
  }
  switch((int)(uVar6)) {
  case 0: ;
    aiStack_10[3] = 0;
    aiStack_10[1] = 1;
    aiStack_10[2] = 1;
    break;
  case 1: ;
  case 3: ;
    aiStack_10[3] = 1;
    aiStack_10[1] = 0;
    aiStack_10[2] = 0;
    break;
  case 2: ;
    aiStack_10[1] = 0;
    aiStack_10[2] = 1;
    aiStack_10[3] = 1;
  }
  if (uVar7 == 0) {
    iVar4 = (int)(uVar2 + ((int)uVar2 >> 0x1f & 3U)) >> 2;
  }
  else {
    iVar4 = aiStack_10[uVar7];
  }
  if (uVar13 == 0) {
    iVar14 = (int)(uVar12 + ((int)uVar12 >> 0x1f & 3U)) >> 2;
  }
  else {
    iVar14 = aiStack_10[uVar13];
  }
  if (uVar9 == 0) {
    iVar10 = (int)(uVar8 + ((int)uVar8 >> 0x1f & 3U)) >> 2;
  }
  else {
    iVar10 = aiStack_10[uVar9];
  }
  if (uVar11 == 0) {
    iVar3 = (int)(uVar5 + ((int)uVar5 >> 0x1f & 3U)) >> 2;
  }
  else {
    iVar3 = aiStack_10[uVar11];
  }
  if (0 < iVar3) {
    if ((short)(&DAT_00799a3c)[param_1 * 0x84] < iVar3) {
      (&DAT_00799a3c)[param_1 * 0x84] = 0;
    }
    else {
      (&DAT_00799a3c)[param_1 * 0x84] = (&DAT_00799a3c)[param_1 * 0x84] - (short)iVar3;
    }
  }
  if (0 < iVar4) {
    if ((short)(&DAT_00799a3e)[param_1 * 0x84] < iVar4) {
      (&DAT_00799a3e)[param_1 * 0x84] = 0;
    }
    else {
      (&DAT_00799a3e)[param_1 * 0x84] = (&DAT_00799a3e)[param_1 * 0x84] - (short)iVar4;
    }
  }
  if (0 < iVar14) {
    if ((short)(&DAT_00799a40)[param_1 * 0x84] < iVar14) {
      (&DAT_00799a40)[param_1 * 0x84] = 0;
    }
    else {
      (&DAT_00799a40)[param_1 * 0x84] = (&DAT_00799a40)[param_1 * 0x84] - (short)iVar14;
    }
  }
  if (0 < iVar10) {
    if ((short)(&DAT_00799a42)[param_1 * 0x84] < iVar10) {
      (&DAT_00799a42)[param_1 * 0x84] = 0;
      return;
    }
    (&DAT_00799a42)[param_1 * 0x84] = (&DAT_00799a42)[param_1 * 0x84] - (short)iVar10;
  }
  return;
}
