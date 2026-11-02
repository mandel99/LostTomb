#include "restore_types.h"
/* Native dialog geometry, font tokens and selection state are DWORDs. */
#define DAT_005e03fc (*(undefined4 *)0x005e03fcu)
#define DAT_007056c8 (*(uint32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38e6c (*(uint32_t *)0x00e38e6cu)
#define DAT_00e38e70 (*(uint32_t *)0x00e38e70u)
#define DAT_00f53f20 (*(uint32_t *)0x00f53f20u)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6480();
extern void FUN_004d6860();
extern void FUN_00536820();
extern void FUN_005370d0();

void FUN_00531cc0(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_00f56f8c;
  FUN_00536820(DAT_00e38da8 + 0x60,DAT_00e38da4 + 0x50,0x12,0xe);
  FUN_004d6480(4,DAT_00e38da8 + 0x90,DAT_00e38da4 + 0xe8,0xc,0,(DAT_007056c8 != 1) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(4,DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x106,0xc,0,(DAT_007056c8 != 2) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2d,0,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x5c,0xe0,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x2d,4,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xec,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 1) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2d,1,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x10a,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 2) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2d,2,DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x92,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6860(DAT_00e38e6c,0x40,&DAT_005e03fc,DAT_00e38da8 + 0x128,DAT_00e38da4 + 0x92,DAT_00f56f8c
               ,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2d,3,DAT_00e38da8 + 0x90,DAT_00e38da4 + 0xb6,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6860(DAT_00e38e70,0x40,&DAT_005e03fc,DAT_00e38da8 + 0x128,DAT_00e38da4 + 0xb6,DAT_00f56f8c
               ,0,DAT_00e38da8,DAT_00e38da4);
  FUN_005370d0();
  DAT_00f53f20 = 0x32;
  return;
}
