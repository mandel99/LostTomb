#include "restore_types.h"
#define DAT_005c79f0 (*(undefined4 *)0x005c79f0u)
#define DAT_006057cc (*(int32_t *)0x006057ccu)
#define DAT_006057d0 (*(int32_t *)0x006057d0u)
#define DAT_006057d8 (*(int32_t *)0x006057d8u)
#define DAT_006059f4 (*(int32_t *)0x006059f4u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00d3b1b3 (*(char *)0x00d3b1b3u)
#define DAT_00d3b1c0 (*(undefined1 *)0x00d3b1c0u)
#define DAT_00e37100 (*(undefined4 *)0x00e37100u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56f94 (*(int32_t *)0x00f56f94u)
#define DAT_00f56f9c (*(int32_t *)0x00f56f9cu)
#define DAT_00f56fb4 (*(int32_t *)0x00f56fb4u)
extern void FUN_00425730();
extern void FUN_004cc540();
extern void FUN_004cc770();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_00532830();
extern void FUN_00536820();
extern void FUN_00537140();

void FUN_00532340(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  char cVar8;
  undefined4 uVar9;
  int local_10c;
  char local_104 [260];
  
  iVar4 = 1;
  DAT_00e38dc9 = 1;
  FUN_00536820(DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x28,0x18,0x15);
  FUN_004d65b0(DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x50,0x14,2,DAT_00e38da8,DAT_00e38da4);
  FUN_004d65b0(DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x78,0x14,0xd,DAT_00e38da8,DAT_00e38da4);
  FUN_004d65b0(DAT_00e38da8 + 0x1d3,DAT_00e38da4 + 0x90,2,10,DAT_00e38da8,DAT_00e38da4);
  if (DAT_00d3b1b3 == '\0') {
    uVar9 = 0xffff;
    cVar8 = DAT_00d3b1c0;
    if (DAT_00d3b1c0 == '\x02') {
      cVar8 = '\x06';
    }
  }
  else {
    DAT_00d3b1b3 = DAT_00d3b1b3 + -1;
    uVar9 = 0xf800;
    cVar8 = '\x02';
  }
  FUN_004d6250(0x2b,cVar8,DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x32,0x130,DAT_00f56fb4,uVar9,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2b,5,DAT_00e38da8 + 0xe0,DAT_00e38da4 + 0x156,DAT_00f56f8c,0xf800,DAT_00e38da8,
               DAT_00e38da4);
  local_10c = 0xc;
  if (DAT_006057d0 < 0xd) {
    local_10c = DAT_006057d0;
  }
  if (0 < local_10c) {
    iVar5 = 0x10;
    do {
      uVar9 = DAT_00f56f9c;
      if (DAT_007056c8 != iVar4) {
        uVar9 = DAT_00f56f94;
      }
      iVar1 = DAT_006059f4 + -1 + iVar4;
      if ((iVar1 < 0) || (DAT_006057d0 <= iVar1)) {
        pcVar6 = (char *)0x0;
      }
      else {
        pcVar6 = (char *)(*(int *)(DAT_006057cc + iVar1 * 4) + DAT_006057d8);
      }
      uVar2 = 0xffffffff;
      do {
        pcVar7 = pcVar6;
        if (uVar2 == 0) break;
        uVar2 = uVar2 - 1;
        pcVar7 = pcVar6 + 1;
        cVar8 = *pcVar6;
        pcVar6 = pcVar7;
      } while (cVar8 != '\0');
      uVar2 = ~uVar2;
      pcVar6 = pcVar7 + -uVar2;
      pcVar7 = local_104;
      for (uVar3 = uVar2 >> 2; iVar1 = DAT_00e38da8, uVar3 != 0; uVar3 = uVar3 - 1) {
        *(undefined4 *)pcVar7 = *(undefined4 *)pcVar6;
        pcVar6 = pcVar6 + 4;
        pcVar7 = pcVar7 + 4;
      }
      for (uVar2 = uVar2 & 3; uVar2 != 0; uVar2 = uVar2 - 1) {
        *pcVar7 = *pcVar6;
        pcVar6 = pcVar6 + 1;
        pcVar7 = pcVar7 + 1;
      }
      FUN_004cc770(local_104,iVar1 + 0xa0,DAT_00e38da4 + 0x72 + iVar5,uVar9,0);
      iVar4 = iVar4 + 1;
      iVar5 = iVar5 + 0x10;
    } while (iVar4 <= local_10c);
  }
  FUN_00425730(DAT_00e38da8,DAT_00e38da4,&DAT_005c79f0,4);
  FUN_004cc540();
  FUN_004cc770(&DAT_00e37100,DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x5a,DAT_00f56f9c,0);
  FUN_00537140(DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x5b,0xffff);
  FUN_00532830();
  return;
}
