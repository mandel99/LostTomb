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
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern void FUN_0047bf50();
extern void FUN_0047c480();

void FUN_0046bda0(int param_1,short param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  short *psVar5;
  int iVar6;
  int iVar7;
  
  psVar5 = &DAT_0078d590;
  iVar6 = param_1 * 0x108;
  (&DAT_007999db)[iVar6] = 2;
  *(undefined1 *)((int)&DAT_007999dc + iVar6 + 1) = 2;
  (&DAT_007999f4)[param_1 * 0x84] = (&DAT_007999f4)[param_1 * 0x84] + param_2;
  psVar4 = &DAT_00799a2c + param_1 * 0x84;
  do {
    sVar1 = *psVar5;
    psVar5 = psVar5 + 2;
    *psVar4 = *psVar4 + sVar1;
    psVar4 = psVar4 + 1;
  } while ((int)psVar5 < 0x78d5c0);
  iVar7 = (&DAT_00f55bb4)[*(int *)(&DAT_005d03ec + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) * 2]
          + 4;
  if (*(int *)(&DAT_005d043c + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) != 0) {
    iVar7 = (&DAT_00f55bb4)
            [*(int *)(&DAT_005d03ec + (short)(&DAT_007999ea)[param_1 * 0x84] * 4) * 2] + 5;
  }
  FUN_0047c480(param_1,(int)*(short *)((int)&DAT_007999dc + iVar6 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar6));
  iVar3 = DAT_0078d770;
  iVar2 = DAT_0078d76c;
  *(short *)((int)&DAT_007999dc + iVar6 + 2) = (short)DAT_0078d770;
  *(short *)(&DAT_007999e0 + iVar6) = (short)iVar2;
  *(int *)(&DAT_007999e4 + iVar6) = DAT_00bca2d8 + iVar2 * 0xe4 + iVar3;
  *(undefined1 *)(&DAT_007999dc + param_1 * 0x42) = 1;
  FUN_0047bf50(param_1,(int)*(short *)((int)&DAT_007999dc + iVar6 + 2),
               (int)*(short *)(&DAT_007999e0 + iVar6),2,2,iVar7,8);
  return;
}
