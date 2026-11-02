#include "restore_types.h"

void FUN_004de140(void)
{
  unsigned int *pair = (unsigned int *)(uintptr_t)0x00e94de0u;

  do {
    pair[-1] = 0xffffffffu;
    pair[0] = 0xffffffffu;
    pair += 2;
  } while ((uintptr_t)pair < 0x00e94e00u);
}
