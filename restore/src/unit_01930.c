#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_00e94e20 (*(undefined *)0x00e94e20u)
#define DAT_00ea4474 (*(undefined4 *)0x00ea4474u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004e2970();

void FUN_004e2d30(int param_1)

{
  if ((((DAT_00605a3c != 1) && (param_1 != 0x5c)) && (DAT_00e94e20 == 0)) &&
     ((0 < *(int *)(&DAT_00705eb8 + param_1 * 0x28) &&
      (9 < (int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276])))) {
    FUN_004e2970(0x11,0,0);
  }
  return;
}
