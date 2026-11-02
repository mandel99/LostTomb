#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
extern void FUN_004e2970();

void FUN_004e32e0(int param_1,int param_2,int param_3)

{
  if ((((DAT_00605a3c != 1) && (DAT_00e94e20 == 0)) && (param_1 == 0x5c)) &&
     ((*(byte *)(&DAT_00af598c + DAT_00bca2d8 + param_3 * 0xe4 + param_2) & 0x80) == 0)) {
    FUN_004e2970(0x26,0,0);
  }
  return;
}
