#include "restore_types.h"
#define DAT_00d38708 (*(undefined4 *)0x00d38708u)
#define DAT_00d3870c (*(undefined4 *)0x00d3870cu)
#define DAT_00d38710 (*(undefined4 *)0x00d38710u)
#define DAT_00d38730 (*(undefined4 *)0x00d38730u)
#define DAT_00d38734 (*(undefined4 *)0x00d38734u)
#define DAT_00d38738 (*(undefined4 *)0x00d38738u)
#define DAT_00d3873c (*(undefined4 *)0x00d3873cu)
#define DAT_00d38740 (*(undefined4 *)0x00d38740u)
#define DAT_00d38744 (*(undefined4 *)0x00d38744u)
#define DAT_00d38748 (*(undefined4 *)0x00d38748u)
#define DAT_00d3874c (*(undefined4 *)0x00d3874cu)
#define DAT_00d38750 (*(undefined4 *)0x00d38750u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea5b3c (*(undefined4 *)0x00ea5b3cu)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea6018 (*(undefined4 *)0x00ea6018u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define s_save_highscore_jas_005e02a8 (*(char (*)[19])0x005e02a8u)
extern bool FUN_004c58d0();

void FUN_004c5970(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  
  iVar2 = DAT_00eaac28;
  iVar6 = DAT_00eaac28 * 0x49d8;
  iVar5 = param_1 * 0x4c;
  *(undefined4 *)(&DAT_00d38730 + iVar5) = (&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276];
  *(undefined4 *)(&DAT_00d38748 + iVar5) = *(undefined4 *)(&DAT_00ea6018 + iVar6);
  *(undefined4 *)(&DAT_00d38740 + iVar5) = *(undefined4 *)(&DAT_00ea187c + iVar6);
  *(undefined4 *)(&DAT_00d3873c + iVar5) = *(undefined4 *)(&DAT_00ea1894 + iVar6);
  *(undefined4 *)(&DAT_00d38738 + iVar5) = (&DAT_00ea5b48)[iVar2 * 0x1276];
  *(int *)(&DAT_00d3870c + iVar5) = param_1;
  (&DAT_00d38750)[param_1 * 0x13] = 1;
  *(undefined4 *)(&DAT_00d3874c + iVar5) = param_2;
  uVar3 = 0xffffffff;
  pcVar7 = &DAT_00e38e74;
  do {
    pcVar8 = pcVar7;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar8 = pcVar7 + 1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar8;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  pcVar7 = pcVar8 + -uVar3;
  pcVar8 = &DAT_00d38710 + iVar5;
  for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar8 = *pcVar7;
    pcVar7 = pcVar7 + 1;
    pcVar8 = pcVar8 + 1;
  }
  *(undefined4 *)(&DAT_00d38734 + iVar5) = (&DAT_00ea5b40)[iVar2 * 0x1276];
  *(undefined4 *)(&DAT_00d38708 + iVar5) = param_3;
  *(undefined4 *)(&DAT_00d38744 + iVar5) = param_4;
  FUN_004c58d0(s_save_highscore_jas_005e02a8);
  return;
}
