#include "restore_types.h"
#include <stdint.h>
#include <string.h>

int FUN_004ea280(int minimum_age)
{
  const unsigned char *record = (const unsigned char *)(uintptr_t)0x00799ae0u;
  int count = 0, i;
  for (i = 0; i < 4000; ++i, record += 0x108) {
    int16_t age, occupied;
    memcpy(&age, record + 0x10, sizeof(age));
    memcpy(&occupied, record + 0x1c, sizeof(occupied));
    if (record[0] != 0 && record[5] != 0 && occupied != 0 &&
        minimum_age <= (int)age - 10) ++count;
  }
  return count;
}
