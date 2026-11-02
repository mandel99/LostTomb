#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00bca2c8 (*(undefined *)0x00bca2c8u)
#define DAT_00bca2cc (*(undefined *)0x00bca2ccu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd728 (*(undefined4 *)0x00bcd728u)
#define DAT_00d05708 (*(undefined *)0x00d05708u)

undefined4 FUN_004d6b00(int param_1)

{
  if (((DAT_00d05708 != 0) && ((&DAT_00bcd728)[param_1 * 0x184] != '\0')) &&
     (((byte)(&DAT_00af598c)[DAT_00bca2c8 + DAT_00bca2cc * 0xe4 + DAT_00bca2d8] & 4) != 4)) {
    return 1;
  }
  return 0;
}
