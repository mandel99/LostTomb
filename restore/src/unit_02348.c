#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_006059e4 (*(int32_t *)0x006059e4u)
#define DAT_00605a3c (*(int32_t *)0x00605a3cu)
#define DAT_00e38dcc (*(int32_t *)0x00e38dccu)
#define DAT_00f53f64 (*(int32_t *)0x00f53f64u)
#define DAT_00f53f78 (*(int32_t *)0x00f53f78u)
#define DAT_00f53fd0 (*(int32_t *)0x00f53fd0u)
extern void FUN_0043e810();
extern void FUN_004cd510();

void FUN_0051efb0(int param_1)

{
  bool bVar1;
  
  if (DAT_00f53f78 == 0) {
    bVar1 = DAT_00f53fd0 != 0;
    if (bVar1) {
      DAT_00f53fd0 = 0;
    }
    if ((DAT_00605a3c == 1) && (DAT_006059e4 != 0)) {
      bVar1 = true;
    }
    if (DAT_00e38dcc != 0) {
      bVar1 = true;
    }
    if ((param_1 != 0) || (bVar1)) {
      if (DAT_00605a3c == 0) {
        FUN_0043e810(DAT_00f53f64 + 0xd,0x3b,0x49,0x6f);
        return;
      }
      FUN_0043e810(DAT_00f53f64 + 8,0x24,0x49,0x6f);
      FUN_004cd510(DAT_00f53f64 + 7,0x23,DAT_00f53f64 + 0x99,0x23,0x4208);
      FUN_004cd510(DAT_00f53f64 + 7,0x24,DAT_00f53f64 + 7,0x93,0x4208);
      FUN_004cd510(DAT_00f53f64 + 0x99,0x24,DAT_00f53f64 + 0x99,0x93,0xc618);
    }
  }
  return;
}
