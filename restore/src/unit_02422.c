#include "restore_types.h"
/* Native dialog geometry, font tokens and selection state are DWORDs. */
#define DAT_007056c8 (*(uint32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38e5c (*(uint32_t *)0x00e38e5cu)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
#define _DAT_00f53f18 (*(undefined4 *)0x00f53f18u)
extern void FUN_004d6250();
extern void FUN_004d6480();
extern void FUN_00536820();
extern void FUN_005370d0();

void FUN_00531190(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = DAT_00f56f8c;
  FUN_00536820(DAT_00e38da8 + 0x60,DAT_00e38da4 + 0x50,0x12,0xc);
  FUN_004d6480(2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x88,0xe,0,(DAT_007056c8 != 1) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xa0,0xe,0,(DAT_007056c8 != 2) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xb8,0xe,0,(DAT_007056c8 != 3) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xd0,0xe,0,(DAT_007056c8 != 4) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xe8,0xe,0,(DAT_007056c8 != 5) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2a,0,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x5e,0xe0,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  if (DAT_00e38e5c == 0) {
    uVar2 = 1;
  }
  else {
    uVar2 = 2;
  }
  FUN_004d6250(0x2a,uVar2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x8c,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 1) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2a,3,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xa4,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 2) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2a,4,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xbc,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 3) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2a,5,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xd4,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 4) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x2a,6,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xec,0xe0,uVar1,
               (-(uint)(DAT_007056c8 != 5) & 0xffff07fe) + 0xfac1,DAT_00e38da8,DAT_00e38da4);
  FUN_005370d0();
  _DAT_00f53f18 = 0x32;
  return;
}
