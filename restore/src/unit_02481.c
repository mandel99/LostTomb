#include "restore_types.h"
#include "sound_streams.h"
#define DAT_00e38e65 (*(char *)0x00e38e65u)
#define DAT_00e38ecc (*(undefined *)0x00e38eccu)
#define DAT_00f55a20 (*(undefined4 *)0x00f55a20u)
#define DAT_00f55b88 (*(undefined *)0x00f55b88u)
#define s_audio_voice_walker__005f90a4 (*(char (*)[20])0x005f90a4u)

undefined4 FUN_00539a70(char *param_1)

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  char *pcVar8;
  
  if (DAT_00f55b88 == 0) {
    return 0;
  }
  if (DAT_00e38e65 == '\0') {
    return 0;
  }
  uVar3 = 0xffffffff;
  pcVar6 = s_audio_voice_walker__005f90a4;
  do {
    pcVar8 = pcVar6;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar8 = pcVar6 + 1;
    cVar1 = *pcVar6;
    pcVar6 = pcVar8;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  pcVar6 = pcVar8 + -uVar3;
  pcVar8 = (char *)&DAT_00f55a20;
  for (uVar4 = uVar3 >> 2; uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar8 = *pcVar6;
    pcVar6 = pcVar6 + 1;
    pcVar8 = pcVar8 + 1;
  }
  uVar3 = 0xffffffff;
  do {
    pcVar6 = param_1;
    if (uVar3 == 0) break;
    uVar3 = uVar3 - 1;
    pcVar6 = param_1 + 1;
    cVar1 = *param_1;
    param_1 = pcVar6;
  } while (cVar1 != '\0');
  uVar3 = ~uVar3;
  iVar5 = -1;
  pcVar8 = (char *)&DAT_00f55a20;
  do {
    pcVar7 = pcVar8;
    if (iVar5 == 0) break;
    iVar5 = iVar5 + -1;
    pcVar7 = pcVar8 + 1;
    cVar1 = *pcVar8;
    pcVar8 = pcVar7;
  } while (cVar1 != '\0');
  pcVar6 = pcVar6 + -uVar3;
  pcVar8 = pcVar7 + -1;
  for (uVar4 = uVar3 >> 2; uVar2 = DAT_00e38ecc, uVar4 != 0; uVar4 = uVar4 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar6;
    pcVar6 = pcVar6 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar3 = uVar3 & 3; uVar3 != 0; uVar3 = uVar3 - 1) {
    *pcVar8 = *pcVar6;
    pcVar6 = pcVar6 + 1;
    pcVar8 = pcVar8 + 1;
  }
  FUN_0055f639(RESTORE_SOUND,1,uVar2);
  FUN_0055f32f(RESTORE_SOUND,DAT_00e38ecc);
  uVar2 = FUN_0055f449(RESTORE_SOUND,&DAT_00f55a20,1,1);
  return uVar2;
}
