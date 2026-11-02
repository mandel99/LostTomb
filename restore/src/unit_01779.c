#include "restore_types.h"
#include "window_messages.h"
#define DAT_00e39128 (*(uint32_t *)0x00e39128u)
#define DAT_00e39140 (*(HWND *)0x00e39140u)
#define s_OK__Paused__005e0764 (*(char (*)[14])0x005e0764u)
extern void FUN_004ce6e0();

void FUN_004cff90(void)

{
  FUN_004ce6e0(s_OK__Paused__005e0764,0,0);
  DAT_00e39128 = 1;
  restore_window_platform()->invalidate(DAT_00e39140,(RECT *)0x0,1);
  return;
}
