#include "restore_types.h"
#define DAT_00d3a6c4 (*(undefined *)0x00d3a6c4u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e94e0c (*(undefined *)0x00e94e0cu)
#define DAT_00e94e10 (*(undefined *)0x00e94e10u)
extern void FUN_00547a60();

void FUN_004168e0(int param_1)

{
  if (DAT_00e38dda == '\x1e') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda == 'I') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda == '\x1f') {
    DAT_00e38dda = '\x01';
  }
  else if (DAT_00e38dda == ' ') {
    DAT_00e38dda = '\x01';
  }
  else if ((DAT_00e38dda == '\x12') || (DAT_00e38dda == '#')) {
    if (DAT_00d3a6c4 != 1) {
      return;
    }
  }
  else if (DAT_00e38dda != '\x01') {
    return;
  }
  if (DAT_00e94e0c != param_1) {
    DAT_00e94e10 = 0;
    DAT_00e94e0c = param_1;
    DAT_00e38dca = 1;
    FUN_00547a60();
    return;
  }
  DAT_00e94e10 = param_1;
  DAT_00e94e0c = 0;
  DAT_00e38dca = 1;
  FUN_00547a60();
  return;
}
