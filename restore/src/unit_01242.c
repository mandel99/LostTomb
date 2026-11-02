#include "restore_types.h"
#define DAT_00ba746c (*(undefined *)0x00ba746cu)
#define DAT_00bc95f0 (*(undefined *)0x00bc95f0u)
#define DAT_00bc968c (*(undefined *)0x00bc968cu)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)

undefined4 FUN_004851b0(int param_1)

{
  short *psVar1;
  
  if ((((0 < param_1) && (DAT_00bc968c == 1)) && (DAT_00bc9694 == 1)) && (0 < DAT_00bc95f0)) {
    psVar1 = &DAT_00ba746c;
    do {
      if (*psVar1 == param_1) {
        return 1;
      }
      psVar1 = psVar1 + 1;
    } while ((int)psVar1 < 0xba7886);
  }
  return 0;
}
