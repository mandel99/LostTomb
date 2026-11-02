#include "restore_types.h"
#define DAT_005c92a0 (*(undefined4 *)0x005c92a0u)
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
extern undefined4 FUN_004265d0();
extern undefined4 FUN_00561a36();

void FUN_00426810(char *param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  UINT UVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  CHAR local_8 [8];
  
  uVar4 = 0xffffffff;
  pcVar7 = &DAT_007027d4;
  do {
    pcVar8 = pcVar7;
    if (uVar4 == 0) break;
    uVar4 = uVar4 - 1;
    pcVar8 = pcVar7 + 1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar8;
  } while (cVar1 != '\0');
  uVar4 = ~uVar4;
  pcVar7 = pcVar8 + -uVar4;
  pcVar8 = param_1;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *pcVar8 = *pcVar7;
    pcVar7 = pcVar7 + 1;
    pcVar8 = pcVar8 + 1;
  }
  iVar6 = 0x41;
  do {
    FUN_00561a36(local_8,&DAT_005c92a0,iVar6);
    UVar2 = GetDriveTypeA(local_8);
    if (UVar2 == 5) {
      iVar3 = FUN_004265d0(local_8,param_2,param_3);
      if (iVar3 == 7) {
        uVar4 = 0xffffffff;
        pcVar7 = local_8;
        break;
      }
    }
    iVar6 = iVar6 + 1;
    if (0x5a < iVar6) {
      return;
    }
  } while( true );
  while( true ) {
    uVar4 = uVar4 - 1;
    pcVar8 = pcVar7 + 1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar8;
    if (cVar1 == '\0') break;
    pcVar8 = pcVar7;
    if (uVar4 == 0) break;
  }
  uVar4 = ~uVar4;
  pcVar7 = pcVar8 + -uVar4;
  for (uVar5 = uVar4 >> 2; uVar5 != 0; uVar5 = uVar5 - 1) {
    *(undefined4 *)param_1 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    param_1 = param_1 + 4;
  }
  for (uVar4 = uVar4 & 3; uVar4 != 0; uVar4 = uVar4 - 1) {
    *param_1 = *pcVar7;
    pcVar7 = pcVar7 + 1;
    param_1 = param_1 + 1;
  }
  return;
}
