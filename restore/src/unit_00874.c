#include "restore_types.h"
#define DAT_00ea44e4 (*(undefined4 *)0x00ea44e4u)
#define DAT_00ea44ec (*(undefined4 *)0x00ea44ecu)
#define DAT_00ea5ce8 (*(undefined4 *)0x00ea5ce8u)
#define DAT_00ea5f9c (*(undefined4 *)0x00ea5f9cu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_004c5120();

void FUN_0045a930(void)

{
  if ((&DAT_00ea5ce8)[DAT_00eaac28 * 0x24ec] != 0) {
    FUN_004c5120((&DAT_00ea5f9c)[DAT_00eaac28 * 0x1276]);
  }
  (&DAT_00ea44ec)[DAT_00eaac28 * 0x1276] = (&DAT_00ea44e4)[DAT_00eaac28 * 0x1276];
  return;
}
