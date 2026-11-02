#include "restore_types.h"
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern int FUN_00488030();

int FUN_00433c90(int param_1)

{
  char cVar1;
  
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xa7) {
    return 0;
  }
  if ((&DAT_007999e8)[param_1 * 0x84] == 99) {
    return 0;
  }
  cVar1 = FUN_00488030(param_1);
  if (cVar1 != '\0') {
    cVar1 = (&DAT_00b67d1c)[DAT_0070e488];
    if (DAT_00bca2e0 == 0) {
      if (cVar1 != 'P') {
        return 0;
      }
    }
    else if (DAT_00bca2e0 == 2) {
      if (cVar1 != '@') {
        return 0;
      }
    }
    else if (DAT_00bca2e0 == 4) {
      if (cVar1 != 'B') {
        return 0;
      }
    }
    else if ((DAT_00bca2e0 == 6) && (cVar1 != 'R')) {
      return 0;
    }
  }
  return (int)*(short *)(&DAT_00799a20 + param_1 * 0x108) / 100;
}
