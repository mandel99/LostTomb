#include "restore_types.h"
#define DAT_005c6df0 (*(undefined4 *)0x005c6df0u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00784db0 (*(undefined *)0x00784db0u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004cd1a0();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6380();
extern void FUN_00536820();

void FUN_00537260(void)

{
  int iVar1;
  undefined4 uVar2;
  short *psVar3;
  undefined *puVar4;
  int iVar5;
  bool bVar6;
  int local_c;
  
  iVar1 = DAT_00e38da8;
  local_c = DAT_00e38da4;
  FUN_00536820(DAT_00e38da8,DAT_00e38da4,0x15,0xe);
  FUN_004d62f0(0x2c,0xa4,iVar1 + 10,local_c + 10,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,3,iVar1,local_c + 200,0x150,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar5 = local_c + 0x2e;
  FUN_004d62f0(0x2c,0xae,iVar1 + 0x14,iVar5,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0xaf,iVar1 + 0x84,iVar5,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2c,0xb0,iVar1 + 0xc5,iVar5,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar5 = 0;
  local_c = local_c + 0x46;
  psVar3 = &DAT_00784db0;
  puVar4 = &DAT_005c6df0;
  do {
    uVar2 = DAT_00f56f8c;
    bVar6 = DAT_007056c8 == iVar5 + 1;
    FUN_004d62f0(0x9d,iVar5,iVar1 + 0x14,local_c,DAT_00f56f9c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004cd1a0(1,puVar4 + -0x118,bVar6,0,0);
    FUN_004d6380(0xbb,(int)psVar3[-0x267],puVar4 + -0x118,uVar2,0,4,4,DAT_00e38da8,DAT_00e38da4);
    FUN_004cd1a0(1,puVar4,DAT_007056c8 == iVar5 + 0xb,0,0);
    FUN_004d6380(0x12,(int)*psVar3,puVar4,uVar2,0,4,4,DAT_00e38da8,DAT_00e38da4);
    puVar4 = puVar4 + 0x1c;
    iVar5 = iVar5 + 1;
    local_c = local_c + 0x18;
    psVar3 = psVar3 + 1;
  } while ((int)puVar4 < 0x5c6e7c);
  return;
}
