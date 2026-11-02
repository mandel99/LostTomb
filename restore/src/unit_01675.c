#include "restore_types.h"
#define DAT_00e2083c (*(uint8_t *)0x00e2083cu)
#define DAT_00e20990 (*(int32_t *)0x00e20990u)
extern void FUN_004cc1b0();
extern void FUN_004cc400();

void FUN_004cc250(undefined4 param_1)

{
  byte bVar1;
  
  bVar1 = (byte)param_1;
  if (bVar1 == 0x20) {
    param_1 = 0x20;
  }
  else if (bVar1 == 0x2c) {
    if ((&DAT_00e2083c)[DAT_00e20990] != '\0') goto LAB_004cc318;
    param_1 = 0x2c;
  }
  else if (bVar1 == 0x2e) {
    if ((&DAT_00e2083c)[DAT_00e20990] != '\0') goto LAB_004cc318;
    param_1 = 0x2e;
  }
  else if (bVar1 == 0x3f) {
    if ((&DAT_00e2083c)[DAT_00e20990] != '\0') goto LAB_004cc318;
    param_1 = 0x3f;
  }
  else if (bVar1 == 0x21) {
    if ((&DAT_00e2083c)[DAT_00e20990] != '\0') goto LAB_004cc318;
    param_1 = 0x21;
  }
  else if (bVar1 == 0x2d) {
    param_1 = 0x2d;
  }
  else if ((((((bVar1 < 0x30) || (0x39 < bVar1)) && ((bVar1 < 0x61 || (0x7a < bVar1)))) &&
            ((bVar1 < 0x41 || (0x5a < bVar1)))) && ((bVar1 < 0x80 || (0x9a < bVar1)))) &&
          ((bVar1 < 0xa0 || (0xa7 < bVar1)))) {
    if (bVar1 != 0xe1) goto LAB_004cc318;
    param_1 = 0xe1;
  }
  FUN_004cc400(param_1);
LAB_004cc318: ;
  FUN_004cc1b0();
  return;
}
