#include "restore_types.h"
#define DAT_005f825c (*(undefined *)0x005f825cu)
#define DAT_007056c8 (*(undefined *)0x007056c8u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00f53f08 (*(undefined *)0x00f53f08u)
#define DAT_00f53f0c (*(undefined *)0x00f53f0cu)
extern void FUN_004d63d0();

void FUN_005271f0(void)

{
  if (DAT_007056c8 != DAT_005f825c) {
    if (DAT_007056c8 == 0) {
      DAT_005f825c = DAT_007056c8;
      FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0x3c,0x6a,0x39,0,DAT_00e38da8,DAT_00e38da4)
      ;
      FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0xa0,0x6a,0x39,0,DAT_00e38da8,DAT_00e38da4)
      ;
      return;
    }
    if (DAT_007056c8 == 1) {
      DAT_005f825c = DAT_007056c8;
      FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0x3c,0x6a,0x39,1,DAT_00e38da8,DAT_00e38da4)
      ;
      FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0xa0,0x6a,0x39,0,DAT_00e38da8,DAT_00e38da4)
      ;
      return;
    }
    if (DAT_007056c8 == 2) {
      DAT_005f825c = DAT_007056c8;
      FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0x3c,0x6a,0x39,0,DAT_00e38da8,DAT_00e38da4)
      ;
      FUN_004d63d0(0,DAT_00f53f0c + 0x1f5,DAT_00f53f08 + 0xa0,0x6a,0x39,1,DAT_00e38da8,DAT_00e38da4)
      ;
    }
  }
  return;
}
