#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_007057c4 (*(undefined *)0x007057c4u)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
extern void FUN_004e2970();

void FUN_004e3180(int param_1)

{
  if ((((DAT_00605a3c != 1) && (DAT_00e94e20 == 0)) && (param_1 == 0x1e)) && (DAT_007057c4 < 1)) {
    FUN_004e2970(0x2d,0,0);
  }
  return;
}
