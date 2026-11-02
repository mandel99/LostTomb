#include "restore_types.h"
#define DAT_00d3b5c0 (*(undefined *)0x00d3b5c0u)
#define DAT_00d3b5dc (*(undefined *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(undefined *)0x00d3b5e0u)
#define DAT_00f53f10 (*(undefined4 *)0x00f53f10u)
#define DAT_00f56494 (*(undefined *)0x00f56494u)
extern int FUN_0053e590();

void FUN_00537000(int param_1,int param_2,undefined4 param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  
  if (param_5 != 1) {
    if (*(int *)(&DAT_00f53f10 + param_1 * 4) < 0x14) {
      return;
    }
    iVar1 = *(int *)(&DAT_00f53f10 + param_1 * 4) + -0x12;
    if (iVar1 <= param_4) {
      param_2 = param_2 + ((param_4 - iVar1) * 0x10) / 2;
      param_4 = iVar1;
    }
  }
  iVar1 = 0;
  if (0 < param_4) {
    do {
      DAT_00d3b5e0 = iVar1 * 0x10 + param_2;
      DAT_00d3b5dc = param_3;
      if (iVar1 == 0) {
        DAT_00d3b5c0 = param_6 * 3 + 0x28 + DAT_00f56494;
      }
      else if (iVar1 < param_4 + -1) {
        DAT_00d3b5c0 = param_6 * 3 + 0x29 + DAT_00f56494;
      }
      else {
        DAT_00d3b5c0 = DAT_00f56494 + (param_6 + 0xe) * 3;
      }
      FUN_0053e590(DAT_00d3b5c0,DAT_00d3b5e0,param_3);
      iVar1 = iVar1 + 1;
    } while (iVar1 < param_4);
  }
  return;
}
