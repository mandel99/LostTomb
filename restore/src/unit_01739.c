#include "restore_types.h"
#include "file_list.h"
#define DAT_005761c4 (*(undefined4 *)0x005761c4u)
#define DAT_006057cc (*(int32_t *)0x006057ccu)
#define DAT_006057d0 (*(int32_t *)0x006057d0u)
#define DAT_006057d8 (*(int32_t *)0x006057d8u)
#define DAT_006059ec (*(int32_t *)0x006059ecu)
#define DAT_006059f0 (*(int32_t *)0x006059f0u)
#define DAT_006059f4 (*(int32_t *)0x006059f4u)
#define DAT_006059f8 (*(int32_t *)0x006059f8u)
#define DAT_00d3b1b3 (*(char *)0x00d3b1b3u)
#define _DAT_00e36efc (*(undefined4 *)0x00e36efcu)
#define s_maps___map_00576260 (*(char (*)[11])0x00576260u)
extern void FUN_004ce3a0();
extern undefined4 * FUN_005169c0();
extern int FUN_00562370();

void FUN_004cdff0(undefined4 param_1,char param_2)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  char local_104 [260];
  
  if (param_2 == '\0') {
    pcVar2 = (char *)FUN_005169c0();
    uVar4 = 0xffffffff;
    do {
      pcVar8 = pcVar2;
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      pcVar8 = pcVar2 + 1;
      cVar1 = *pcVar2;
      pcVar2 = pcVar8;
    } while (cVar1 != '\0');
    uVar4 = ~uVar4;
    pcVar2 = pcVar8 + -uVar4;
    pcVar8 = local_104;
    for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(undefined4 *)pcVar8 = *(undefined4 *)pcVar2;
      pcVar2 = pcVar2 + 4;
      pcVar8 = pcVar8 + 4;
    }
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *pcVar8 = *pcVar2;
      pcVar2 = pcVar2 + 1;
      pcVar8 = pcVar8 + 1;
    }
    uVar4 = 0xffffffff;
    pcVar2 = &DAT_005761c4;
    do {
      pcVar8 = pcVar2;
      if (uVar4 == 0) break;
      uVar4 = uVar4 - 1;
      pcVar8 = pcVar2 + 1;
      cVar1 = *pcVar2;
      pcVar2 = pcVar8;
    } while (cVar1 != '\0');
    uVar4 = ~uVar4;
    iVar6 = -1;
    pcVar2 = local_104;
    do {
      pcVar7 = pcVar2;
      if (iVar6 == 0) break;
      iVar6 = iVar6 + -1;
      pcVar7 = pcVar2 + 1;
      cVar1 = *pcVar2;
      pcVar2 = pcVar7;
    } while (cVar1 != '\0');
    pcVar2 = pcVar8 + -uVar4;
    pcVar8 = pcVar7 + -1;
    for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
      *(undefined4 *)pcVar8 = *(undefined4 *)pcVar2;
      pcVar2 = pcVar2 + 4;
      pcVar8 = pcVar8 + 4;
    }
    for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
      *pcVar8 = *pcVar2;
      pcVar2 = pcVar2 + 1;
      pcVar8 = pcVar8 + 1;
    }
    FUN_0041c740(RESTORE_SAVE_FILES,local_104,0);
  }
  else {
    FUN_0041c740(RESTORE_SAVE_FILES,s_maps___map_00576260,0);
  }
  iVar6 = 0;
  DAT_006059f8 = DAT_006057d0;
  if (0 < DAT_006057d0) {
    do {
      if ((iVar6 < 0) || (DAT_006057d0 <= iVar6)) {
        iVar3 = 0;
      }
      else {
        iVar3 = *(int *)(DAT_006057cc + iVar6 * 4) + DAT_006057d8;
      }
      FUN_004ce3a0(iVar3);
      iVar6 = iVar6 + 1;
    } while (iVar6 < DAT_006059f8);
  }
  FUN_0041ca10(RESTORE_SAVE_FILES,(RestoreFileCompare)FUN_00562370);
  DAT_006059f4 = 0;
  DAT_006059f0 = 0;
  DAT_00d3b1b3 = 0;
  DAT_006059ec = 0;
  _DAT_00e36efc = 0;
  return;
}
