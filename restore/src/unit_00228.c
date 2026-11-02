#include "restore_types.h"
#define DAT_005c7df8 (*(undefined4 *)0x005c7df8u)
#define DAT_005c7e20 (*(undefined4 *)0x005c7e20u)
#define DAT_006fdc9c (*(undefined *)0x006fdc9cu)
#define DAT_006fdca0 (*(undefined *)0x006fdca0u)
#define DAT_00d3b134 (*(undefined *)0x00d3b134u)
#define DAT_00e38dc9 (*(char *)0x00e38dc9u)
extern void FUN_00425730();
extern void FUN_004c72b0();

void FUN_00418f70(void)

{
  DAT_00e38dc9 = 1;
  if (DAT_00d3b134 != 0) {
    FUN_00425730(DAT_006fdca0 + 0x22a,DAT_006fdc9c + 0xa8,&DAT_005c7df8,1);
    FUN_00425730(DAT_006fdca0 + 0x22a,DAT_006fdc9c + 0x17e,&DAT_005c7e20,1);
    FUN_004c72b0();
    return;
  }
  return;
}
