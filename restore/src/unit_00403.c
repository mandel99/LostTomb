#include "restore_types.h"
#define DAT_005c79c8 (*(undefined *)0x005c79c8u)
#define DAT_005c79ca (*(undefined *)0x005c79cau)
#define DAT_005c79d2 (*(undefined *)0x005c79d2u)
#define DAT_005c79e1 (*(char *)0x005c79e1u)
#define DAT_005c79e2 (*(char *)0x005c79e2u)
#define DAT_005c79e4 (*(undefined *)0x005c79e4u)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00e38dc8 (*(char *)0x00e38dc8u)
#define DAT_00f55bb4 (*(undefined4 *)0x00f55bb4u)
extern int FUN_0053e590();

void FUN_00425900(void)

{
  DAT_00d3b5e0 = (int)DAT_005c79c8;
  DAT_00d3b5c0 = (&DAT_00f55bb4)[DAT_005c79d2 * 2];
  DAT_00d3b5dc = (int)DAT_005c79ca;
  if (DAT_005c79e1 != '\0') {
    if (DAT_005c79e1 == '\x14') {
      DAT_00e38dc8 = 1;
    }
    DAT_00d3b5c0 = DAT_00d3b5c0 + 4 + DAT_005c79e4;
    FUN_0053e590(DAT_00d3b5c0,DAT_00d3b5e0,DAT_00d3b5dc);
    return;
  }
  if (DAT_005c79e2 != '\0') {
    DAT_00d3b5c0 = DAT_00d3b5c0 + 1 + DAT_005c79e4;
  }
  FUN_0053e590(DAT_00d3b5c0,DAT_00d3b5e0,DAT_00d3b5dc);
  return;
}
