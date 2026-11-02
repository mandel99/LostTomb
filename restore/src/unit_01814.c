#include "restore_types.h"

void FUN_004d3f40(void)
{
  unsigned char *record = (unsigned char *)(uintptr_t)0x00bcbac0u;

  do {
    record[0] = 0;
    record += 0x90;
  } while ((uintptr_t)record < 0x00bcd650u);
}
