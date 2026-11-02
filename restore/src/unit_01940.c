#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
#define DAT_00ea6188 (*(undefined4 *)0x00ea6188u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e2970();

void FUN_004e3240(int param_1)

{
  if ((((DAT_00605a3c != 1) && (DAT_00e94e20 == 0)) && (param_1 == 0x5f)) &&
     (*(int *)(&DAT_00ea6188 + DAT_00eaac28 * 0x49d8) < 1)) {
    FUN_004e2970(0x2b,0,0);
  }
  return;
}
