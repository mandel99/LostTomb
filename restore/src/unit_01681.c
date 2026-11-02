#include "restore_types.h"
#define DAT_00e207ec (*(uint8_t *)0x00e207ecu)
#define DAT_00e208a0 (*(uint8_t *)0x00e208a0u)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)

void FUN_004cc390(void)

{
  if (*(int *)(&DAT_00e207ec + DAT_00e20990 * 4) < *(int *)(&DAT_00e208a0 + DAT_00e20990 * 4)) {
    *(int *)(&DAT_00e207ec + DAT_00e20990 * 4) = *(int *)(&DAT_00e207ec + DAT_00e20990 * 4) + 1;
  }
  return;
}
