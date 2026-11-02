#include "restore_types.h"
#define s_Immigrant_e01_wav_005fb16c (*(char (*)[18])0x005fb16cu)
#define s_zookeeper_e01_wav_0060110c (*(char (*)[18])0x0060110cu)
extern undefined4 FUN_00539a70();

void FUN_0053bfe0(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  char *pcVar6;
  char local_20 [32];
  
  if ((0 < param_1) && (-1 < param_2)) {
    pcVar5 = local_20;
    for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
      pcVar5[0] = '\0';
      pcVar5[1] = '\0';
      pcVar5[2] = '\0';
      pcVar5[3] = '\0';
      pcVar5 = pcVar5 + 4;
    }
    if (param_1 == 0x6c) {
      pcVar5 = s_zookeeper_e01_wav_0060110c + param_2 * 0x18;
    }
    else {
      pcVar5 = s_Immigrant_e01_wav_005fb16c + (param_2 + -0x14 + param_1 * 0x14) * 0x18;
    }
    uVar3 = 0xffffffff;
    do {
      pcVar6 = pcVar5;
      if (uVar3 == 0) break;
      uVar3 = uVar3 - 1;
      pcVar6 = pcVar5 + 1;
      cVar1 = *pcVar5;
      pcVar5 = pcVar6;
    } while (cVar1 != '\0');
    uVar3 = ~uVar3;
    pcVar5 = pcVar6 + -uVar3;
    pcVar6 = local_20;
    for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
      *(undefined4 *)pcVar6 = *(undefined4 *)pcVar5;
      pcVar5 = pcVar5 + 4;
      pcVar6 = pcVar6 + 4;
    }
    for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
      *pcVar6 = *pcVar5;
      pcVar5 = pcVar5 + 1;
      pcVar6 = pcVar6 + 1;
    }
    FUN_00539a70(local_20);
  }
  return;
}
