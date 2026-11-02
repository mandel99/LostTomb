#include "restore_types.h"

void FUN_004d3f60(void)
{
  unsigned char *record = (unsigned char *)(uintptr_t)0x00799ba1u;

  do {
    switch (*(short *)(record - 0xb1)) {
    case 0x88:
      record[9] = record[0];
      record[0] = 0;
      record[10] = 0;
      break;
    case 0x4a:
      record[10] = record[0];
      record[0] = 0;
      record[9] = 0;
      break;
    case 0x46:
    case 0x47:
    case 0x48:
    case 0xbe:
    case 0xbf:
    case 0xc1:
      record[10] = 0;
      record[9] = 0;
      break;
    default:
      record[0] = 0;
      record[10] = 0;
      record[9] = 0;
      break;
    }
    record += 0x108;
  } while ((uintptr_t)record < 0x0089b799u);
}
