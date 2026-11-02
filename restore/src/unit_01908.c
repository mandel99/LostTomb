#include "restore_types.h"
#include <stdint.h>
undefined4 FUN_004e0c50(int index,int *x_out,int *y_out)

{
  const unsigned char *record=(const unsigned char *)(uintptr_t)
      (0x007999dcu+(uint32_t)index*0x108u);
  int y=(int)*(const int16_t *)(record+4);

  *x_out=(int)*(const int16_t *)(record+2);
  *y_out=y;
  /* The original writes AL only after sign-extending y into EAX. */
  return (undefined4)(((uint32_t)y&0xffffff00u)|1u);
}
