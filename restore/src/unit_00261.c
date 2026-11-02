#include "restore_types.h"
#define DAT_006fdc98 (*(int32_t *)0x006fdc98u)
#define DAT_007027ac (*(int32_t *)0x007027acu)
#define DAT_007027b0 (*(int32_t *)0x007027b0u)
extern uint FUN_0041bad0();

uint FUN_0041bb30(void)

{
  char cVar1;
  
  if (DAT_006fdc98 == 0) {
    return (uint)(DAT_007027ac == DAT_007027b0);
  }
  cVar1 = FUN_0041bad0(DAT_007027ac);
  return (-(uint)(cVar1 != '\0') & 0xfffffffe) + 2;
}
