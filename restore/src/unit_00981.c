#include "restore_types.h"
#define DAT_005d03ec (*(undefined4 *)0x005d03ecu)
#define DAT_005d043c (*(undefined4 *)0x005d043cu)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00af5994 (*(undefined4 *)0x00af5994u)
#define DAT_00af60ac (*(undefined4 *)0x00af60acu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern void FUN_00476530();
extern void FUN_0047bf50();
extern void FUN_0047c480();
extern void FUN_00516f90();
extern void FUN_005173a0();

void FUN_0046cb30(int param_1)

{
  short *psVar1;
  short *psVar2;
  short sVar3;
  short sVar4;
  longlong lVar5;
  int iVar6;
  uint *puVar7;
  int iVar8;
  short *psVar9;
  uint *puVar10;
  int iVar11;
  int iVar12;
  int aiStack_60 [12];
  int aiStack_30 [12];
  
  iVar11 = param_1 * 0x108;
  sVar3 = (&DAT_007999f4)[param_1 * 0x84];
  psVar9 = &DAT_00799a2c + param_1 * 0x84;
  iVar6 = 0;
  do {
    sVar4 = *psVar9;
    iVar8 = iVar6 + 4;
    psVar9 = psVar9 + 1;
    *(int *)((int)aiStack_30 + iVar6) = (int)sVar4 / 6;
    *(int *)((int)aiStack_60 + iVar6) = (int)sVar4 % 6;
    iVar6 = iVar8;
  } while (iVar8 < 0x30);
  FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar11));
  (&DAT_007999e8)[param_1 * 0x84] = 0x17;
  (&DAT_007999ea)[param_1 * 0x84] = 0xd;
  (&DAT_007999db)[iVar11] = 2;
  *(undefined1 *)((int)&DAT_007999dc + iVar11 + 1) = 2;
  *(undefined1 *)(&DAT_007999dc + param_1 * 0x42) = 0;
  lVar5 = (longlong)(int)sVar3 * 0x2aaaaaab;
  (&DAT_007999f4)[param_1 * 0x84] =
       ((short)((ulonglong)lVar5 >> 0x20) + sVar3 % 6) - (short)(lVar5 >> 0x3f);
  iVar6 = 0;
  psVar9 = &DAT_00799a2c + param_1 * 0x84;
  do {
    psVar1 = (short *)((int)aiStack_60 + iVar6);
    psVar2 = (short *)((int)aiStack_30 + iVar6);
    iVar6 = iVar6 + 4;
    *psVar9 = *psVar1 + *psVar2;
    psVar9 = psVar9 + 1;
  } while (iVar6 < 0x30);
  (&DAT_007999f8)[param_1 * 0x84] = 0;
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar11),2,2,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar11)] & 1) +
               (&DAT_00f55bb4)
               [*(int *)(&DAT_005d03ec + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) * 2] +
               *(int *)(&DAT_005d043c + (short)(&DAT_007999ea)[param_1 * 0x84] * 4),8);
  iVar12 = (int)*(short *)((int)&DAT_007999dc + iVar11 + 2);
  iVar8 = (int)*(short *)(&DAT_007999e0 + iVar11);
  iVar11 = 3;
  iVar6 = (DAT_00bca2d8 + iVar8 * 0xe4 + iVar12) * 4;
  puVar7 = (uint *)(&DAT_00af5994 + iVar6);
  puVar10 = (uint *)(&DAT_00af60ac + iVar6);
  do {
    *puVar7 = *puVar7 | 0x20;
    iVar11 = iVar11 + -1;
    *puVar10 = *puVar10 | 0x20;
    puVar7 = puVar7 + 0xe4;
    puVar10 = puVar10 + 1;
  } while (iVar11 != 0);
  FUN_00476530();
  FUN_00516f90(iVar12,iVar8,iVar12 + 2,iVar8 + 2,1);
  FUN_005173a0(iVar12,iVar8,iVar12 + 2,iVar8 + 2);
  return;
}
