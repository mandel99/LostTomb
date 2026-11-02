#include "restore_types.h"
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e94ddc (*(undefined4 *)0x00e94ddcu)
extern void FUN_00430cd0();

void FUN_00416ac0(int param_1)

{
  if (DAT_00e38dda == '\x1e') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda == 'I') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda != '\x01') {
    return;
  }
  if (((-1 < param_1) && (param_1 < 5)) && (-1 < (int)(&DAT_00e94ddc)[param_1 * 2])) {
    FUN_00430cd0((&DAT_00e94ddc)[param_1 * 2]);
    DAT_00e38dca = 1;
  }
  return;
}
