#include "restore_types.h"
#define DAT_005700d4 (*(undefined4 *)0x005700d4u)
#define DAT_005700e8 (*(undefined4 *)0x005700e8u)
#define DAT_005d17a8 (*(undefined *)0x005d17a8u)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2e8 (*(undefined *)0x00bca2e8u)
#define DAT_00bca2ec (*(undefined *)0x00bca2ecu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_0046a400();
extern void FUN_00470990();
extern undefined4 FUN_00485100();

void FUN_004f5f10(int param_1)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  uVar1 = DAT_005d17a8;
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xb7) {
    param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
    if (DAT_005e4ef4 == 5) {
      iVar2 = *(int *)(&DAT_005700e8 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 4);
    }
    else {
      iVar2 = *(int *)(&DAT_005700d4 + (uint)*(byte *)(&DAT_00799a2c + param_1 * 0x84) * 4);
    }
    iVar3 = (*(int *)(&DAT_007999e4 + param_1 * 0x108) + iVar2) % 0xe4 - DAT_00bca2e8;
    iVar4 = (*(int *)(&DAT_007999e4 + param_1 * 0x108) + iVar2) / 0xe4 - DAT_00bca2ec;
    iVar2 = FUN_0046a400(DAT_00eaac28,0xd7,iVar3,iVar4,0,0);
    FUN_00485100(iVar2);
    FUN_00470990(DAT_00eaac28,0xd7,iVar2,iVar3,iVar4,uVar1,0,0);
    (&DAT_007999ea)[iVar2 * 0x84] = (short)param_1;
  }
  return;
}
