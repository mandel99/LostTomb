#include "restore_types.h"
#define LAB_004f6500 (*(undefined1 *)0x004f6500u)
#define LAB_004f6510 (*(undefined1 *)0x004f6510u)
#define DAT_00eaaf68 (*(undefined4 *)0x00eaaf68u)
extern void FUN_00561441();

void FUN_004f64b0(void)

{
  FUN_00561441(&DAT_00eaaf68,0xe0,0x32,&LAB_004f6500,&LAB_004f6510);
  return;
}
