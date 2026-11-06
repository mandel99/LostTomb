#include "restore_types.h"
#define DAT_005cf594 (*(undefined4 *)0x005cf594u)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00784ce0 (*(undefined4 *)0x00784ce0u)
#define DAT_00784cfc (*(undefined *)0x00784cfcu)
#define DAT_00784d00 (*(undefined *)0x00784d00u)
#define DAT_00784d02 (*(undefined *)0x00784d02u)
#define DAT_00784d0c (*(undefined *)0x00784d0cu)
#define DAT_00784d28 (*(undefined *)0x00784d28u)
#define DAT_00784d2a (*(undefined *)0x00784d2au)
#define DAT_00784d2c (*(undefined *)0x00784d2cu)
#define DAT_00784d2e (*(undefined *)0x00784d2eu)
#define DAT_00784d30 (*(undefined *)0x00784d30u)
#define DAT_00784d32 (*(undefined *)0x00784d32u)
#define DAT_00784d34 (*(undefined *)0x00784d34u)
#define DAT_00784d3a (*(undefined *)0x00784d3au)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004264d0();
extern undefined4 FUN_00426b20();
extern undefined4 FUN_00442870();
extern void FUN_004cd680();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_0051f780();
extern void FUN_00536820();

void FUN_00532fe0(void)

{
  int iVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  undefined4 uVar8;
  int *local_c;
  int local_8;
  
  iVar6 = DAT_00e38da8;
  iVar4 = DAT_00e38da4;
  iVar1 = DAT_00e38da8 + 0x10;
  iVar2 = DAT_00e38da4 + 0x20;
  if (((DAT_00784d0c != 0) && (DAT_00e38dda != '?')) && (cVar3 = FUN_00442870(0x11), cVar3 == '\0'))
  {
    DAT_00784d0c = 0;
    FUN_004264d0(0x36,0x0051f780u,1);
  }
  if (((DAT_00784cfc != 0) && (DAT_00e38dda != '?')) && (cVar3 = FUN_00442870(0xf), cVar3 == '\0'))
  {
    DAT_00784cfc = 0;
    FUN_004264d0(0x38,0x0051f780u,1);
  }
  if (((DAT_00784d3a != 0) && (DAT_00e38dda != '?')) &&
     ((cVar3 = FUN_00442870(8), cVar3 == '\0' || (cVar3 = FUN_00442870(9), cVar3 == '\0')))) {
    DAT_00784d3a = 0;
    FUN_004264d0(0x8b,0x0051f780u,1);
  }
  if (((DAT_00784d00 != 0) && (DAT_00e38dda != '?')) && (cVar3 = FUN_00442870(0x17), cVar3 == '\0'))
  {
    DAT_00784d00 = 0;
    FUN_004264d0(0x3a,0x0051f780u,1);
  }
  if (((DAT_00784d02 != 0) && (DAT_00e38dda != '?')) && (cVar3 = FUN_00442870(0x17), cVar3 == '\0'))
  {
    DAT_00784d02 = 0;
    FUN_004264d0(0x3c,0x0051f780u,1);
  }
  if (((DAT_00784d32 != 0) && (DAT_00e38dda != '?')) && (cVar3 = FUN_00442870(0x1d), cVar3 == '\0'))
  {
    DAT_00784d32 = 0;
    FUN_004264d0(0x40,0x0051f780u,1);
  }
  if (((DAT_00784d34 != 0) && (DAT_00e38dda != '?')) && (cVar3 = FUN_00442870(0x1c), cVar3 == '\0'))
  {
    DAT_00784d34 = 0;
    FUN_004264d0(0x44,0x0051f780u,1);
  }
  if ((DAT_00784d2a != 0) && (DAT_00e38dda != '?')) {
    if (DAT_00784d28 == 0) {
      DAT_00784d2a = 0;
      FUN_004264d0(0x48,0x0051f780u,1);
      return;
    }
    if ((DAT_00784d32 == 0) && (cVar3 = FUN_00442870(10), cVar3 == '\0')) {
      DAT_00784d2a = 0;
      FUN_004264d0(0x54,0x0051f780u,1);
      DAT_00e38dca = 1;
      return;
    }
  }
  if (((DAT_00784d2c != 0) && (DAT_00e38dda != '?')) && (DAT_00784d28 == 0)) {
    DAT_00784d2c = 0;
    FUN_004264d0(0x4a,0x0051f780u,1);
    return;
  }
  if ((DAT_00784d2e != 0) && (DAT_00e38dda != '?')) {
    if (DAT_00784d28 == 0) {
      DAT_00784d2e = 0;
      FUN_004264d0(0x4c,0x0051f780u,1);
      return;
    }
    if ((DAT_00784d34 == 0) && (cVar3 = FUN_00442870(0x1c), cVar3 == '\0')) {
      DAT_00784d2e = 0;
      FUN_004264d0(0x56,0x0051f780u,1);
      DAT_00e38dca = 1;
      return;
    }
  }
  if (((DAT_00784d30 != 0) && (DAT_00e38dda != '?')) && (DAT_00784d28 == 0)) {
    DAT_00784d30 = 0;
    FUN_004264d0(0x4e,0x0051f780u,1);
  }
  FUN_00536820(iVar1,iVar2,0x26,0x1a);
  FUN_004d62f0(0x2c,0x2f,iVar6 + 0x1a,iVar4 + 0x2a,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0xd,3,iVar1,iVar4 + 0x1a8,0x260,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  iVar6 = 2;
  iVar4 = FUN_00426b20();
  if (2 < iVar4) {
    local_8 = -0x10e;
    local_c = &DAT_005cf594;
    do {
      iVar4 = *local_c;
      if ((int)local_c < 0x5cf5d1) {
        iVar5 = 9;
        iVar7 = local_8 + 0x140;
      }
      else if ((int)local_c < 0x5cf611) {
        iVar5 = 0xd0;
        iVar7 = local_8;
      }
      else {
        iVar5 = 0x197;
        iVar7 = local_8 + -0x140;
      }
      FUN_004cd680(iVar1 + iVar5,iVar7 + iVar2,0xbe,0x12,(DAT_007056c8 != iVar6 + -1) - 1 & 0xfac1);
      uVar8 = DAT_00f56fa4;
      if ((&DAT_00784ce0)[iVar4] != 0) {
        uVar8 = DAT_00f56f8c;
      }
      FUN_004d6250(0x43,iVar4,iVar1 + iVar5,iVar7 + 4 + iVar2,0xbe,uVar8,0,DAT_00e38da8,DAT_00e38da4
                  );
      local_c = local_c + 1;
      local_8 = local_8 + 0x14;
      iVar6 = iVar6 + 1;
      iVar4 = FUN_00426b20();
    } while (iVar6 < iVar4);
  }
  return;
}
