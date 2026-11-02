#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_007057c8 (*(undefined *)0x007057c8u)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
extern void FUN_004e2970();

void FUN_004e31c0(int param_1)

{
  if ((((DAT_00605a3c != 1) && (DAT_00e94e20 == 0)) && (param_1 == 0x21)) && (DAT_007057c8 < 1)) {
    FUN_004e2970(0x2e,0,0);
  }
  return;
}
