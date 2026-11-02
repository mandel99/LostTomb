#include "restore_types.h"
#include "campaign_profile.h"
#define DAT_00d3b198 (*(int32_t *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00eadfac (*(int32_t *)0x00eadfacu)
#define DAT_00eadfb0 (*(int32_t *)0x00eadfb0u)
#define DAT_00eadfb8 (*(int32_t *)0x00eadfb8u)
#define DAT_00eadfc8 (*(int32_t *)0x00eadfc8u)
#define DAT_00eadfcc (*(int32_t *)0x00eadfccu)

extern void FUN_004ce3a0();
extern char *FUN_00516a20(void);

void FUN_00516ab0(void)

{
  char cVar1;
  char *profile_path;
  int iVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  undefined4 *puVar7;
  char *pcVar8;
  
  if (DAT_00d3b198 < DAT_00eadfb0) {
    DAT_00eadfcc = DAT_00d3b198 + DAT_00eadfc8;
    if (((-1 < DAT_00eadfcc) && (DAT_00eadfcc < DAT_00eadfb0)) &&
       (pcVar6 = (char *)(*(int *)(DAT_00eadfac + DAT_00eadfcc * 4) + DAT_00eadfb8),
       pcVar6 != (char *)0x0)) {
      puVar7 = (undefined4 *)&DAT_00e38e74;
      for (iVar3 = 8; iVar3 != 0; iVar3 = iVar3 + -1) {
        *puVar7 = 0;
        puVar7 = puVar7 + 1;
      }
      uVar4 = 0xffffffff;
      do {
        pcVar8 = pcVar6;
        if (uVar4 == 0) break;
        uVar4 = uVar4 - 1;
        pcVar8 = pcVar6 + 1;
        cVar1 = *pcVar6;
        pcVar6 = pcVar8;
      } while (cVar1 != '\0');
      uVar4 = ~uVar4;
      pcVar6 = pcVar8 + -uVar4;
      pcVar8 = &DAT_00e38e74;
      for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(undefined4 *)pcVar8 = *(undefined4 *)pcVar6;
        pcVar6 = pcVar6 + 4;
        pcVar8 = pcVar8 + 4;
      }
      for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
        *pcVar8 = *pcVar6;
        pcVar6 = pcVar6 + 1;
        pcVar8 = pcVar8 + 1;
      }
      FUN_004ce3a0(&DAT_00e38e74);
      profile_path = FUN_00516a20();
      FUN_0041b680(RESTORE_CAMPAIGN_PROFILE,profile_path);
      DAT_00e38dca = 1;
    }
  }
  return;
}
