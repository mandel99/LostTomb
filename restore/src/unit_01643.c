#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799ad4 (*(undefined4 *)0x00799ad4u)
extern void FUN_004cc110();

void FUN_004ca6d0(int param_1)

{
  int iVar1;
  
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    iVar1 = *(int *)(&DAT_00799ad4 + param_1 * 0x108);
    if (0x351 < iVar1) {
      FUN_004cc110(0x42,0x83);
      return;
    }
    if (599 < iVar1) {
      FUN_004cc110(0x42,0x82);
      return;
    }
    if (0x1c1 < iVar1) {
      FUN_004cc110(0x42,0x81);
      return;
    }
    if (9 < iVar1) {
      FUN_004cc110(0x42,0x80);
      return;
    }
  }
  FUN_004cc110(0x42,0x7f);
  return;
}
