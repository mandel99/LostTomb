#include "restore_types.h"
#define DAT_00d3b1c4 (*(undefined *)0x00d3b1c4u)
#define DAT_00d3b1c8 (*(undefined4 *)0x00d3b1c8u)
#define DAT_00ea3e34 (*(undefined4 *)0x00ea3e34u)

void FUN_004e9350(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  if (param_2 != 0) {
    iVar2 = DAT_00d3b1c4;
    do {
      iVar1 = ((&DAT_00d3b1c8)[iVar2] & 0x3f) + param_1 * 0x24ec;
      if (*(short *)(&DAT_00ea3e34 + iVar1 * 2) < 1) {
        iVar3 = iVar3 + 1;
      }
      else {
        param_2 = param_2 + -1;
        *(short *)(&DAT_00ea3e34 + iVar1 * 2) = *(short *)(&DAT_00ea3e34 + iVar1 * 2) + -1;
        iVar3 = 0;
      }
      iVar2 = iVar2 + 1;
      if (99 < iVar2) {
        iVar2 = 0;
      }
    } while ((iVar3 < 100) && (param_2 != 0));
  }
  iVar2 = 0;
  iVar3 = 10;
  if (param_2 != 0) {
    do {
      iVar1 = param_1 * 0x24ec + iVar3;
      if (*(short *)(&DAT_00ea3e34 + iVar1 * 2) < 1) {
        iVar2 = iVar2 + 1;
      }
      else {
        param_2 = param_2 + -1;
        *(short *)(&DAT_00ea3e34 + iVar1 * 2) = *(short *)(&DAT_00ea3e34 + iVar1 * 2) + -1;
        iVar2 = 0;
      }
      iVar3 = iVar3 + 1;
      if (99 < iVar3) {
        iVar3 = 0;
      }
    } while ((iVar2 < 100) && (param_2 != 0));
  }
  return;
}
