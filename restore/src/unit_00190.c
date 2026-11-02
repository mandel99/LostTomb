#include "restore_types.h"
#define DAT_0070e460 (*(undefined *)0x0070e460u)
#define DAT_0070e464 (*(undefined *)0x0070e464u)
#define DAT_0070e494 (*(undefined4 *)0x0070e494u)
#define DAT_00bca2bc (*(undefined *)0x00bca2bcu)
#define DAT_00bca2c0 (*(undefined *)0x00bca2c0u)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e94ddc (*(undefined4 *)0x00e94ddcu)

void FUN_00416b10(int param_1)

{
  if (DAT_00e38dda == '\x1e') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda == 'I') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda == '\x1f') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda == ' ') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda != '\x01') {
    return;
  }
  if ((-1 < param_1) && (param_1 < 5)) {
    (&DAT_00e94ddc)[param_1 * 2] =
         (&DAT_0070e494)[DAT_0070e464 / 2 + (DAT_0070e460 / 2 + DAT_00bca2c0) * 0xe7 + DAT_00bca2bc]
    ;
  }
  return;
}
