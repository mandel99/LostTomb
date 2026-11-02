#include "restore_types.h"
#define DAT_005f89cc (*(undefined4 *)0x005f89ccu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e20778 (*(undefined *)0x00e20778u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00ea5cc0 (*(undefined4 *)0x00ea5cc0u)
#define DAT_00ea5dc8 (*(undefined4 *)0x00ea5dc8u)
#define DAT_00ea5dd8 (*(undefined4 *)0x00ea5dd8u)
#define DAT_00ea5ddc (*(undefined4 *)0x00ea5ddcu)
#define DAT_00ea5de0 (*(undefined4 *)0x00ea5de0u)
#define DAT_00ea5de4 (*(undefined4 *)0x00ea5de4u)
#define DAT_00ea5de8 (*(undefined4 *)0x00ea5de8u)
#define DAT_00ea5dec (*(undefined4 *)0x00ea5decu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
#define DAT_00f56f8c (*(undefined *)0x00f56f8cu)
#define DAT_00f56f9c (*(undefined *)0x00f56f9cu)
#define DAT_00f56fa4 (*(undefined *)0x00f56fa4u)
extern void FUN_004d6160();
extern void FUN_004d6250();
extern void FUN_004d62f0();
extern void FUN_004d63d0();
extern void FUN_004d6860();

void FUN_00525720(void)

{
  undefined4 uVar1;
  
  if ((int)(&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276] < (int)(&DAT_00ea5de4)[DAT_00eaac28 * 0x1276]) {
    FUN_004d6160(0x34,0x46,DAT_00f53f0c + 0xa0,DAT_00f53f08 + 0xe0,0x160,DAT_00f56f9c,0,DAT_00e38da8
                 ,DAT_00e38da4);
  }
  else {
    FUN_004d62f0(0x34,0x3f,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xda,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    uVar1 = DAT_00f56fa4;
    if (DAT_007056c8 != 1) {
      uVar1 = DAT_00f56f9c;
    }
    DAT_00e20778 = 0;
    FUN_004d62f0(0x34,(&DAT_00ea5dd8)[DAT_00eaac28 * 0x1276] + 0x33,DAT_00f53f0c + 0xe0,
                 DAT_00f53f08 + 0xda,uVar1,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5de4)[DAT_00eaac28 * 0x1276],0x40,&DAT_005f89cc,
                 DAT_00e20778 + 0xe0 + DAT_00f53f0c,DAT_00f53f08 + 0xda,uVar1,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  if ((int)(&DAT_00ea5de8)[DAT_00eaac28 * 0x1276] <= (int)(&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276]) {
    FUN_004d62f0(0x34,0x40,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0xee,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    uVar1 = DAT_00f56fa4;
    if (DAT_007056c8 != 2) {
      uVar1 = DAT_00f56f9c;
    }
    DAT_00e20778 = 0;
    FUN_004d62f0(0x34,(&DAT_00ea5ddc)[DAT_00eaac28 * 0x1276] + 0x37,DAT_00f53f0c + 0xe0,
                 DAT_00f53f08 + 0xee,uVar1,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5de8)[DAT_00eaac28 * 0x1276],0x40,&DAT_005f89cc,
                 DAT_00e20778 + 0xe0 + DAT_00f53f0c,DAT_00f53f08 + 0xee,uVar1,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  if ((int)(&DAT_00ea5dec)[DAT_00eaac28 * 0x1276] <= (int)(&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276]) {
    FUN_004d62f0(0x34,0x41,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0x102,DAT_00f56f9c,0,DAT_00e38da8,
                 DAT_00e38da4);
    uVar1 = DAT_00f56fa4;
    if (DAT_007056c8 != 3) {
      uVar1 = DAT_00f56f9c;
    }
    DAT_00e20778 = 0;
    FUN_004d62f0(0x34,(&DAT_00ea5de0)[DAT_00eaac28 * 0x1276] + 0x3b,DAT_00f53f0c + 0xe0,
                 DAT_00f53f08 + 0x102,uVar1,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d6860((&DAT_00ea5dec)[DAT_00eaac28 * 0x1276],0x40,&DAT_005f89cc,
                 DAT_00e20778 + 0xe0 + DAT_00f53f0c,DAT_00f53f08 + 0x102,uVar1,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  if ((int)(&DAT_00ea5de4)[DAT_00eaac28 * 0x1276] <= (int)(&DAT_00ea5cc0)[DAT_00eaac28 * 0x1276]) {
    FUN_004d6250(0x34,(&DAT_00ea5dc8)[DAT_00eaac28 * 0x1276] + 0x42,DAT_00f53f0c + 0x80,
                 DAT_00f53f08 + 0x155,0xf0,DAT_00f56f8c,0,DAT_00e38da8,DAT_00e38da4);
    FUN_004d63d0(1,DAT_00f53f0c + 0x80,DAT_00f53f08 + 0x150,0xf0,0x14,DAT_007056c8 == 4,DAT_00e38da8
                 ,DAT_00e38da4);
  }
  FUN_004d63d0(1,DAT_00f53f0c + 400,DAT_00f53f08 + 0x150,0xa0,0x14,DAT_007056c8 == 5,DAT_00e38da8,
               DAT_00e38da4);
  return;
}
