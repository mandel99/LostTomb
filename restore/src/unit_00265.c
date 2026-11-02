#include "restore_types.h"
#include "campaign_profile.h"
#define DAT_007009dc (*(undefined4 *)0x007009dcu)
#define DAT_007009e0 (*(undefined4 *)0x007009e0u)
#define DAT_007009e4 (*(undefined4 *)0x007009e4u)
#define DAT_00700a04 (*(undefined4 *)0x00700a04u)
#define DAT_00700a08 (*(undefined4 *)0x00700a08u)
#define DAT_00700a0c (*(undefined4 *)0x00700a0cu)
#define DAT_00700a10 (*(undefined4 *)0x00700a10u)
#define DAT_00700a14 (*(undefined4 *)0x00700a14u)
#define DAT_00700a18 (*(undefined4 *)0x00700a18u)
#define DAT_00700a1c (*(undefined4 *)0x00700a1cu)
#define DAT_00700a20 (*(undefined4 *)0x00700a20u)
#define DAT_00700a24 (*(undefined4 *)0x00700a24u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5b3c (*(undefined4 *)0x00ea5b3cu)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea6018 (*(undefined4 *)0x00ea6018u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)

extern undefined4 * FUN_00516a20();

void FUN_0041c600(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  char *pcVar9;
  
  iVar2 = DAT_00eaac28;
  iVar7 = DAT_00eaac28 * 0x49d8;
  iVar6 = param_1 * 0x4c;
  *(undefined4 *)(&DAT_00700a04 + iVar6) = (&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276];
  *(undefined4 *)(&DAT_00700a1c + iVar6) = *(undefined4 *)(&DAT_00ea6018 + iVar7);
  *(undefined4 *)(&DAT_00700a14 + iVar6) = *(undefined4 *)(&DAT_00ea187c + iVar7);
  *(undefined4 *)(&DAT_00700a10 + iVar6) = *(undefined4 *)(&DAT_00ea1894 + iVar7);
  *(undefined4 *)(&DAT_00700a0c + iVar6) = (&DAT_00ea5b48)[iVar2 * 0x1276];
  *(int *)(&DAT_007009e0 + iVar6) = param_1;
  *(undefined4 *)(&DAT_00700a24 + iVar6) = 1;
  *(undefined4 *)(&DAT_00700a20 + iVar6) = param_2;
  uVar4 = 0xffffffff;
  pcVar8 = &DAT_00e38e74;
  do {
    pcVar9 = pcVar8;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar9 = pcVar8 + 1;
    cVar1 = *pcVar8;
    pcVar8 = pcVar9;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pcVar8 = pcVar9 + -uVar4;
  pcVar9 = &DAT_007009e4 + iVar6;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar9 = *(undefined4 *)pcVar8;
    pcVar8 = pcVar8 + 4;
    pcVar9 = pcVar9 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar9 = *pcVar8;
    pcVar8 = pcVar8 + 1;
    pcVar9 = pcVar9 + 1;
  }
  *(undefined4 *)(&DAT_00700a08 + iVar6) = (&DAT_00ea5b40)[iVar2 * 0x1276];
  *(undefined4 *)(&DAT_007009dc + iVar6) = param_3;
  *(undefined4 *)(&DAT_00700a18 + iVar6) = param_4;
  uVar3 = FUN_00516a20();
  FUN_0041b8a0(RESTORE_CAMPAIGN_PROFILE,(const char *)(uintptr_t)uVar3);
  return;
}
