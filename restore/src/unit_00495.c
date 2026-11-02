#include "restore_types.h"
#define DAT_00705808 (*(undefined *)0x00705808u)
#define DAT_00705810 (*(undefined *)0x00705810u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a59 (*(undefined4 *)0x00799a59u)

undefined4 FUN_00434060(int param_1)

{
  char cVar1;
  
  if (((((&DAT_007999e8)[param_1 * 0x84] != 0x33) && ((&DAT_007999e8)[param_1 * 0x84] != 0x35)) &&
      (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0')) &&
     (DAT_00705810 + DAT_00705808 != 0)) {
    cVar1 = (&DAT_00799a59)[param_1 * 0x108];
    if (cVar1 == '\x01') {
      return 8;
    }
    if (cVar1 == '\x02') {
      return 5;
    }
    if (cVar1 == '\x03') {
      return 2;
    }
  }
  return 0;
}
