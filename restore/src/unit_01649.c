#include "restore_types.h"
#define DAT_00d3a4c0 (*(undefined4 *)0x00d3a4c0u)
#define DAT_00d3a6cc (*(undefined *)0x00d3a6ccu)
#define DAT_00d3a6d0 (*(undefined *)0x00d3a6d0u)
#define DAT_00d3a6d4 (*(undefined *)0x00d3a6d4u)
#define DAT_00d3a6d8 (*(undefined *)0x00d3a6d8u)
#define DAT_00d3b130 (*(undefined *)0x00d3b130u)
#define DAT_00d3b138 (*(undefined *)0x00d3b138u)
#define DAT_00d3b13c (*(undefined *)0x00d3b13cu)
#define DAT_00d3b164 (*(undefined *)0x00d3b164u)
#define DAT_00d3b168 (*(undefined *)0x00d3b168u)
#define DAT_00d3b51c (*(undefined *)0x00d3b51cu)
#define DAT_00e38e30 (*(undefined *)0x00e38e30u)
#define DAT_00e38e34 (*(undefined *)0x00e38e34u)
#define DAT_00e92dfc (*(undefined *)0x00e92dfcu)
#define DAT_00f56f84 (*(undefined *)0x00f56f84u)
extern void FUN_004c8070();
extern void FUN_004cd680();
extern void FUN_004cd9d0();

void FUN_004caf10(void)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  DAT_00d3b168 = DAT_00f56f84;
  DAT_00d3b164 = DAT_00f56f84;
  DAT_00d3a6d0 = 0xdc;
  DAT_00d3b130 = 0;
  DAT_00d3b13c = 0x1e;
  DAT_00d3b51c = 0x4208;
  FUN_004c8070(&DAT_00d3a4c0,DAT_00d3a6d8 + 5,DAT_00d3a6d4 + 5,0xbe,1,0);
  DAT_00d3a6cc = DAT_00d3b138 * 0x10 + 10;
  if (2 < DAT_00d3b138) {
    DAT_00d3a6d0 = 300;
    DAT_00d3b130 = 0;
    DAT_00d3b13c = 0x1e;
    FUN_004c8070(&DAT_00d3a4c0,DAT_00d3a6d8 + 5,DAT_00d3a6d4 + 5,0x10e,1,0);
    DAT_00d3a6cc = DAT_00d3b138 * 0x10 + 10;
  }
  if (DAT_00e38e34 < DAT_00d3a6d0 + 0x14) {
    DAT_00d3a6d8 = DAT_00e38e34 + 0x14;
  }
  else {
    DAT_00d3a6d8 = (DAT_00e38e34 - DAT_00d3a6d0) + -0x14;
  }
  DAT_00d3a6d4 = DAT_00e38e30 + 0x32;
  if (199 < DAT_00e38e30) {
    DAT_00d3a6d4 = DAT_00e38e30 + -0x48;
  }
  if (DAT_00e92dfc == 2) {
    uVar2 = 0xbb88;
    uVar1 = 0x58a0;
  }
  else {
    uVar2 = 0x5dc8;
    uVar1 = 0x2c40;
  }
  FUN_004cd680(DAT_00d3a6d8,DAT_00d3a6d4,DAT_00d3a6d0 + 0x14,DAT_00d3a6cc,uVar1);
  FUN_004cd9d0(DAT_00d3a6d8 + 1,DAT_00d3a6d4 + 1,DAT_00d3a6d0 + 0x12,DAT_00d3a6cc + -2,uVar2);
  FUN_004c8070(&DAT_00d3a4c0,DAT_00d3a6d8 + 5,DAT_00d3a6d4 + 7,DAT_00d3a6d0 + -0x1e,0,0);
  return;
}
