#include "restore_types.h"
#define DAT_00704db0 (*(int32_t *)0x00704db0u)
#define PTR_DAT_0057cdb8 (*(unsigned char * *)0x0057cdb8u)
#define PTR_DAT_0057cf60 (*(unsigned char * *)0x0057cf60u)

undefined ** FUN_0041e190(void)

{
  undefined **ppuVar1;
  
  ppuVar1 = &PTR_DAT_0057cf60;
  if (DAT_00704db0 != 1) {
    ppuVar1 = &PTR_DAT_0057cdb8;
  }
  return ppuVar1;
}
