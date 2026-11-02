#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00f56d54 (*(undefined *)0x00f56d54u)
#define DAT_00f56d5c (*(undefined *)0x00f56d5cu)
#define DAT_00f56d64 (*(undefined *)0x00f56d64u)
#define PTR_s_Obelisk3x3a_005e4588 (*(unsigned char * *)0x005e4588u)
extern void FUN_0047bf50();

void FUN_004e5840(int param_1,undefined4 param_2,undefined4 param_3)

{
  short sVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  
  uVar2 = (*(byte *)((int)&DAT_00799a2c + param_1 * 0x108 + 1) - 0x42) * 3;
  uVar3 = -(uint)((&DAT_007999e8)[param_1 * 0x84] != 0xcf) & 2;
  iVar4 = uVar3 + 4;
  iVar6 = uVar3 + 3;
  if ((int)(iVar4 * (((int)uVar2 < 1) - 1 & uVar2)) / 100 < iVar6) {
    iVar6 = (int)(iVar4 * (((int)uVar2 < 1) - 1 & uVar2)) / 100;
  }
  FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,(&PTR_s_Obelisk3x3a_005e4588)
               [(-(uint)((&DAT_007999e8)[param_1 * 0x84] != 0xcf) & 4) + iVar6],
               (short)(&DAT_007999ea)[param_1 * 0x84] + 5);
  sVar1 = (&DAT_007999ea)[param_1 * 0x84];
  uVar5 = DAT_00f56d54;
  if (((sVar1 != 0) && (uVar5 = DAT_00f56d5c, sVar1 != 1)) && (uVar5 = 0, sVar1 == 2)) {
    uVar5 = DAT_00f56d64;
  }
  FUN_0047bf50(param_1,param_2,param_3,(&DAT_007999db)[param_1 * 0x108],
               (&DAT_007999db)[param_1 * 0x108],uVar5,8);
  return;
}
