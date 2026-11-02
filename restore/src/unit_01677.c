#include "restore_types.h"
#define DAT_00e207ec (*(uint8_t *)0x00e207ecu)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)
extern void FUN_004cc4c0();

void FUN_004cc330(void)

{
  if (0 < *(int *)(&DAT_00e207ec + DAT_00e20990 * 4)) {
    *(int *)(&DAT_00e207ec + DAT_00e20990 * 4) = *(int *)(&DAT_00e207ec + DAT_00e20990 * 4) + -1;
    FUN_004cc4c0();
    return;
  }
  return;
}
