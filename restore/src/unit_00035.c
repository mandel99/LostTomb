#include "restore_types.h"
#define DAT_0057961c (*(undefined *)0x0057961cu)
#define DAT_007027c5 (*(char *)0x007027c5u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)

undefined4 FUN_00403ef0(void)

{
  if (DAT_00e38ea8 == DAT_0057961c) {
    return CONCAT31(DAT_00e38ea8 >> 7,DAT_007027c5 != '\0');
  }
  return CONCAT31(DAT_00e38ea8 >> 7,1);
}
