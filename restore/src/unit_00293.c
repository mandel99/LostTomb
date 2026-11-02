#include "restore_types.h"
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define PTR_DAT_0057cc30 (*(unsigned char * *)0x0057cc30u)
#define PTR_DAT_0057ccc0 (*(unsigned char * *)0x0057ccc0u)

undefined ** FUN_0041e100(void)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_0057ccc0;
  if (DAT_00704db0 != 1) {
    ppuVar1 = &PTR_DAT_0057cc30;
  }
  return ppuVar1;
}
