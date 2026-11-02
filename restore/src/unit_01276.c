#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
extern int FUN_00487f80();

int FUN_00488030(int param_1)

{
  return FUN_00487f80((int)(short)(&DAT_007999e8)[param_1 * 0x84]);
}
