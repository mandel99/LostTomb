#include "restore_types.h"
/* Native dialog geometry, font tokens and selection state are DWORDs. */
#define DAT_005e03fc (*(undefined4 *)0x005e03fcu)
#define DAT_007056c8 (*(uint32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38e63 (*(char *)0x00e38e63u)
#define DAT_00e38e64 (*(char *)0x00e38e64u)
#define DAT_00e38e65 (*(char *)0x00e38e65u)
#define DAT_00e38ec0 (*(char *)0x00e38ec0u)
#define DAT_00e38ec4 (*(uint32_t *)0x00e38ec4u)
#define DAT_00e38ec8 (*(uint32_t *)0x00e38ec8u)
#define DAT_00e38ecc (*(uint32_t *)0x00e38eccu)
#define DAT_00e38ed0 (*(uint32_t *)0x00e38ed0u)
#define DAT_00f53f1c (*(uint32_t *)0x00f53f1cu)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d6480();
extern void FUN_004d6860();
extern void FUN_00536820();
extern void FUN_005370d0();

void FUN_00531ef0(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_00f56f8c;
  FUN_00536820(DAT_00e38da8 + 0x30,DAT_00e38da4 + 0x50,0x18,0x12);
  FUN_004d6480(3,DAT_00e38da8 + 0x40,DAT_00e38da4 + 0xa2,0xe,0,(DAT_007056c8 != 1) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(3,DAT_00e38da8 + 0x40,DAT_00e38da4 + 0xc0,0xe,0,(DAT_007056c8 != 2) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(3,DAT_00e38da8 + 0x40,DAT_00e38da4 + 0xde,0xe,0,(DAT_007056c8 != 3) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(3,DAT_00e38da8 + 0x40,DAT_00e38da4 + 0xfc,0xe,0,(DAT_007056c8 != 4) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(3,DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x128,0xc,0,(DAT_007056c8 != 5) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(3,DAT_00e38da8 + 0x90,DAT_00e38da4 + 0x146,0xc,0,(DAT_007056c8 != 6) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2e,0,DAT_00e38da8 + 0x60,DAT_00e38da4 + 0x5c,0x120,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x2e,0xc,DAT_00e38da8 + 0x80,DAT_00e38da4 + 300,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 5) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2e,9,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x14a,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 6) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d62f0(0x2e,10,DAT_00e38da8 + 0x70,DAT_00e38da4 + 0x8e,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d62f0(0x2e,0xb,DAT_00e38da8 + 0x150,DAT_00e38da4 + 0x8e,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x2e,(DAT_00e38e64 != '\0') + '\x01',DAT_00e38da8 + 0x40,DAT_00e38da4 + 0xa6,0xe0,
               uVar1,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6860(DAT_00e38ec8,0x40,&DAT_005e03fc,DAT_00e38da8 + 0x172,DAT_00e38da4 + 0xa6,DAT_00f56f8c
               ,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2e,(DAT_00e38e65 != '\0') + '\x03',DAT_00e38da8 + 0x40,DAT_00e38da4 + 0xc4,0xe0,
               uVar1,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6860(DAT_00e38ecc,0x40,&DAT_005e03fc,DAT_00e38da8 + 0x172,DAT_00e38da4 + 0xc4,DAT_00f56f8c
               ,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2e,(DAT_00e38e63 != '\0') + '\x05',DAT_00e38da8 + 0x40,DAT_00e38da4 + 0xe2,0xe0,
               uVar1,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6860(DAT_00e38ec4,0x40,&DAT_005e03fc,DAT_00e38da8 + 0x172,DAT_00e38da4 + 0xe2,DAT_00f56f8c
               ,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2e,(DAT_00e38ec0 != '\0') + '\a',DAT_00e38da8 + 0x40,DAT_00e38da4 + 0x100,0xe0,
               uVar1,0,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6860(DAT_00e38ed0,0x40,&DAT_005e03fc,DAT_00e38da8 + 0x172,DAT_00e38da4 + 0x100,
               DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
  FUN_005370d0();
  DAT_00f53f1c = 0x32;
  return;
}
