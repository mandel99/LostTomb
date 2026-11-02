#include "restore_types.h"
#define DAT_00bcb610 (*(uint32_t *)0x00bcb610u)
#define DAT_00bcb614 (*(uint32_t *)0x00bcb614u)
#define DAT_00bcb620 (*(undefined *)0x00bcb620u)
#define DAT_00d05710 (*(undefined1 *)0x00d05710u)

void FUN_00489a50(undefined4 param_1)

{
  DAT_00bcb610 = param_1;
  DAT_00bcb614 = GetTickCount();
  if (DAT_00d05710 != '\0') {
    QueryPerformanceCounter((LARGE_INTEGER *)&DAT_00bcb620);
  }
  return;
}
