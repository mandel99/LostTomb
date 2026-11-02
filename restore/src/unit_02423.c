#include "restore_types.h"
/* Native dialog geometry, font tokens and selection state are DWORDs. */
#define DAT_006de510 (*(uint32_t *)0x006de510u)
#define DAT_007056c8 (*(uint32_t *)0x007056c8u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
#define DAT_00f56fa4 (*(uint32_t *)0x00f56fa4u)
#define DAT_00f56fb4 (*(uint32_t *)0x00f56fb4u)
#define _DAT_00f53f4c (*(undefined4 *)0x00f53f4cu)
extern void FUN_004d6250();
extern void FUN_004d6480();
extern void FUN_00536820();

void FUN_00531480(void)

{
  undefined4 uVar1;
  
  uVar1 = DAT_00f56f8c;
  FUN_00536820(DAT_00e38da8 + 0x20,DAT_00e38da4 + 0x26,0x1a,0x1c);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x74,0xe,0,(DAT_007056c8 != 1) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x8c,0xe,0,(DAT_007056c8 != 2) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xa4,0xe,0,(DAT_007056c8 != 3) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xbc,0xe,0,(DAT_007056c8 != 4) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xd4,0xe,0,(DAT_007056c8 != 5) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xec,0xe,0,(DAT_007056c8 != 6) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x104,0xe,0,(DAT_007056c8 != 7) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x11c,0xe,0,(DAT_007056c8 != 8) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x134,0xe,0,(DAT_007056c8 != 9) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x14c,0xe,0,(DAT_007056c8 != 10) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x164,0xe,0,(DAT_007056c8 != 0xb) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x17c,0xe,0,(DAT_007056c8 != 0xc) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x1ac,0xe,0,(DAT_007056c8 != 0xd) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6480(0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x1c4,0xe,0,(DAT_007056c8 != 0xe) + '\x01',
               DAT_00e38da8,DAT_00e38da4);
  FUN_004d6250(0x136,0,DAT_00e38da8 + 0x30,DAT_00e38da4 + 0x3c,0x180,DAT_00f56fb4,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x136,1,DAT_00e38da8 + 0x30,DAT_00e38da4 + 0x5f,0x180,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 2) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,2,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x78,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 4) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,3,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x90,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 8) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,4,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xa8,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x10) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,5,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xc0,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x20) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,6,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xd8,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x40) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,7,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0xf0,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x80) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,8,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x108,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x100) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,9,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x120,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x200) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,10,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x138,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x400) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,0xb,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x150,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x800) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,0xc,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x168,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  uVar1 = DAT_00f56fa4;
  if ((DAT_006de510 & 0x1000) == 0) {
    uVar1 = DAT_00f56f8c;
  }
  FUN_004d6250(0x136,0xd,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x180,0xe0,uVar1,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x136,0xe,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x1b0,0xe0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  FUN_004d6250(0x136,0xf,DAT_00e38da8 + 0x80,DAT_00e38da4 + 0x1c8,0xe0,DAT_00f56f8c,0,DAT_00e38da8,
               DAT_00e38da4);
  _DAT_00f53f4c = 0x32;
  return;
}
