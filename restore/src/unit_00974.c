#include "restore_types.h"
#define DAT_005d03ec (*(undefined4 *)0x005d03ecu)
#define DAT_005d043c (*(undefined4 *)0x005d043cu)
#define DAT_0078d590 (*(undefined4 *)0x0078d590u)
#define DAT_0078d76c (*(undefined4 *)0x0078d76cu)
#define DAT_0078d770 (*(undefined4 *)0x0078d770u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00acf85c (*(undefined4 *)0x00acf85cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern void FUN_0047bf50();
extern void FUN_0047c480();

void FUN_0046bab0(int param_1,short param_2)

{
  byte bVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  short *psVar7;
  short *psVar8;
  int iVar9;
  
  iVar9 = param_1 * 0x108;
  psVar8 = &DAT_0078d590;
  (&DAT_007999e8)[param_1 * 0x84] = 0x14;
  (&DAT_007999ea)[param_1 * 0x84] = 10;
  (&DAT_007999db)[iVar9] = 2;
  *(undefined1 *)((int)&DAT_007999dc + iVar9 + 1) = 2;
  (&DAT_007999f4)[param_1 * 0x84] = (&DAT_007999f4)[param_1 * 0x84] + param_2;
  psVar7 = &DAT_00799a2c + param_1 * 0x84;
  do {
    sVar2 = *psVar8;
    psVar8 = psVar8 + 2;
    *psVar7 = *psVar7 + sVar2;
    psVar7 = psVar7 + 1;
  } while ((int)psVar8 < 0x78d5c0);
  bVar1 = (&DAT_00acf85c)[*(int *)(&DAT_007999e4 + iVar9)];
  iVar3 = (&DAT_00f55bb4)[*(int *)(&DAT_005d03ec + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) * 2];
  iVar4 = *(int *)(&DAT_005d043c + (short)(&DAT_007999ea)[param_1 * 0x84] * 4);
  FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar9));
  iVar6 = DAT_0078d770;
  iVar5 = DAT_0078d76c;
  *(short *)((int)&DAT_007999dc + iVar9 + 2) = (short)DAT_0078d770;
  *(short *)(&DAT_007999e0 + iVar9) = (short)iVar5;
  *(int *)(&DAT_007999e4 + iVar9) = DAT_00bca2d8 + iVar5 * 0xe4 + iVar6;
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar9 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar9),(&DAT_007999db)[iVar9],(&DAT_007999db)[iVar9],
               (bVar1 & 1) + iVar3 + iVar4,8);
  return;
}
