#include "restore_types.h"
#define DAT_00e2079c (*(uint8_t *)0x00e2079cu)
#define DAT_00e207ec (*(uint8_t *)0x00e207ecu)
#define DAT_00e208a0 (*(uint8_t *)0x00e208a0u)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)
extern void FUN_004cbee0();
extern void FUN_004cc1b0();

void FUN_004cc350(void)

{
  if (0 < DAT_00e20990) {
    FUN_004cbee0(*(int *)(&DAT_00e207ec + DAT_00e20990 * 4) +
                 *(int *)(&DAT_00e2079c + DAT_00e20990 * 4),
                 *(int *)(&DAT_00e208a0 + DAT_00e20990 * 4) +
                 *(int *)(&DAT_00e2079c + DAT_00e20990 * 4));
    FUN_004cc1b0();
    return;
  }
  return;
}
