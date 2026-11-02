#include "restore_types.h"
#include <stdlib.h>

int FUN_004cde60(int param_1,int param_2)

{
  if (param_2 != 0) {
    int32_t scaled = (int32_t)((uint32_t)param_1 * 100u);
    /* LEA/SHL wraps before signed IDIV in the native routine. */
    if (scaled == INT32_MIN && param_2 == -1) abort();
    return scaled / param_2;
  }
  return 0;
}
