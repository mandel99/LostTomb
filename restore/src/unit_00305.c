#include "restore_types.h"
#include "file_list.h"
#define DAT_00704c08 (*(int32_t *)0x00704c08u)
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define DAT_00704db8 (*(int32_t *)0x00704db8u)
#define DAT_00704dbc (*(int32_t *)0x00704dbcu)
#define DAT_007056c4 (*(int32_t *)0x007056c4u)
#define DAT_007056c8 (*(int32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
#define DAT_00f56f94 (*(int32_t *)0x00f56f94u)
#define DAT_00f56f9c (*(int32_t *)0x00f56f9cu)
#define DAT_00f56fa4 (*(int32_t *)0x00f56fa4u)

extern undefined * FUN_0041e120();
extern undefined ** FUN_0041e190();
extern void FUN_004cc770();
extern void FUN_004ccfd0();
extern void FUN_004cd1a0();
extern void FUN_004cd240();
extern void FUN_0051f780();
extern void FUN_00536a30();

void FUN_0041f220(void)

{
  int iVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 local_8 [2];
  
  iVar6 = 1;
  if (DAT_00704db0 == 1) {
    iVar4 = 0xd;
    iVar1 = 0xfc;
  }
  else {
    iVar4 = 0xf;
    iVar1 = 0xdc;
  }
  FUN_00536a30(DAT_00e38da8 + 0xf,DAT_00e38da4 + -0xf + iVar1,0x10,iVar4 + 1);
  iVar1 = 0x10;
  for (; iVar6 <= (int)((-(uint)(DAT_00704db0 != 1) & 2) + 0xd); iVar6 = iVar6 + 1) {
    iVar4 = (int)(uintptr_t)FUN_0041e010(RESTORE_MISSION_FILES,DAT_00704db8 + -1 + iVar6);
    uVar3 = DAT_00f56f94;
    if (iVar4 != 0) {
      uVar5 = DAT_00f56f9c;
      if ((DAT_007056c8 != iVar6) &&
         (((DAT_007056c8 != 0 || (DAT_00704db8 + -1 + iVar6 != DAT_00704dbc)) &&
          (iVar2 = FUN_0041dfe0(RESTORE_MISSION_FILES,DAT_00704db8 + -1 + iVar6), uVar5 = uVar3, iVar2 == 2)))) {
        uVar5 = DAT_00f56fa4;
      }
      FUN_004cc770(iVar4,DAT_00e38da8 + 0x18,
                   DAT_00e38da4 + iVar1 + (-(uint)(DAT_00704db0 != 1) & 0xffffffe0) + 0xec,uVar5,0);
    }
    iVar1 = iVar1 + 0x10;
  }
  if (DAT_00704db0 == 1) {
    local_8[0] = 0x26;
    local_8[1] = 0x27;
    iVar6 = 0;
    do {
      iVar1 = (-(uint)(DAT_00704db0 != 1) & 2) + 0xd + iVar6;
      iVar4 = iVar1 * 0x1c;
      FUN_004cd1a0(0,(char *)FUN_0041e190()+iVar4,iVar6 == 0 || DAT_007056c8 == iVar1 + 1,0,0);
      FUN_004cd240(0x126,local_8[iVar6],(char *)FUN_0041e190()+iVar4,DAT_00f56f8c,0,4,7);
      iVar6 = iVar6 + 1;
    } while (iVar6 < 2);
    if (DAT_00704c08 == 0) {
      FUN_004ccfd0(0x126,0x28,DAT_00e38da8 + 0x18,
                   DAT_00e38da4 + (-(uint)(DAT_00704db0 != 1) & 0xffffffe0) + 0xfc,0xeb,DAT_00f56f94
                   ,0);
    }
  }
  if (DAT_00704db0 == 1) {
    FUN_004cd1a0(0,FUN_0041e120(),DAT_007056c4 != 0,0x15e,0x19a);
  }
  FUN_0051f780();
  FUN_0051f780();
  return;
}
