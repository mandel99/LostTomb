#include "restore_types.h"
#define DAT_00605a3c (*(undefined *)0x00605a3cu)
#define DAT_0077de03 (*(undefined4 *)0x0077de03u)
#define DAT_0077f6e6 (*(undefined4 *)0x0077f6e6u)
#define DAT_00784f14 (*(undefined *)0x00784f14u)
#define DAT_00784f18 (*(undefined *)0x00784f18u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
#define DAT_00e38dac (*(undefined *)0x00e38dacu)
#define DAT_00e38db0 (*(undefined *)0x00e38db0u)
#define DAT_00e39088 (*(char *)0x00e39088u)
#define DAT_00f56fb4 (*(undefined *)0x00f56fb4u)
extern void FUN_004d6250();

void FUN_0052d940(void)

{
  if (((DAT_00605a3c != 1) && (0 < DAT_00784f14)) &&
     ((&DAT_0077f6e6)[DAT_00784f14 * 0x62] == '\x01')) {
    if (DAT_00e39088 != '\0') {
      FUN_004d6250(0x15,(&DAT_0077de03)[DAT_00784f18 * 0x6a],(DAT_00e38db0 + -0x14c) / 2,
                   DAT_00e38dac + -0x76,0x14c,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
      return;
    }
    FUN_004d6250(0xc3,(&DAT_0077de03)[DAT_00784f18 * 0x6a],(DAT_00e38db0 + -0x14c) / 2,
                 DAT_00e38dac + -0x76,0x14c,DAT_00f56fb4,0,DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
