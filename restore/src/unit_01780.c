#include "restore_types.h"
#include "window_messages.h"
#define DAT_00e39128 (*(uint32_t *)0x00e39128u)
#define DAT_00e39140 (*(HWND *)0x00e39140u)

void FUN_004cffc0(void)

{
  HWND pHVar1;
  
  pHVar1 = restore_window_platform()->foreground();
  if (pHVar1 == DAT_00e39140) {
    DAT_00e39128 = 0;
  }
  return;
}
