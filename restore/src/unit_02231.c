#include "restore_types.h"
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00eadd6c (*(undefined *)0x00eadd6cu)
#define DAT_00eadd8c (*(undefined *)0x00eadd8cu)
#define DAT_00eadd90 (*(undefined *)0x00eadd90u)
#define DAT_00f56f94 (*(undefined *)0x00f56f94u)
extern void FUN_004d62f0();

void FUN_004fc3b0(undefined4 param_1)

{
  char cVar1;
  
  cVar1 = *(char *)(&DAT_00799a9a + DAT_00eadd6c * 0x84);
  if ('1' < cVar1) {
    FUN_004d62f0(0x7f,0x1a,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  if ('\'' < cVar1) {
    FUN_004d62f0(0x7f,0x1b,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  if ('\x1d' < cVar1) {
    FUN_004d62f0(0x7f,0x1c,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  if ('\x13' < cVar1) {
    FUN_004d62f0(0x7f,0x1d,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  if ('\t' < cVar1) {
    FUN_004d62f0(0x7f,0x1e,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  if ('\0' < cVar1) {
    FUN_004d62f0(0x7f,0x1f,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
    return;
  }
  FUN_004d62f0(0x7f,0x20,DAT_00eadd90 + 0x24,param_1,DAT_00f56f94,0,DAT_00eadd90,DAT_00eadd8c);
  return;
}
