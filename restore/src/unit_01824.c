#include "restore_types.h"
#define DAT_005761c4 (*(undefined4 *)0x005761c4u)
extern void FUN_004cdff0();
extern undefined4 * FUN_005169c0();
extern undefined4 FUN_00562bb5();

undefined4 FUN_004d4d80(undefined4 param_1)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  uint uVar4;
  uint uVar5;
  char *pcVar6;
  char *pcVar7;
  char local_104 [260];
  
  iVar2 = FUN_00562bb5(param_1);
  if (iVar2 == -1) {
    return 1;
  }
  pcVar3 = (char *)FUN_005169c0();
  uVar4 = 0xffffffff;
  do {
    pcVar7 = pcVar3;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar7 = pcVar3 + 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar7;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pcVar3 = pcVar7 + -uVar4;
  pcVar7 = local_104;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar7 = *(undefined4 *)pcVar3;
    pcVar3 = pcVar3 + 4;
    pcVar7 = pcVar7 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar7 = *pcVar3;
    pcVar3 = pcVar3 + 1;
    pcVar7 = pcVar7 + 1;
  }
  uVar4 = 0xffffffff;
  pcVar3 = &DAT_005761c4;
  do {
    pcVar7 = pcVar3;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar7 = pcVar3 + 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar7;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  iVar2 = -1;
  pcVar3 = local_104;
  do {
    pcVar6 = pcVar3;
    if (iVar2 == 0) break;
    iVar2 = iVar2 + -1;
    pcVar6 = pcVar3 + 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar6;
  } while (cVar1 != '\0');
  pcVar3 = pcVar7 + -uVar4;
  pcVar7 = pcVar6 + -1;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar7 = *(undefined4 *)pcVar3;
    pcVar3 = pcVar3 + 4;
    pcVar7 = pcVar7 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar7 = *pcVar3;
    pcVar3 = pcVar3 + 1;
    pcVar7 = pcVar7 + 1;
  }
  FUN_004cdff0(local_104,0);
  return 0;
}
