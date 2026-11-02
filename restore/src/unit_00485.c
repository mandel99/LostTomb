#include "restore_types.h"
#define DAT_00ac2d4c (*(undefined4 *)0x00ac2d4cu)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)

undefined4 FUN_00433a60(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  
  cVar1 = (&DAT_00ac2d4c)[param_1];
  if (((*(byte *)(&DAT_00af598c + param_1) & 8) == 0) && (cVar1 == '\0')) {
    return 0;
  }
  if (cVar1 < -10) {
    return 0;
  }
  if (cVar1 < -5) {
    return 1;
  }
  if (cVar1 < '\0') {
    return 2;
  }
  if (cVar1 == '\0') {
    return 3;
  }
  if (cVar1 < '\x05') {
    return 4;
  }
  if (cVar1 < '\n') {
    return 5;
  }
  if (cVar1 < '\x0f') {
    return 6;
  }
  if (cVar1 < '\x14') {
    return 7;
  }
  uVar2 = 8;
  if ('\x18' < cVar1) {
    uVar2 = 9;
  }
  return uVar2;
}
