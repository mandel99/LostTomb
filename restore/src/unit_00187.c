#include "restore_types.h"
#define DAT_00605a44 (*(undefined *)0x00605a44u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00ea623c (*(undefined4 *)0x00ea623cu)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
extern void FUN_0040a0e0();

void FUN_00416990(int param_1)

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
  else {
    if (DAT_00e38dda != ' ') {
      if (DAT_00e38dda == '\a') {
        if (DAT_00605a44 != param_1) {
          DAT_00d3b198 = param_1;
          FUN_0040a0e0();
          return;
        }
        DAT_00e38dda = '\x01';
        DAT_00e38dca = 1;
        (&DAT_00ea623c)[DAT_00eaac28 * 0x49d8] = 0;
      }
      else if (DAT_00e38dda == '\x01') goto LAB_00416a08;
      return;
    }
    DAT_00e38dda = '\x01';
  }
LAB_00416a08: ;
  DAT_00d3b198 = param_1;
  FUN_0040a0e0();
  return;
}
