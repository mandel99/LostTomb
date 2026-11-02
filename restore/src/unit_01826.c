#include "restore_types.h"
#include "mission_loaders.h"
#include "sound_streams.h"
#define DAT_00d3b1b8 (*(undefined1 *)0x00d3b1b8u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00ea184c (*(undefined4 *)0x00ea184cu)
#define DAT_00f55b94 (*(uint32_t *)0x00f55b94u)
extern void FUN_0046a320();
extern void FUN_0046b320();
extern void FUN_0046f030();
extern void FUN_0046fa40();
extern void FUN_004ce2d0();
extern void FUN_004d41e0();
extern void FUN_004d52a0();
extern undefined4 FUN_005647eb();
extern uint FUN_00568eab();

uint32_t FUN_004d5100(const char *param_1)

{
  char cVar1;
  int iVar2;
  uint uVar3;
  uint uVar4;
  char *pcVar5;
  char *pcVar6;
  
  FUN_004bd330(RESTORE_LOAD_SETTINGS);
  FUN_0046fa40();
  FUN_004ce2d0();
  DAT_00d3b1b8 = 0;
  iVar2 = FUN_00568eab(param_1,0x8000);
  if (iVar2 == -1) {
    return 1;
  }
  FUN_0055f561(RESTORE_SOUND,2);
  DAT_00f55b94 = 0;
  FUN_004d41e0(iVar2);
  FUN_005647eb(iVar2);
  DAT_00d3b1b8 = 1;
  FUN_004d52a0();
  FUN_0046b320();
  FUN_0046f030();
  FUN_0046a320();
  uVar3 = 0xffffffff;
  pcVar5 = &DAT_00e38e74;
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
  pcVar6 = (char *)&DAT_00ea184c;
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
  return 0;
}
