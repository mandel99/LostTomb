#include "restore_types.h"
#define DAT_005761c4 (*(undefined4 *)0x005761c4u)
#define DAT_006059e8 (*(int32_t *)0x006059e8u)
#define DAT_00d3b1c0 (*(undefined1 *)0x00d3b1c0u)
#define DAT_00e20998 (*(int32_t *)0x00e20998u)
#define DAT_00e370bc (*(undefined4 *)0x00e370bcu)
#define DAT_00e37100 (*(undefined4 *)0x00e37100u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define _DAT_00d3b1bc (*(undefined4 *)0x00d3b1bcu)
extern void FUN_0040f8f0();
extern void FUN_004cc3c0();
extern void FUN_004cc3e0();
extern void FUN_004cc500();
extern void FUN_004cdff0();
extern undefined4 * FUN_005169c0();

void FUN_00402450(void)

{
  char cVar1;
  char *pcVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  char local_104 [260];
  
  DAT_006059e8 = 0;
  pcVar2 = (char *)FUN_005169c0();
  uVar3 = 0xffffffff;
  do {
    pcVar7 = pcVar2;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar7 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar7;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  pcVar2 = pcVar7 + -uVar3;
  pcVar7 = local_104;
  for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar7 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar7 = pcVar7 + 4;
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar7 = *pcVar2;
    pcVar2 = pcVar2 + 1;
    pcVar7 = pcVar7 + 1;
  }
  uVar3 = 0xffffffff;
  pcVar2 = &DAT_005761c4;
  do {
    pcVar7 = pcVar2;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar7 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar7;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  iVar5 = -1;
  pcVar2 = local_104;
  do {
    pcVar6 = pcVar2;
    if (iVar5 == 0) break;
    iVar5 = iVar5 + -1;
    pcVar6 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar6;
  } while (cVar1 != '\0');
  pcVar2 = pcVar7 + -uVar3;
  pcVar7 = pcVar6 + -1;
  for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar7 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar7 = pcVar7 + 4;
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar7 = *pcVar2;
    pcVar2 = pcVar2 + 1;
    pcVar7 = pcVar7 + 1;
  }
  FUN_004cdff0(local_104,0);
  DAT_00e20998 = 0;
  DAT_00e38dda = 0xd;
  _DAT_00d3b1bc = 1;
  DAT_00d3b1c0 = 1;
  FUN_0040f8f0(&DAT_00e37100,0x40,0);
  uVar3 = 0xffffffff;
  pcVar2 = &DAT_00e370bc;
  do {
    pcVar7 = pcVar2;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar7 = pcVar2 + 1;
    cVar1 = *pcVar2;
    pcVar2 = pcVar7;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  pcVar2 = pcVar7 + -uVar3;
  pcVar7 = (char *)&DAT_00e37100;
  for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar7 = *(undefined4 *)pcVar2;
    pcVar2 = pcVar2 + 4;
    pcVar7 = pcVar7 + 4;
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar7 = *pcVar2;
    pcVar2 = pcVar2 + 1;
    pcVar7 = pcVar7 + 1;
  }
  FUN_004cc500(2);
  FUN_004cc3c0();
  FUN_004cc3e0();
  DAT_00e38dca = 1;
  return;
}
