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
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern int FUN_0046a400();
extern void FUN_0047bf50();
extern void FUN_0047c480();

void FUN_0046c1c0(int param_1)

{
  short *psVar1;
  short *psVar2;
  short sVar3;
  short sVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  short *psVar8;
  int iVar9;
  undefined2 *puVar10;
  int iVar11;
  int *piVar12;
  uint uVar13;
  uint uVar14;
  int iVar15;
  int *local_74;
  int local_70;
  int local_60 [12];
  uint auStack_30 [12];
  
  iVar11 = param_1 * 0x108;
  iVar7 = 0;
  uVar5 = (uint)(short)(&DAT_007999f4)[param_1 * 0x84];
  psVar8 = &DAT_00799a2c + param_1 * 0x84;
  do {
    uVar13 = (uint)*psVar8;
    uVar14 = uVar13 & 0x80000003;
    *(int *)((int)local_60 + iVar7) = (int)(uVar13 + ((int)uVar13 >> 0x1f & 3U)) >> 2;
    if ((int)uVar14 < 0) {
      uVar14 = (uVar14 - 1 | 0xfffffffc) + 1;
    }
    *(uint *)((int)auStack_30 + iVar7) = uVar14;
    iVar7 = iVar7 + 4;
    psVar8 = psVar8 + 1;
  } while (iVar7 < 0x30);
  FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar11));
  (&DAT_007999e8)[param_1 * 0x84] = 0x13;
  (&DAT_007999ea)[param_1 * 0x84] = 9;
  (&DAT_007999db)[iVar11] = 1;
  *(undefined1 *)((int)&DAT_007999dc + iVar11 + 1) = 1;
  uVar13 = uVar5 & 0x80000003;
  *(undefined1 *)(&DAT_007999dc + param_1 * 0x42) = 0;
  if ((int)uVar13 < 0) {
    uVar13 = (uVar13 - 1 | 0xfffffffc) + 1;
  }
  sVar4 = (short)((int)(uVar5 + ((int)uVar5 >> 0x1f & 3U)) >> 2);
  (&DAT_007999f4)[param_1 * 0x84] = (short)uVar13 + sVar4;
  iVar7 = 0;
  psVar8 = &DAT_00799a2c + param_1 * 0x84;
  do {
    psVar1 = (short *)((int)auStack_30 + iVar7);
    psVar2 = (short *)((int)local_60 + iVar7);
    iVar7 = iVar7 + 4;
    *psVar8 = *psVar1 + *psVar2;
    psVar8 = psVar8 + 1;
  } while (iVar7 < 0x30);
  (&DAT_007999f8)[param_1 * 0x84] = 0;
  iVar15 = (&DAT_00f55bb4)[*(int *)(&DAT_005d03ec + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) * 2]
           + *(int *)(&DAT_005d043c + (short)(&DAT_007999ea)[param_1 * 0x84] * 4);
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar11),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar11)] & 1) + iVar15,8);
  sVar3 = *(short *)(&DAT_007999e0 + iVar11);
  iVar7 = (int)*(short *)((int)&DAT_007999dc + iVar11 + 2);
  iVar11 = iVar7 + 1;
  iVar6 = FUN_0046a400(DAT_00eaac28,(int)(short)(&DAT_007999e8)[param_1 * 0x84],iVar11,(int)sVar3,0,
                       0);
  local_74 = local_60;
  iVar9 = iVar6 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar6 * 0x84] = sVar4;
  puVar10 = &DAT_00799a2c + iVar6 * 0x84;
  do {
    *puVar10 = (short)*local_74;
    local_74 = local_74 + 1;
    local_70 = local_70 + -1;
    puVar10 = puVar10 + 1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar6 * 0x84] = 0;
  FUN_0047bf50(iVar6,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar9),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar9)] & 1) + iVar15,8);
  iVar6 = sVar3 + 1;
  iVar7 = FUN_0046a400(DAT_00eaac28,(int)(short)(&DAT_007999e8)[param_1 * 0x84],iVar7,iVar6,0,0);
  local_74 = local_60;
  iVar9 = iVar7 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar7 * 0x84] = sVar4;
  puVar10 = &DAT_00799a2c + iVar7 * 0x84;
  do {
    *puVar10 = (short)*local_74;
    local_74 = local_74 + 1;
    local_70 = local_70 + -1;
    puVar10 = puVar10 + 1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar7 * 0x84] = 0;
  FUN_0047bf50(iVar7,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar9),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar9)] & 1) + iVar15,8);
  iVar7 = FUN_0046a400(DAT_00eaac28,(int)(short)(&DAT_007999e8)[param_1 * 0x84],iVar11,iVar6,0,0);
  piVar12 = local_60;
  iVar11 = iVar7 * 0x108;
  (&DAT_007999f4)[iVar7 * 0x84] = sVar4;
  puVar10 = &DAT_00799a2c + iVar7 * 0x84;
  iVar6 = 0xc;
  do {
    iVar9 = *piVar12;
    piVar12 = piVar12 + 1;
    *puVar10 = (short)iVar9;
    puVar10 = puVar10 + 1;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  (&DAT_007999f8)[iVar7 * 0x84] = 0;
  FUN_0047bf50(iVar7,(int)*(short *)((int)&DAT_007999dc + iVar11 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar11),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar11)] & 1) + iVar15,8);
  return;
}
