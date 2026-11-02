#include "restore_types.h"
#define DAT_005d03ec (*(undefined4 *)0x005d03ecu)
#define DAT_005d043c (*(undefined4 *)0x005d043cu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00af5998 (*(undefined4 *)0x00af5998u)
#define DAT_00af643c (*(undefined4 *)0x00af643cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern void FUN_00476530();
extern void FUN_0047bf50();
extern void FUN_0047c480();
extern void FUN_00516f90();
extern void FUN_005173a0();

void FUN_0046ccf0(int param_1)

{
  short *psVar1;
  short *psVar2;
  uint uVar3;
  uint *puVar4;
  int iVar5;
  short *psVar6;
  uint *puVar7;
  int iVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  uint auStack_60 [12];
  int aiStack_30 [12];
  
  iVar8 = param_1 * 0x108;
  iVar5 = 0;
  uVar3 = (uint)(short)(&DAT_007999f4)[param_1 * 0x84];
  psVar6 = &DAT_00799a2c + param_1 * 0x84;
  do {
    uVar10 = (uint)*psVar6;
    uVar11 = uVar10 & 0x80000007;
    *(int *)((int)aiStack_30 + iVar5) = (int)(uVar10 + ((int)uVar10 >> 0x1f & 7U)) >> 3;
    if ((int)uVar11 < 0) {
      uVar11 = (uVar11 - 1 | 0xfffffff8) + 1;
    }
    *(uint *)((int)auStack_60 + iVar5) = uVar11;
    iVar5 = iVar5 + 4;
    psVar6 = psVar6 + 1;
  } while (iVar5 < 0x30);
  FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar8 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar8));
  (&DAT_007999e8)[param_1 * 0x84] = 0x1b;
  (&DAT_007999ea)[param_1 * 0x84] = 0x11;
  (&DAT_007999db)[iVar8] = 3;
  *(undefined1 *)((int)&DAT_007999dc + iVar8 + 1) = 3;
  uVar10 = uVar3 & 0x80000007;
  *(undefined1 *)(&DAT_007999dc + param_1 * 0x42) = 0;
  if ((int)uVar10 < 0) {
    uVar10 = (uVar10 - 1 | 0xfffffff8) + 1;
  }
  iVar5 = 0;
  (&DAT_007999f4)[param_1 * 0x84] =
       (short)uVar10 + (short)((int)(uVar3 + ((int)uVar3 >> 0x1f & 7U)) >> 3);
  psVar6 = &DAT_00799a2c + param_1 * 0x84;
  do {
    psVar1 = (short *)((int)auStack_60 + iVar5);
    psVar2 = (short *)((int)aiStack_30 + iVar5);
    iVar5 = iVar5 + 4;
    *psVar6 = *psVar1 + *psVar2;
    psVar6 = psVar6 + 1;
  } while (iVar5 < 0x30);
  (&DAT_007999f8)[param_1 * 0x84] = 0;
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar8 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar8),3,3,
               (&DAT_00f55bb4)
               [*(int *)(&DAT_005d03ec + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) * 2] +
               *(int *)(&DAT_005d043c + (short)(&DAT_007999ea)[param_1 * 0x84] * 4),8);
  iVar12 = (int)*(short *)((int)&DAT_007999dc + iVar8 + 2);
  iVar9 = (int)*(short *)(&DAT_007999e0 + iVar8);
  iVar8 = 4;
  iVar5 = (DAT_00bca2d8 + iVar9 * 0xe4 + iVar12) * 4;
  puVar4 = (uint *)(&DAT_00af5998 + iVar5);
  puVar7 = (uint *)(&DAT_00af643c + iVar5);
  do {
    *puVar4 = *puVar4 | 0x20;
    iVar8 = iVar8 + -1;
    *puVar7 = *puVar7 | 0x20;
    puVar4 = puVar4 + 0xe4;
    puVar7 = puVar7 + 1;
  } while (iVar8 != 0);
  FUN_00476530();
  FUN_00516f90(iVar12,iVar9,iVar12 + 3,iVar9 + 3,1);
  FUN_005173a0(iVar12,iVar9,iVar12 + 3,iVar9 + 3);
  return;
}
