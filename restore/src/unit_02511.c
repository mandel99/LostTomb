#include "restore_types.h"
#include "sprite_libraries.h"
#define DAT_00784914 (*(undefined4 *)0x00784914u)
#define DAT_00e94e18 (*(undefined *)0x00e94e18u)
#define _DAT_00f55bac (*(undefined4 *)0x00f55bacu)
#define s_MAUSOLEUM0_EXTRA_0060182c (*(char (*)[17])0x0060182cu)
#define s_MAUSOLEUM1_EXTRA_00601804 (*(char (*)[17])0x00601804u)
#define s_MAUSOLEUM2_EXTRA_00601818 (*(char (*)[17])0x00601818u)
#define s_OBELISK_EXTRA_006017e0 (*(char (*)[14])0x006017e0u)
#define s_SPHINX_EXTRA_005791a8 (*(char (*)[13])0x005791a8u)
#define s_SUN_TEMPLE_EXTRA_006017f0 (*(char (*)[17])0x006017f0u)
#define s_The_Glory_of_Ptolemy_00601840 (*(char (*)[21])0x00601840u)
extern int FUN_0053d8e0();
extern int FUN_0053d930();

void FUN_0053d970(void)

{
  byte bVar1;
  byte *pbVar2;
  int iVar3;
  int iVar4;
  char *pcVar5;
  bool bVar6;
  
  pcVar5 = s_The_Glory_of_Ptolemy_00601840;
  pbVar2 = &DAT_00784914;
  do {
    bVar1 = *pbVar2;
    bVar6 = bVar1 < (byte)*pcVar5;
    if (bVar1 != *pcVar5) {
LAB_0053d9a4: ;
      iVar3 = (1 - (uint)bVar6) - (uint)(bVar6 != 0);
      goto LAB_0053d9a9;
    }
    if (bVar1 == 0) break;
    bVar1 = pbVar2[1];
    bVar6 = bVar1 < (byte)pcVar5[1];
    if (bVar1 != pcVar5[1]) goto LAB_0053d9a4;
    pbVar2 = pbVar2 + 2;
    pcVar5 = pcVar5 + 2;
  } while (bVar1 != 0);
  iVar3 = 0;
LAB_0053d9a9: ;
  iVar4 = 4;
  if (iVar3 != 0) {
    iVar4 = FUN_0053d8e0();
  }
  _DAT_00f55bac = iVar4;
  if (iVar4 == 2) {
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_OBELISK_EXTRA_006017e0,9);
    return;
  }
  if (iVar4 == 3) {
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_SUN_TEMPLE_EXTRA_006017f0,9);
    return;
  }
  if (iVar4 != 4) {
    FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_SPHINX_EXTRA_005791a8,9);
    return;
  }
  DAT_00e94e18 = FUN_0053d930();
  if (DAT_00e94e18 != 0) {
    if (DAT_00e94e18 == 1) {
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_MAUSOLEUM1_EXTRA_00601804,9);
      return;
    }
    if (DAT_00e94e18 == 2) {
      FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_MAUSOLEUM2_EXTRA_00601818,9);
      return;
    }
  }
  FUN_0053dd90(RESTORE_SPRITE_LIBRARIES,s_MAUSOLEUM0_EXTRA_0060182c,9);
  return;
}
