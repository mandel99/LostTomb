#include "restore_types.h"
#define DAT_0070e488 (*(undefined *)0x0070e488u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799a59 (*(undefined4 *)0x00799a59u)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
extern void FUN_004361a0();

void FUN_00435630(int param_1,undefined4 param_2,undefined4 param_3)

{
  char cVar1;
  undefined4 uVar2;
  
  DAT_00d3b5c0 = (&DAT_00b7482c)[DAT_0070e488];
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    cVar1 = (&DAT_00799a59)[param_1 * 0x108];
    if (cVar1 == '\x01') {
      uVar2 = 2;
    }
    else if (cVar1 == '\x02') {
      uVar2 = 5;
    }
    else {
      if (cVar1 != '\x03') {
        return;
      }
      uVar2 = 8;
    }
    FUN_004361a0(uVar2,param_2,param_3,0);
  }
  return;
}
