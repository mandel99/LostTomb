#include "restore_types.h"
#include "campaign_profile.h"
#define DAT_005761d0 (*(undefined4 *)0x005761d0u)
#define DAT_00605a3c (*(uint32_t *)0x00605a3cu)
#define DAT_00700998 (*(undefined *)0x00700998u)
#define DAT_007027b4 (*(undefined1 *)0x007027b4u)
#define DAT_00e38eec (*(uint32_t *)0x00e38eecu)
#define DAT_00ea5f8c (*(undefined4 *)0x00ea5f8cu)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define s_autosave_history_sav_005e1bfc (*(char (*)[21])0x005e1bfcu)
#define s_autosave_replay_sav_0057bfc4 (*(char (*)[20])0x0057bfc4u)
#define s_autosave_sav_005e1c14 (*(char (*)[13])0x005e1c14u)

extern undefined4 FUN_004d4e20();
extern undefined4 * FUN_005169c0();
extern undefined4 * FUN_00516a20();

void FUN_004d5c30(void)

{
  char cVar1;
  char *pcVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  char *pcVar9;
  char local_64 [100];
  
  if (DAT_00605a3c != 1) {
    iVar6 = (&DAT_00ea5f8c)[DAT_00eaac28 * 0x1276];
    if (((iVar6 != 2) || (DAT_00e38eec == 0)) && (iVar6 != 1)) {
      if (DAT_007027b4 == '\x01') {
        pcVar7 = s_autosave_replay_sav_0057bfc4;
        if (iVar6 != 0) {
          pcVar7 = s_autosave_history_sav_005e1bfc;
        }
      }
      else {
        pcVar7 = s_autosave_sav_005e1c14;
      }
      (&DAT_00ea5f8c)[DAT_00eaac28 * 0x1276] = 1;
      pcVar2 = (char *)FUN_005169c0();
      uVar4 = 0xffffffff;
      do {
        pcVar9 = pcVar2;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar9 = pcVar2 + 1;
        cVar1 = *pcVar2;
        pcVar2 = pcVar9;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      pcVar2 = pcVar9 + -uVar4;
      pcVar9 = local_64;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar2;
        pcVar2 = pcVar2 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar9 = *pcVar2;
        pcVar2 = pcVar2 + 1;
        pcVar9 = pcVar9 + 1;
      }
      uVar4 = 0xffffffff;
      pcVar2 = &DAT_005761d0;
      do {
        pcVar9 = pcVar2;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar9 = pcVar2 + 1;
        cVar1 = *pcVar2;
        pcVar2 = pcVar9;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      iVar6 = -1;
      pcVar2 = local_64;
      do {
        pcVar8 = pcVar2;
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pcVar8 = pcVar2 + 1;
        cVar1 = *pcVar2;
        pcVar2 = pcVar8;
      } while (cVar1 != '\0');
      pcVar2 = pcVar9 + -uVar4;
      pcVar9 = pcVar8 + -1;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar9 = *(undefined4 *)pcVar2;
        pcVar2 = pcVar2 + 4;
        pcVar9 = pcVar9 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar9 = *pcVar2;
        pcVar2 = pcVar2 + 1;
        pcVar9 = pcVar9 + 1;
      }
      uVar4 = 0xffffffff;
      do {
        pcVar2 = pcVar7;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar2 = pcVar7 + 1;
        cVar1 = *pcVar7;
        pcVar7 = pcVar2;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      iVar6 = -1;
      pcVar7 = local_64;
      do {
        pcVar9 = pcVar7;
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pcVar9 = pcVar7 + 1;
        cVar1 = *pcVar7;
        pcVar7 = pcVar9;
      } while (cVar1 != '\0');
      pcVar7 = pcVar2 + -uVar4;
      pcVar2 = pcVar9 + -1;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar2 = *(undefined4 *)pcVar7;
        pcVar7 = pcVar7 + 4;
        pcVar2 = pcVar2 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar2 = *pcVar7;
        pcVar7 = pcVar7 + 1;
        pcVar2 = pcVar2 + 1;
      }
      FUN_004d4e20(local_64);
      if (DAT_007027b4 == '\x01') {
        uVar4 = 0xffffffff;
        pcVar7 = local_64;
        do {
          pcVar2 = pcVar7;
          if (uVar4 == 0) break;
          uVar4 = uVar4 - 1;
          pcVar2 = pcVar7 + 1;
          cVar1 = *pcVar7;
          pcVar7 = pcVar2;
        } while (cVar1 != '\0');
        uVar4 = ~uVar4;
        pcVar7 = pcVar2 + -uVar4;
        pcVar2 = (char *)&DAT_00700998;
        for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
          *(undefined4 *)pcVar2 = *(undefined4 *)pcVar7;
          pcVar7 = pcVar7 + 4;
          pcVar2 = pcVar2 + 4;
        }
        for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
          *pcVar2 = *pcVar7;
          pcVar7 = pcVar7 + 1;
          pcVar2 = pcVar2 + 1;
        }
        uVar3 = FUN_00516a20();
        FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,(const char *)(uintptr_t)uVar3);
      }
    }
  }
  return;
}
