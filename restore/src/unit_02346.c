#include "restore_types.h"
#include "city_ui_controllers.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
#define DAT_00e94e0c (*(int32_t *)0x00e94e0cu)
#define DAT_00f53db0 (*(int32_t *)0x00f53db0u)
#define DAT_00f53f78 (*(int32_t *)0x00f53f78u)
#define DAT_00f56f8c (*(int32_t *)0x00f56f8cu)
extern void FUN_004d6250();

void FUN_0051ed30(int param_1,undefined4 param_2)

{
  int iVar1;
  
  iVar1 = 1;
  if (DAT_00e94e0c == DAT_00f53db0) {
    iVar1 = param_1;
  }
  if (DAT_00e38dc9 != '\0') {
    iVar1 = 1;
  }
  if ((DAT_00f53f78 == 0) && (iVar1 != 0)) {
    DAT_00f53db0 = DAT_00e94e0c;
    if (DAT_00e94e0c == 0) {
      FUN_004d6250(6,4,param_2,0x22,0x75,DAT_00f56f8c,0xffff,DAT_00e38da8,DAT_00e38da4);
      return;
    }
    FUN_004d6250(0xe,DAT_00e94e0c,param_2,0x22,0x75,DAT_00f56f8c,0xffff,DAT_00e38da8,DAT_00e38da4);
  }
  return;
}
