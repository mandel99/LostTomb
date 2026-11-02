#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799ac0 (*(undefined4 *)0x00799ac0u)
extern void FUN_004cc110();

void FUN_004ca600(int param_1)

{
  int iVar1;
  
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    iVar1 = (int)(&DAT_00799ac0)[param_1 * 0x42] / 10;
    if (0x54 < iVar1) {
      FUN_004cc110(0x42,0x8c);
      return;
    }
    if (0x3b < iVar1) {
      FUN_004cc110(0x42,0x8b);
      return;
    }
    if (0x2c < iVar1) {
      FUN_004cc110(0x42,0x8a);
      return;
    }
    if (0 < iVar1) {
      FUN_004cc110(0x42,0x89);
      return;
    }
  }
  FUN_004cc110(0x42,0x88);
  return;
}
