#include "restore_types.h"
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define DAT_00704db4 (*(int32_t *)0x00704db4u)
#define DAT_00704db8 (*(int32_t *)0x00704db8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56494 (*(int32_t *)0x00f56494u)
extern int FUN_004cde40();
extern int FUN_004cde60();
extern int FUN_0053e590();

void FUN_0041e1b0(void)

{
  undefined4 uVar1;
  int iVar2;
  bool bVar3;
  
  iVar2 = DAT_00704db4 - ((-(uint)(DAT_00704db0 != 1) & 2) + 0xd);
  if (0 < iVar2) {
    bVar3 = DAT_00704db0 != 1;
    if (DAT_00704db8 < 1) {
      uVar1 = 0;
    }
    else if ((int)((-(uint)(DAT_00704db0 != 1) & 2) + 0xd + DAT_00704db8) < DAT_00704db4) {
      uVar1 = FUN_004cde60(DAT_00704db8,iVar2);
    }
    else {
      uVar1 = 100;
    }
    iVar2 = FUN_004cde40(((-(uint)(DAT_00704db0 != 1) & 2) + 0xd) * 0x10 + -0x4c,uVar1);
    FUN_0053e590(DAT_00f56494 + 0x27,DAT_00e38da8 + 0x121,
                 DAT_00e38da4 + iVar2 + (-(uint)bVar3 & 0xffffffe0) + 0x115);
  }
  return;
}
