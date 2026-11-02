#include "restore_types.h"
#define DAT_006059ec (*(int32_t *)0x006059ecu)
#define DAT_006059f4 (*(int32_t *)0x006059f4u)
#define DAT_006059f8 (*(int32_t *)0x006059f8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56494 (*(int32_t *)0x00f56494u)
extern int FUN_004cde40();
extern int FUN_004cde60();
extern int FUN_0053e590();

void FUN_00532830(void)

{
  undefined4 uVar1;
  int iVar2;
  
  if (DAT_006059ec == 0) {
    if (0 < DAT_006059f8 + -0xc) {
      if (DAT_006059f4 < 1) {
        uVar1 = 0;
      }
      else if (DAT_006059f4 + 0xc < DAT_006059f8) {
        uVar1 = FUN_004cde60(DAT_006059f4,DAT_006059f8 + -0xc);
      }
      else {
        uVar1 = 100;
      }
      iVar2 = FUN_004cde40(0x82,uVar1);
      FUN_0053e590(DAT_00f56494 + 0x27,DAT_00e38da8 + 0x1d8,DAT_00e38da4 + 0x91 + iVar2);
      return;
    }
  }
  else if (0 < DAT_006059f8 + -0xf) {
    if (DAT_006059f4 < 1) {
      uVar1 = 0;
    }
    else if (DAT_006059f4 + 0xf < DAT_006059f8) {
      uVar1 = FUN_004cde60(DAT_006059f4,DAT_006059f8 + -0xf);
    }
    else {
      uVar1 = 100;
    }
    iVar2 = FUN_004cde40(0xa4,uVar1);
    FUN_0053e590(DAT_00f56494 + 0x27,DAT_00e38da8 + 0x11c,DAT_00e38da4 + 0xf5 + iVar2);
  }
  return;
}
