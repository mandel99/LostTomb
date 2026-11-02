#include "restore_types.h"
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004f68f0();

undefined4 FUN_004a4340(int param_1,int param_2)

{
  if (*(char *)((int)&DAT_00799aa0 + param_2 * 0x108 + 3) == '\0') {
    return 0;
  }
  if ((&DAT_00bcd630)[param_1 * 0x184] == '\x04') {
    return 0;
  }
  FUN_004f68f0(param_1);
  return 1;
}
