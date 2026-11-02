#include "restore_types.h"

void FUN_004d3d80(void)
{
  unsigned char *record = (unsigned char *)(uintptr_t)0x00785ddfu;

  do {
    record[-1] = 6;
    record[0] = 6;
    record[1] = 6;
    record[2] = 6;
    record += 0x7c;
  } while ((uintptr_t)record < 0x0078a687u);
}
