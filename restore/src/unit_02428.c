#include "restore_types.h"
#define DAT_005c7a80 (*(undefined4 *)0x005c7a80u)
#define DAT_006057cc (*(int32_t *)0x006057ccu)
#define DAT_006057d0 (*(int32_t *)0x006057d0u)
#define DAT_006057d8 (*(int32_t *)0x006057d8u)
#define DAT_006059f4 (*(int32_t *)0x006059f4u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00d3b1c0 (*(undefined1 *)0x00d3b1c0u)
#define DAT_00e37078 (*(undefined4 *)0x00e37078u)
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
extern void FUN_004ce3a0();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d65b0();
extern void FUN_00532830();
extern void FUN_00536820();
extern void FUN_00537140();

void FUN_005325f0(void)

{
  int iVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  int local_10c;
  undefined4 local_108;
  char local_104 [260];
  
  iVar5 = 1;
  DAT_00e38dc9 = 1;
  FUN_00536820(DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x28,0x18,0x15);
  FUN_004d65b0(DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x50,0x14,2,DAT_00e38da8,DAT_00e38da4);
  FUN_004d65b0(DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x78,0x14,0xd,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2b,DAT_00d3b1c0 + 3,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x32,0x130,DAT_00f56fb4,
               0xffff,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2b,5,DAT_00e38da8 + 0xe0,DAT_00e38da4 + 0x156,DAT_00f56f8c,0xf800,DAT_00e38da8,
               DAT_00e38da4);
  local_10c = 0xc;
  if (DAT_006057d0 < 0xd) {
    local_10c = DAT_006057d0;
  }
  if (0 < local_10c) {
    iVar6 = 0x10;
    do {
      if (DAT_007056c8 == iVar5) {
        local_108 = DAT_00f56f9c;
      }
      else {
        local_108 = DAT_00f56f94;
      }
      iVar1 = DAT_006059f4 + -1 + iVar5;
      if ((iVar1 < 0) || (DAT_006057d0 <= iVar1)) {
        pcVar7 = (char *)0x0;
      }
      else {
        pcVar7 = (char *)(*(int *)(DAT_006057cc + iVar1 * 4) + DAT_006057d8);
      }
      uVar3 = 0xffffffff;
      do {
        pcVar8 = pcVar7;
        if (uVar3 == 0) break;
        uVar3 = uVar3 - 1;
        pcVar8 = pcVar7 + 1;
        cVar2 = *pcVar7;
        pcVar7 = pcVar8;
      } while (cVar2 != '\0');
      uVar3 = ~uVar3;
      pcVar7 = pcVar8 + -uVar3;
      pcVar8 = local_104;
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
      FUN_004ce3a0(local_104);
      FUN_004cc770(local_104,DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x72 + iVar6,local_108,0);
      iVar5 = iVar5 + 1;
      iVar6 = iVar6 + 0x10;
    } while (iVar5 <= local_10c);
  }
  FUN_00425730(DAT_00e38da8,DAT_00e38da4,&DAT_005c7a80,4);
  FUN_004cc540();
  FUN_004cc770(&DAT_00e37078,DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x5a,DAT_00f56f9c,0);
  FUN_00537140(DAT_00e38da8 + 0xa0,DAT_00e38da4 + 0x5b,0xffff);
  FUN_00532830();
  return;
}
