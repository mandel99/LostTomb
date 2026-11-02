#include "restore_types.h"
#include <stdint.h>

int FUN_004e1600(int value)

{
  if (value<=50) return (int)((uint32_t)value+99u);
  return (int)((uint32_t)value+199u);
}
