#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_00bc9690 (*(undefined *)0x00bc9690u)
extern void FUN_0040f8f0();
extern void FUN_0046ac50();
extern void FUN_00485200();

void FUN_0046ac10(int param_1)

{
  FUN_0046ac50(param_1);
  FUN_0040f8f0(&DAT_007999d8 + param_1 * 0x108,0x108,0);
  if (DAT_00bc9690 == 9) {
    FUN_00485200();
    return;
  }
  return;
}
