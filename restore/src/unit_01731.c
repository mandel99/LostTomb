#include "restore_types.h"
#include <stdlib.h>

int FUN_004cde60(int param_1,int param_2)

{
  if (param_2 != 0) {
    int32_t scaled = (int32_t)((uint32_t)param_1 * 100u);
    /* LEA/SHL wraps before signed IDIV in the native routine. */
    if (scaled == INT32_MIN && param_2 == -1) {
      int quotient;
      /* Preserve the x86 divide fault and its actual arithmetic operands. */
      __asm {
        mov ecx,param_2
        mov eax,param_1
        lea eax,[eax+eax*4]
        lea eax,[eax+eax*4]
        shl eax,2
        cdq
        idiv ecx
        mov quotient,eax
      }
      return quotient;
    }
    return scaled / param_2;
  }
  return 0;
}
