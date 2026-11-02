#include "restore_types.h"

void FUN_004d4180(void)
{
  unsigned char persistent_flag = 0;
  unsigned char *record = (unsigned char *)(uintptr_t)0x00799af0u;

  do {
    if (record[-0x10] == 1) {
      short type = *(short *)record;
      if (type == 0x29 || type == 0x2a || type == 0x2b) {
        unsigned int value = record[0x9a];
        if (value & 0x40u) {
          persistent_flag = 1;
          value -= 0x40u;
        }
        value++;
        if (value == 4) value = 0;
        record[0x9a] = (unsigned char)((persistent_flag << 4) + value);
      }
    }
    record += 0x108;
  } while ((uintptr_t)record < 0x0089b6e8u);
}
