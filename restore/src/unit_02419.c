#include "restore_types.h"
#define DAT_005c5e20 (*(undefined4 *)0x005c5e20u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00d38360 (*(undefined *)0x00d38360u)
#define DAT_00d38364 (*(undefined *)0x00d38364u)
#define DAT_00d38368 (*(undefined *)0x00d38368u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004cd1a0();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6380();
extern void FUN_00536820();

void FUN_00530780(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  undefined *puVar4;
  int local_4;
  
  iVar2 = DAT_00e38da8;
  iVar3 = DAT_00e38da4;
  iVar1 = DAT_00e38da4 + 0x50;
  FUN_00536820(DAT_00e38da8,iVar1,0x1e,0x12);
  FUN_004d6250(0x2c,0xb2,iVar2,iVar3 + 100,0x1e0,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,3,iVar2,iVar3 + 0x138,0x1e0,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  puVar4 = &DAT_005c5e20;
  local_4 = 0;
  do {
    if (local_4 == 0) {
      iVar3 = DAT_00d38360 / 0xf;
      FUN_004d62f0(0x2c,0xb3,iVar2 + 0x3c,*(short *)(puVar4 + 2) + 8 + iVar1,DAT_00f56f94,0,
                   DAT_00e38da8,DAT_00e38da4);
      iVar3 = iVar3 + 0xaa;
LAB_00530963: ;
      FUN_004d6380(0x2c,iVar3,puVar4,DAT_00f56f8c,0,0,0x58,DAT_00e38da8,DAT_00e38da4);
    }
    else {
      if (local_4 == 1) {
        iVar3 = DAT_00d38364 / 0x1e;
        FUN_004d62f0(0x2c,0xbc,iVar2 + 0x3c,*(short *)(puVar4 + 2) + 8 + iVar1,DAT_00f56f94,0,
                     DAT_00e38da8,DAT_00e38da4);
        iVar3 = iVar3 + 0xbb;
        goto LAB_00530963;
      }
      if (local_4 == 2) {
        iVar3 = DAT_00d38368 / 0x14;
        FUN_004d62f0(0x2c,0xc0,iVar2 + 0x3c,*(short *)(puVar4 + 2) + 8 + iVar1,DAT_00f56f94,0,
                     DAT_00e38da8,DAT_00e38da4);
        iVar3 = iVar3 + 0xc1;
        goto LAB_00530963;
      }
    }
    local_4 = local_4 + 1;
    FUN_004cd1a0(1,puVar4,DAT_007056c8 == local_4,0,0x50);
    puVar4 = puVar4 + 0x1c;
    if (2 < local_4) {
      return;
    }
  } while( true );
}
