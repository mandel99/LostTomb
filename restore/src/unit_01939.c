#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_007058b8 (*(undefined *)0x007058b8u)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
extern void FUN_004e2970();

void FUN_004e3200(int param_1)

{
  if ((((DAT_00605a3c != 1) && (DAT_00e94e20 == 0)) &&
      ((param_1 == 0x28 || ((param_1 == 0x2c || (param_1 == 0x2d)))))) && (DAT_007058b8 < 1)) {
    FUN_004e2970(0x2a,0,0);
  }
  return;
}
