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

void FUN_0046c500(int param_1)

{
  short *psVar1;
  short *psVar2;
  short sVar3;
  short sVar4;
  int iVar5;
  int iVar6;
  short *psVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  undefined2 *puVar11;
  int iVar12;
  int iVar13;
  int *piVar14;
  int iVar15;
  int *local_78;
  int *local_74;
  int local_70;
  int local_6c;
  int local_60 [12];
  int aiStack_30 [12];
  
  iVar13 = param_1 * 0x108;
  sVar3 = (&DAT_007999f4)[param_1 * 0x84];
  psVar7 = &DAT_00799a2c + param_1 * 0x84;
  iVar5 = 0;
  do {
    sVar4 = *psVar7;
    iVar6 = iVar5 + 4;
    psVar7 = psVar7 + 1;
    *(int *)((int)local_60 + iVar5) = (int)sVar4 / 9;
    *(int *)((int)aiStack_30 + iVar5) = (int)sVar4 % 9;
    iVar5 = iVar6;
  } while (iVar6 < 0x30);
  sVar4 = sVar3 / 9;
  FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar13 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar13));
  (&DAT_007999e8)[param_1 * 0x84] = 0x13;
  (&DAT_007999ea)[param_1 * 0x84] = 9;
  (&DAT_007999db)[iVar13] = 1;
  *(undefined1 *)((int)&DAT_007999dc + iVar13 + 1) = 1;
  *(undefined1 *)(&DAT_007999dc + param_1 * 0x42) = 0;
  iVar5 = 0;
  (&DAT_007999f4)[param_1 * 0x84] = sVar3 % 9 + sVar4;
  psVar7 = &DAT_00799a2c + param_1 * 0x84;
  do {
    psVar1 = (short *)((int)aiStack_30 + iVar5);
    psVar2 = (short *)((int)local_60 + iVar5);
    iVar5 = iVar5 + 4;
    *psVar7 = *psVar1 + *psVar2;
    psVar7 = psVar7 + 1;
  } while (iVar5 < 0x30);
  (&DAT_007999f8)[param_1 * 0x84] = 0;
  iVar15 = (&DAT_00f55bb4)[*(int *)(&DAT_005d03ec + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) * 2]
           + *(int *)(&DAT_005d043c + (short)(&DAT_007999ea)[param_1 * 0x84] * 4);
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar13 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar13),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar13)] & 1) + iVar15,8);
  iVar12 = (int)*(short *)((int)&DAT_007999dc + iVar13 + 2);
  iVar13 = (int)*(short *)(&DAT_007999e0 + iVar13);
  iVar5 = iVar12 + 1;
  iVar6 = FUN_0046a400(DAT_00eaac28,0x13,iVar5,iVar13,0,0);
  local_78 = local_60;
  iVar8 = iVar6 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar6 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar6 * 0x84;
  do {
    *puVar11 = (short)*local_78;
    local_78 = local_78 + 1;
    local_70 = local_70 + -1;
    puVar11 = puVar11 + 1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar6 * 0x84] = 0;
  FUN_0047bf50(iVar6,(int)*(short *)((int)&DAT_007999dc + iVar8 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar8),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar8)] & 1) + iVar15,8);
  iVar6 = iVar12 + 2;
  iVar8 = FUN_0046a400(DAT_00eaac28,0x13,iVar6,iVar13,0,0);
  local_78 = local_60;
  iVar9 = iVar8 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar8 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar8 * 0x84;
  do {
    *puVar11 = (short)*local_78;
    local_78 = local_78 + 1;
    local_70 = local_70 + -1;
    puVar11 = puVar11 + 1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar8 * 0x84] = 0;
  FUN_0047bf50(iVar8,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar9),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar9)] & 1) + iVar15,8);
  iVar8 = iVar13 + 1;
  iVar9 = FUN_0046a400(DAT_00eaac28,0x13,iVar12,iVar8,0,0);
  local_78 = local_60;
  iVar10 = iVar9 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar9 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar9 * 0x84;
  do {
    *puVar11 = (short)*local_78;
    local_78 = local_78 + 1;
    local_70 = local_70 + -1;
    puVar11 = puVar11 + 1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar9 * 0x84] = 0;
  FUN_0047bf50(iVar9,(int)*(short *)((int)&DAT_007999dc + iVar10 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar10),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar10)] & 1) + iVar15,8);
  iVar9 = FUN_0046a400(DAT_00eaac28,0x13,iVar5,iVar8,0,0);
  local_78 = local_60;
  iVar10 = iVar9 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar9 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar9 * 0x84;
  do {
    *puVar11 = (short)*local_78;
    local_78 = local_78 + 1;
    local_70 = local_70 + -1;
    puVar11 = puVar11 + 1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar9 * 0x84] = 0;
  FUN_0047bf50(iVar9,(int)*(short *)((int)&DAT_007999dc + iVar10 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar10),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar10)] & 1) + iVar15,8);
  iVar8 = FUN_0046a400(DAT_00eaac28,0x13,iVar6,iVar8,0,0);
  piVar14 = local_60;
  iVar9 = iVar8 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar8 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar8 * 0x84;
  do {
    iVar10 = *piVar14;
    piVar14 = piVar14 + 1;
    *puVar11 = (short)iVar10;
    puVar11 = puVar11 + 1;
    local_70 = local_70 + -1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar8 * 0x84] = 0;
  FUN_0047bf50(iVar8,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar9),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar9)] & 1) + iVar15,8);
  iVar13 = iVar13 + 2;
  iVar8 = FUN_0046a400(DAT_00eaac28,0x13,iVar12,iVar13,0,0);
  local_74 = local_60;
  iVar12 = iVar8 * 0x108;
  local_70 = 0xc;
  (&DAT_007999f4)[iVar8 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar8 * 0x84;
  do {
    *puVar11 = (short)*local_74;
    local_74 = local_74 + 1;
    local_70 = local_70 + -1;
    puVar11 = puVar11 + 1;
  } while (local_70 != 0);
  (&DAT_007999f8)[iVar8 * 0x84] = 0;
  FUN_0047bf50(iVar8,(int)*(short *)((int)&DAT_007999dc + iVar12 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar12),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar12)] & 1) + iVar15,8);
  iVar5 = FUN_0046a400(DAT_00eaac28,0x13,iVar5,iVar13,0,0);
  local_74 = local_60;
  iVar8 = iVar5 * 0x108;
  local_6c = 0xc;
  (&DAT_007999f4)[iVar5 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar5 * 0x84;
  do {
    *puVar11 = (short)*local_74;
    local_74 = local_74 + 1;
    local_6c = local_6c + -1;
    puVar11 = puVar11 + 1;
  } while (local_6c != 0);
  (&DAT_007999f8)[iVar5 * 0x84] = 0;
  FUN_0047bf50(iVar5,(int)*(short *)((int)&DAT_007999dc + iVar8 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar8),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar8)] & 1) + iVar15,8);
  iVar5 = FUN_0046a400(DAT_00eaac28,0x13,iVar6,iVar13,0,0);
  piVar14 = local_60;
  iVar13 = iVar5 * 0x108;
  (&DAT_007999f4)[iVar5 * 0x84] = sVar4;
  puVar11 = &DAT_00799a2c + iVar5 * 0x84;
  iVar6 = 0xc;
  do {
    iVar8 = *piVar14;
    piVar14 = piVar14 + 1;
    *puVar11 = (short)iVar8;
    puVar11 = puVar11 + 1;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  (&DAT_007999f8)[iVar5 * 0x84] = 0;
  FUN_0047bf50(iVar5,(int)*(short *)((int)&DAT_007999dc + iVar13 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar13),1,1,
               ((byte)(&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar13)] & 1) + iVar15,8);
  return;
}
