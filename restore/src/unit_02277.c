#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00eadfb0 (*(int32_t *)0x00eadfb0u)
#define DAT_00eadfc8 (*(int32_t *)0x00eadfc8u)
#define DAT_00f55c9c (*(int32_t *)0x00f55c9cu)
#define DAT_00f56494 (*(int32_t *)0x00f56494u)
#define DAT_00f56fb4 (*(int32_t *)0x00f56fb4u)
#define PTR_DAT_005f7ee0 (*(unsigned char * *)0x005f7ee0u)
extern void FUN_00425730();
extern void FUN_004ccb30();
extern int FUN_004cde40();
extern int FUN_004cde60();
extern void FUN_004cf1c0();
extern undefined4 FUN_005165b0();
extern void FUN_00516720();
extern void FUN_00536820();
extern void FUN_00536a30();
extern int FUN_0053e590();

void FUN_00516630(void)

{
  int iVar1;
  undefined4 uVar2;
  
  if (DAT_00e38dca == '\x01') {
    FUN_004cf1c0();
    FUN_0053e590(DAT_00f55c9c,DAT_00e38da8,DAT_00e38da4);
  }
  DAT_00e38dc9 = 1;
  FUN_00536820(DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x28,0x18,0x17);
  FUN_00536a30(DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x78,0x14,0xd);
  uVar2 = FUN_005165b0();
  FUN_00425730(DAT_00e38da8,DAT_00e38da4,&PTR_DAT_005f7ee0,uVar2);
  FUN_004ccb30(0x124,3,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x3a,0x180,DAT_00f56fb4,0);
  FUN_00516720();
  FUN_00536a30(DAT_00e38da8 + 0x1d3,DAT_00e38da4 + 0x92,2,10);
  if (0 < DAT_00eadfb0 + -0xc) {
    if (DAT_00eadfc8 < 1) {
      uVar2 = 0;
    }
    else if (DAT_00eadfc8 + 0xc < DAT_00eadfb0) {
      uVar2 = FUN_004cde60(DAT_00eadfc8,DAT_00eadfb0 + -0xc);
    }
    else {
      uVar2 = 100;
    }
    iVar1 = FUN_004cde40(0x82,uVar2);
    FUN_0053e590(DAT_00f56494 + 0x27,DAT_00e38da8 + 0x1d8,DAT_00e38da4 + 0x91 + iVar1);
  }
  return;
}
