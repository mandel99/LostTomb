#include "restore_types.h"
#define DAT_00bcb9c8 (*(undefined4 *)0x00bcb9c8u)
#define DAT_00bcba08 (*(undefined4 *)0x00bcba08u)
extern void FUN_0040f8f0();
extern void FUN_004b4980();
extern void FUN_004b77d0();

void FUN_004b82e0(int param_1)

{
  if (0 < param_1) {
    param_1 = param_1 * 0x90;
    if ((&DAT_00bcb9c8)[param_1] != '\0') {
      if (*(short *)(&DAT_00bcba08 + param_1) != 0) {
        FUN_004b4980((int)*(short *)(&DAT_00bcba08 + param_1));
      }
      FUN_0040f8f0(&DAT_00bcb9c8 + param_1,0x90,0);
      FUN_004b77d0();
    }
  }
  return;
}
