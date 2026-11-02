#include "restore_types.h"
#define DAT_00784f0c (*(int32_t *)0x00784f0cu)
#define DAT_00e20768 (*(uint32_t *)0x00e20768u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00f55cac (*(int32_t *)0x00f55cacu)
#define DAT_00f56fb4 (*(int32_t *)0x00f56fb4u)
extern void FUN_00448370();
extern void FUN_004cb5a0();
extern void FUN_004cc110();
extern void FUN_004cc720();
extern void FUN_004cf1c0();
extern void FUN_00536820();
extern int FUN_0053e590();

void FUN_004cb4d0(void)

{
  undefined4 uVar1;
  undefined1 local_c8 [200];
  
  if (DAT_00e38dca == '\x01') {
    FUN_004cf1c0();
    FUN_0053e590(DAT_00f55cac,DAT_00e38da8,DAT_00e38da4);
  }
  FUN_00536820(DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x28,0x18,0x15);
  FUN_004cc110(0x125,5);
  uVar1 = DAT_00784f0c;
  DAT_00784f0c = 0;
  FUN_00448370(DAT_00e20768,local_c8,200);
  DAT_00784f0c = uVar1;
  FUN_004cc720(local_c8,DAT_00e38da8 + 0x8c,DAT_00e38da4 + 0x3c,0x170,DAT_00f56fb4,0);
  FUN_004cb5a0();
  return;
}
