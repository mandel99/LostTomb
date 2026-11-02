#include "restore_types.h"
#define DAT_00e207ec (*(uint8_t *)0x00e207ecu)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)

void FUN_004cc3c0(void)

{
  *(undefined4 *)(&DAT_00e207ec + DAT_00e20990 * 4) = 0;
  return;
}
