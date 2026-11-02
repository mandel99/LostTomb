#include "restore_types.h"
#define DAT_00d3b550 (*(undefined *)0x00d3b550u)
#define DAT_00d3b554 (*(undefined *)0x00d3b554u)
#define DAT_00d3b558 (*(undefined *)0x00d3b558u)
#define DAT_00d3b55c (*(undefined *)0x00d3b55cu)

bool FUN_004cdc30(int param_1,int param_2)

{
  if (param_1 < DAT_00d3b55c) {
    return false;
  }
  if (DAT_00d3b558 < param_1) {
    return false;
  }
  if (param_2 < DAT_00d3b554) {
    return false;
  }
  return param_2 <= DAT_00d3b550;
}
