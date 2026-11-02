#include "restore_types.h"
#define DAT_0078d838 (*(undefined *)0x0078d838u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00978d70 (*(undefined *)0x00978d70u)
extern void FUN_0046cea0();

undefined4 FUN_004539a0(void)

{
  if (DAT_00978d70 == 0) {
    return 0;
  }
  if (*(char *)(&DAT_007999dc + DAT_0078d838 * 0x42) == '\0') {
    FUN_0046cea0(DAT_0078d838,(int)(short)(&DAT_007999e8)[DAT_0078d838 * 0x84]);
  }
  return 1;
}
