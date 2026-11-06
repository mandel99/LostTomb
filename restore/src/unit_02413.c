#include "restore_types.h"
#define DAT_00775928 (*(int32_t *)0x00775928u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
extern void FUN_004d6250();

void FUN_0052e6f0(void)

{
  int iVar1;
  int iVar2;
  
  iVar2 = DAT_00e38dac;
  iVar1 = DAT_00e38dac + -0x54;
  FUN_004d6250(0x2c,0,0xf,iVar1,100,DAT_00f56f8c,(-(uint)(DAT_00775928 != 1) & 0xffff0800) + 0xf800,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2c,1,0xf,iVar2 + -0x40,100,DAT_00f56f8c,(DAT_00775928 != 2) - 1 & 0xf800,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2c,2,0xf,iVar2 + -0x2c,100,DAT_00f56f8c,(DAT_00775928 != 3) - 1 & 0xf800,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2c,3,0xf,iVar2 + -0x18,100,DAT_00f56f8c,(DAT_00775928 != 4) - 1 & 0xf800,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2c,0xa6,0x77,iVar1,100,DAT_00f56f8c,(DAT_00775928 != 0xe) - 1 & 0xf800,DAT_00e38da8
               ,DAT_00e38da4);
  FUN_004d6250(0x2c,0xa7,0x77,iVar2 + -0x40,100,DAT_00f56f8c,(DAT_00775928 != 0xf) - 1 & 0xf800,
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2c,0xdf,0x77,iVar2 + -0x2c,100,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2c,7,0x77,iVar2 + -0x18,100,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  return;
}
