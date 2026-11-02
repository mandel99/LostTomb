#include "restore_types.h"
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

int FUN_0045dc50(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  if (param_6 < param_5) {
    param_6 = param_5 - param_6;
  }
  else {
    param_6 = param_6 - param_5;
  }
  if (param_5 == -1) {
    param_6 = 0;
  }
  if (param_3 < param_1) {
    DAT_00d3b4fc = param_1 - param_3;
  }
  else if (param_1 < param_3) {
    DAT_00d3b4fc = param_3 - param_1;
  }
  else {
    DAT_00d3b4fc = 0;
  }
  if (param_4 < param_2) {
    _DAT_00d3b4f8 = param_2 - param_4;
  }
  else if (param_2 < param_4) {
    _DAT_00d3b4f8 = param_4 - param_2;
  }
  else {
    _DAT_00d3b4f8 = 0;
  }
  if (DAT_00d3b4fc < _DAT_00d3b4f8) {
    return _DAT_00d3b4f8 + param_6;
  }
  return DAT_00d3b4fc + param_6;
}
