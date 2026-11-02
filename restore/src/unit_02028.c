#include "restore_types.h"
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00ea895c (*(undefined4 *)0x00ea895cu)
extern int FUN_00458f80();

void FUN_004e8630(void)

{
  _DAT_00ea895c = 1;
  FUN_00458f80(DAT_00eaac28);
  return;
}
