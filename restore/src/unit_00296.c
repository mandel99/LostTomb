#include "restore_types.h"
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_00704dac (*(int32_t *)0x00704dacu)
#define DAT_00704db0 (*(int32_t *)0x00704db0u)

undefined4 FUN_0041e140(void)

{
  undefined4 uVar1;
  
  uVar1 = 1;
  if ((DAT_00605a3c == 1) || ((DAT_00704db0 == 1 && (DAT_00704dac == 2)))) {
    uVar1 = 0;
  }
  return uVar1;
}
