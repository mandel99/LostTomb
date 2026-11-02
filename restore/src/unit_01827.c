#include "restore_types.h"
#include "mission_loaders.h"
#include "sound_streams.h"
#define DAT_005d1960 (*(undefined4 *)0x005d1960u)
#define DAT_00d3b1b8 (*(undefined1 *)0x00d3b1b8u)
#define DAT_00e38e74 (*(char *)0x00e38e74u)
#define DAT_00e39080 (*(undefined *)0x00e39080u)
#define DAT_00ea184c (*(undefined4 *)0x00ea184cu)
#define DAT_00ea187c (*(undefined4 *)0x00ea187cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f55b94 (*(undefined *)0x00f55b94u)
extern void FUN_0046a320();
extern void FUN_0046b320();
extern void FUN_0046f030();
extern void FUN_0046fa40();
extern int FUN_004cde40();
extern void FUN_004ce2d0();
extern void FUN_004d41e0();
extern void FUN_004d52a0();
extern undefined4 FUN_005647eb();
extern uint FUN_00568eab();

undefined4 FUN_004d51b0(undefined4 param_1)

{
  char cVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  uint uVar5;
  uint uVar6;
  char *pcVar7;
  char *pcVar8;
  
  iVar2 = DAT_00e39080;
  FUN_004bd330(RESTORE_LOAD_SETTINGS);
  FUN_0046fa40();
  FUN_004ce2d0();
  DAT_00d3b1b8 = 0;
  iVar3 = FUN_00568eab(param_1,0x8000);
  if (iVar3 == -1) {
    return 1;
  }
  FUN_0055f561(RESTORE_SOUND,2);
  DAT_00f55b94 = 0;
  FUN_004d41e0(iVar3);
  FUN_005647eb(iVar3);
  DAT_00e39080 = iVar2;
  uVar4 = FUN_004cde40(*(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8),
                       *(undefined4 *)(&DAT_005d1960 + iVar2 * 4));
  DAT_00d3b1b8 = 1;
  *(undefined4 *)(&DAT_00ea187c + DAT_00eaac28 * 0x49d8) = uVar4;
  FUN_004d52a0();
  FUN_0046b320();
  FUN_0046f030();
  FUN_0046a320();
  uVar5 = 0xffffffff;
  pcVar7 = &DAT_00e38e74;
  do {
    pcVar8 = pcVar7;
    if (uVar5 == 0) break;
    uVar5 = uVar5 - 1;
    pcVar8 = pcVar7 + 1;
    cVar1 = *pcVar7;
    pcVar7 = pcVar8;
  } while (cVar1 != '\0');
  uVar5 = ~uVar5;
  pcVar7 = pcVar8 + -uVar5;
  pcVar8 = (char *)&DAT_00ea184c;
  for (uVar6 = uVar5 >> 2; uVar6 != 0; uVar6 = uVar6 - 1) {
    *(undefined4 *)pcVar8 = *(undefined4 *)pcVar7;
    pcVar7 = pcVar7 + 4;
    pcVar8 = pcVar8 + 4;
  }
  for (uVar5 = uVar5 & 3; uVar5 != 0; uVar5 = uVar5 - 1) {
    *pcVar8 = *pcVar7;
    pcVar7 = pcVar7 + 1;
    pcVar8 = pcVar8 + 1;
  }
  return 0;
}
