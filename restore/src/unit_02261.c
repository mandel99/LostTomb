#include "restore_types.h"
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd724 (*(undefined4 *)0x00bcd724u)
extern int FUN_004cde60();

char FUN_0050fc10(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_004cde60(*(undefined4 *)(&DAT_00bcd720 + param_1 * 0x184),
                       *(undefined4 *)(&DAT_00bcd724 + param_1 * 0x184));
  if (iVar1 < 0x10) {
    return '\b';
  }
  if (iVar1 < 0x1f) {
    return '\a';
  }
  if (iVar1 < 0x33) {
    return '\x06';
  }
  if (iVar1 < 0x47) {
    return '\x05';
  }
  return (iVar1 < 0x5b) + '\x03';
}
