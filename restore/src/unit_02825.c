#include "restore_types.h"
#define DAT_00604950 (*(uint32_t *)0x00604950u)

uint FUN_00562348(void)

{
  DAT_00604950 = DAT_00604950 * 0x343fd + 0x269ec3;
  return DAT_00604950 >> 0x10 & 0x7fff;
}
