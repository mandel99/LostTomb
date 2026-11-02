#include "restore_types.h"
#define DAT_005e0400 (*(undefined4 *)0x005e0400u)
#define DAT_00d3a6cc (*(undefined *)0x00d3a6ccu)
#define DAT_00d3a6d0 (*(undefined *)0x00d3a6d0u)
#define DAT_00d3a6d4 (*(undefined *)0x00d3a6d4u)
#define DAT_00d3a6d8 (*(undefined *)0x00d3a6d8u)
#define DAT_00d3b51c (*(undefined *)0x00d3b51cu)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define DAT_00e92dfc (*(undefined *)0x00e92dfcu)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
extern void FUN_004cc770();
extern void FUN_004ccbb0();
extern void FUN_004cd680();
extern void FUN_004cd9d0();
extern int FUN_00559fc0();

void FUN_004cb300(void)

{
  undefined2 uVar1;
  int iVar2;
  undefined4 uVar3;
  int *piVar4;
  undefined4 uVar5;
  int iVar6;
  int local_40 [16];
  
  local_40[0] = 0;
  local_40[1] = 0;
  local_40[2] = 0;
  local_40[3] = 0;
  local_40[4] = 0;
  local_40[5] = 0;
  local_40[6] = 0;
  local_40[7] = 0;
  local_40[8] = 0;
  local_40[9] = 0;
  local_40[10] = 0;
  local_40[0xb] = 0;
  local_40[0xc] = 0;
  local_40[0xd] = 0;
  local_40[0xe] = 0;
  local_40[0xf] = 0;
  iVar2 = FUN_00559fc0(local_40);
  uVar1 = DAT_00d3b51c;
  DAT_00d3a6cc = 0x14;
  DAT_00d3a6d0 = iVar2 * 0x3c + 0x32;
  if (DAT_00e38e34 < iVar2 * 0x3c + 0x46) {
    DAT_00d3a6d8 = DAT_00e38e34 + 0x14;
  }
  else {
    DAT_00d3a6d8 = (DAT_00e38e34 - DAT_00d3a6d0) + -0x14;
  }
  DAT_00d3a6d4 = DAT_00e38e30 + 10;
  if (199 < DAT_00e38e30) {
    DAT_00d3a6d4 = DAT_00e38e30 + -0x20;
  }
  if (DAT_00e92dfc == 2) {
    DAT_00d3b51c = 0x3922;
    uVar5 = 0xbb88;
    uVar3 = 0x58a0;
  }
  else {
    DAT_00d3b51c = 0x1c82;
    uVar5 = 0x5dc8;
    uVar3 = 0x2c40;
  }
  FUN_004cd680(DAT_00d3a6d8,DAT_00d3a6d4,DAT_00d3a6d0,0x14,uVar3);
  FUN_004cd9d0(DAT_00d3a6d8 + 1,DAT_00d3a6d4 + 1,DAT_00d3a6d0 + -2,DAT_00d3a6cc + -2,uVar5);
  iVar2 = 0;
  DAT_00e20778 = 0;
  iVar6 = 0;
  piVar4 = local_40;
  do {
    if (*piVar4 != 0) {
      if (iVar2 != 0) {
        FUN_004cc770(&DAT_005e0400,DAT_00e20778 + 5 + DAT_00d3a6d8,DAT_00d3a6d4 + 5,DAT_00f56f84,
                     (int)DAT_00d3b51c);
      }
      FUN_004ccbb0(0x17,iVar6 + 0x27,DAT_00e20778 + 5 + DAT_00d3a6d8,DAT_00d3a6d4 + 5,DAT_00f56f84,
                   (int)DAT_00d3b51c);
      iVar2 = iVar2 + 1;
    }
    iVar6 = iVar6 + 1;
    piVar4 = piVar4 + 1;
  } while (iVar6 < 0x10);
  DAT_00d3b51c = uVar1;
  return;
}
