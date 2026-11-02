#include "restore_types.h"
#define DAT_00f53dcc (*(undefined4 *)0x00f53dccu)
#define _DAT_00f53dc8 (*(undefined4 *)0x00f53dc8u)

void FUN_00527ad0(int param_1)

{
  if (param_1 < 0x65) {
    DAT_00f53dcc = 100;
    _DAT_00f53dc8 = 0xffffffff;
    return;
  }
  if (param_1 < 0xc9) {
    DAT_00f53dcc = 200;
    _DAT_00f53dc8 = 0;
    return;
  }
  if (param_1 < 0x191) {
    DAT_00f53dcc = 400;
    _DAT_00f53dc8 = 1;
    return;
  }
  if (param_1 < 0x321) {
    DAT_00f53dcc = 800;
    _DAT_00f53dc8 = 2;
    return;
  }
  if (param_1 < 0x641) {
    DAT_00f53dcc = 0x640;
    _DAT_00f53dc8 = 3;
    return;
  }
  if (param_1 < 0xc81) {
    DAT_00f53dcc = 0xc80;
    _DAT_00f53dc8 = 4;
    return;
  }
  if (param_1 < 0x1901) {
    DAT_00f53dcc = 0x1900;
    _DAT_00f53dc8 = 5;
    return;
  }
  if (param_1 < 0x3201) {
    DAT_00f53dcc = 0x3200;
    _DAT_00f53dc8 = 6;
    return;
  }
  if (param_1 < 0x6401) {
    DAT_00f53dcc = 0x6400;
    _DAT_00f53dc8 = 7;
    return;
  }
  DAT_00f53dcc = 0xc800;
  _DAT_00f53dc8 = 8;
  return;
}
