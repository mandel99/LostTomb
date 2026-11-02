#include "restore_types.h"
#include <string.h>

void FUN_004d3200(void)

{
  unsigned char *source = (unsigned char *)(uintptr_t)0x007900e0u;

  do {
    unsigned char record[0xc4];

    memset(record,0,sizeof(record));
    memcpy(record,source - 0x28,0x6c);
    memcpy(record + 0x0a,source - 0x1e,0x1e);
    memcpy(record + 0x2e,source,4);
    memcpy(record + 0x32,source + 4,2);
    memcpy(record + 0x34,source + 6,0x3c);
    memset(record + 0x70,0,0x0c);
    memcpy(source - 0x28,record,sizeof(record));
    source += 0xc4;
  } while ((uintptr_t)source < 0x00799a00u);
}
