#include "restore_types.h"
#include <stdint.h>
void FUN_004e0cc0(int index,int *x_out,int *y_out)

{
  const unsigned char *record=(const unsigned char *)(uintptr_t)
      (0x00799a40u+(uint32_t)index*0x108u);

  *x_out=*(const int32_t *)record+22;
  *y_out=*(const int32_t *)(record+4)+4;
}
