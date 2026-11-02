#include "restore_types.h"
char FUN_0050fc70(int param_1)

{
  if (*(undefined1 *)(0x00bcd74cu + (uint)param_1 * 0x184u) == '\0') {
    return '\x1c';
  }
  return (*(undefined1 *)(0x00bcd74cu + (uint)param_1 * 0x184u) != '\x01') + '\x1d';
}
