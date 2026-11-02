#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_005e03fc (*(undefined4 *)0x005e03fcu)
#define DAT_00d3a6cc (*(undefined *)0x00d3a6ccu)
#define DAT_00d3a6d0 (*(undefined *)0x00d3a6d0u)
#define DAT_00d3a6d4 (*(undefined *)0x00d3a6d4u)
#define DAT_00d3a6d8 (*(undefined *)0x00d3a6d8u)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define DAT_00e92dfc (*(undefined *)0x00e92dfcu)
#define DAT_00ea1878 (*(undefined4 *)0x00ea1878u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea5b3c (*(undefined4 *)0x00ea5b3cu)
#define DAT_00ea5b40 (*(undefined4 *)0x00ea5b40u)
#define DAT_00ea5b44 (*(undefined4 *)0x00ea5b44u)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
extern void FUN_004ccbb0();
extern void FUN_004cce00();
extern void FUN_004cd680();
extern void FUN_004cd9d0();

void FUN_004cb0b0(void)

{
  undefined *puVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 local_54;
  undefined4 local_50;
  undefined **local_4c;
  undefined *local_48;
  undefined *local_44;
  undefined *local_40;
  undefined *local_3c;
  undefined *local_38;
  undefined *local_34;
  undefined4 local_30 [6];
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  uVar2 = DAT_00f56f84;
  DAT_00d3a6d0 = 0xb4;
  DAT_00d3a6cc = 0x78;
  if (DAT_00e38e34 < 200) {
    DAT_00d3a6d8 = DAT_00e38e34 + 0x14;
  }
  else {
    DAT_00d3a6d8 = DAT_00e38e34 + -200;
  }
  DAT_00d3a6d4 = DAT_00e38e30 + 10;
  if (199 < DAT_00e38e30) {
    DAT_00d3a6d4 = DAT_00e38e30 + -0x20;
  }
  if (DAT_00e92dfc == 2) {
    local_54 = 0x3922;
    local_50 = 0xff3a;
    uVar5 = 0xbb88;
    uVar3 = 0x58a0;
  }
  else {
    local_54 = 0x1c82;
    local_50 = 0x7f9a;
    uVar5 = 0x5dc8;
    uVar3 = 0x2c40;
  }
  local_30[1] = (&DAT_00ea446c)[DAT_00eaac28 * 0x1276];
  local_30[3] = (&DAT_00ea5b3c)[DAT_00eaac28 * 0x1276];
  local_30[5] = (&DAT_00ea5b40)[DAT_00eaac28 * 0x1276];
  local_14 = (&DAT_00ea5b44)[DAT_00eaac28 * 0x1276];
  local_c = (&DAT_00ea5b48)[DAT_00eaac28 * 0x1276];
  local_4 = *(undefined4 *)(&DAT_00ea1878 + DAT_00eaac28 * 0x49d8);
  local_30[0] = 0x87;
  local_30[2] = 0x88;
  local_30[4] = 0x89;
  local_18 = 0x8a;
  local_10 = 0x8b;
  local_8 = 0x8e;
  local_48 = &DAT_005e03fc;
  local_44 = &DAT_0057c048;
  local_40 = &DAT_0057c048;
  local_3c = &DAT_0057c048;
  local_38 = &DAT_0057c048;
  local_34 = &DAT_005e03fc;
  FUN_004cd680(DAT_00d3a6d8,DAT_00d3a6d4,0xb4,0x78,uVar3);
  FUN_004cd9d0(DAT_00d3a6d8 + 1,DAT_00d3a6d4 + 1,DAT_00d3a6d0 + -2,DAT_00d3a6cc + -2,uVar5);
  local_4c = &local_48;
  iVar6 = 0;
  puVar4 = local_30 + 1;
  do {
    uVar3 = puVar4[-1];
    FUN_004ccbb0(0x44,uVar3,DAT_00d3a6d8 + 5,iVar6 + 5 + DAT_00d3a6d4,uVar2,local_54);
    FUN_004ccbb0(0x44,uVar3,DAT_00d3a6d8 + 4,iVar6 + 4 + DAT_00d3a6d4,uVar2,local_50);
    puVar1 = *local_4c;
    FUN_004cce00(*puVar4,0x40,puVar1,DAT_00d3a6d8 + 0x8c,iVar6 + 5 + DAT_00d3a6d4,uVar2,local_54);
    FUN_004cce00(*puVar4,0x40,puVar1,DAT_00d3a6d8 + 0x8b,iVar6 + 4 + DAT_00d3a6d4,uVar2,local_50);
    iVar6 = iVar6 + 0xe;
    local_4c = local_4c + 1;
    puVar4 = puVar4 + 2;
  } while (iVar6 < 0x54);
  return;
}
