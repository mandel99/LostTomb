#include "restore_types.h"
#define DAT_00d3b4bc (*(undefined *)0x00d3b4bcu)
#define DAT_00d3b4d0 (*(undefined *)0x00d3b4d0u)
#define DAT_00d3b4d4 (*(undefined *)0x00d3b4d4u)

void FUN_004cd490(int param_1)

{
  int iVar1;
  
  if (param_1 == 0) {
    iVar1 = DAT_00d3b4d4;
    if (-1 < DAT_00d3b4bc) {
      DAT_00d3b4bc = DAT_00d3b4bc + (DAT_00d3b4d4 - DAT_00d3b4d0) * 2;
      return;
    }
  }
  else {
    iVar1 = DAT_00d3b4d0;
    if (-1 < DAT_00d3b4bc) {
      iVar1 = DAT_00d3b4d0 - DAT_00d3b4d4;
    }
  }
  DAT_00d3b4bc = DAT_00d3b4bc + iVar1 * 2;
  return;
}
