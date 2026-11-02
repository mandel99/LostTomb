#include "restore_types.h"

void FUN_004d40a0(void)
{
  unsigned char *city = (unsigned char *)(uintptr_t)0x00ea5d3cu;

  do {
    *(unsigned int *)(city - 4) = 0;
    *(unsigned int *)city = 0;
    city += 0x49d8;
  } while ((uintptr_t)city <= 0x00eaa714u);
}
