#include "restore_types.h"

void FUN_004d4080(void)
{
  unsigned char *record = (unsigned char *)(uintptr_t)0x00799a5eu;

  do {
    record[0] = 0;
    record += 0x108;
  } while ((uintptr_t)record < 0x0089b75eu);
}
