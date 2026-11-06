#include "restore_types.h"
#define DAT_005d2e88 (*(int32_t *)0x005d2e88u)
#define DAT_0077f748 (*(uint8_t *)0x0077f748u)
#define DAT_00784f14 (*(int32_t *)0x00784f14u)
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dac (*(int32_t *)0x00e38dacu)
#define DAT_00f56f8c (*(uint32_t *)0x00f56f8cu)
extern void FUN_004d6250();
extern void FUN_0051f780();
extern void FUN_0052db10();
extern void FUN_0052e610();

void FUN_0052da40(void)

{
  int iVar1;
  
  iVar1 = (int32_t)((uint32_t)DAT_00784f14 - 1u);
  if (DAT_00784f14 < 1) {
    if (-1 < DAT_005d2e88) {
      FUN_0052e610();
      return;
    }
    FUN_004d6250(0x2f,9,DAT_00e38da8,(int32_t)((uint32_t)DAT_00e38dac - 0x4cu),0x280,DAT_00f56f8c,0,DAT_00e38da8,
                 DAT_00e38da4);
  }
  else {
    if ((&DAT_0077f748)[(uint32_t)iVar1 * 0x62u] == '\x01') {
      FUN_0052db10();
    }
    if ((&DAT_0077f748)[(uint32_t)iVar1 * 0x62u] == '\x03') {
      FUN_0051f780();
      return;
    }
  }
  return;
}
