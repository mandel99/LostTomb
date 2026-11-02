#include "restore_types.h"
#define DAT_00f5400c (*(undefined4 *)0x00f5400cu)
#define DAT_00f5404c (*(undefined *)0x00f5404cu)
#define DAT_00f54054 (*(undefined *)0x00f54054u)
#define DAT_00f54114 (*(undefined *)0x00f54114u)
extern bool FUN_004ce100();
extern void FUN_004ce2d0();

void FUN_005391e0(void)

{
  if (DAT_00f54054 != 0) {
    if (DAT_00f5404c == 0) {
      FUN_004ce100(&DAT_00f5400c);
    }
    if (DAT_00f54114 != 0) {
      _SmackClose_4(DAT_00f54114);
      DAT_00f54114 = 0;
    }
    FUN_004ce2d0();
    return;
  }
  return;
}
