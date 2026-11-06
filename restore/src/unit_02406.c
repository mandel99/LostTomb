#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004cd930();
extern void FUN_004cd9d0();
extern void FUN_004d62f0();
extern void FUN_0052d630();
extern void FUN_0052e6f0();
extern void FUN_0052ec50();

void FUN_0052d4e0(void)

{
  int iVar1;
  
  FUN_0052d630();
  FUN_004cd9d0(10,DAT_00e38dac + -0x74,DAT_00e38db0 + -0x14,0x73,0xf73b);
  FUN_004cd930(10,DAT_00e38dac + -0x74,DAT_00e38db0 + -0x14,0x73);
  iVar1 = DAT_00e38dac;
  FUN_004d62f0(0x2c,0xde,0xf,DAT_00e38dac + -0x74,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004cd930(0xf,iVar1 + -0x56,100,0x12);
  FUN_004cd930(0xf,iVar1 + -0x42,100,0x12);
  FUN_004cd930(0xf,iVar1 + -0x2e,100,0x12);
  FUN_004cd930(0xf,iVar1 + -0x1a,100,0x12);
  FUN_004cd930(0x77,iVar1 + -0x56,100,0x12);
  FUN_004cd930(0x77,iVar1 + -0x42,100,0x12);
  FUN_004cd930(0x77,iVar1 + -0x2e,100,0x12);
  FUN_004cd930(0x77,iVar1 + -0x1a,100,0x12);
  FUN_0052e6f0();
  FUN_004cd9d0(0xdf,DAT_00e38dac + -0x72,DAT_00e38db0 + -0xee,0x6d,0xf73b);
  FUN_004cd930(0xdf,DAT_00e38dac + -0x72,DAT_00e38db0 + -0xee,0x6d);
  FUN_0052ec50();
  return;
}
