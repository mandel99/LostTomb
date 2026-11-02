#include "restore_types.h"
#define DAT_00d3b5c0 (*(int32_t *)0x00d3b5c0u)
#define DAT_00d3b5dc (*(int32_t *)0x00d3b5dcu)
#define DAT_00d3b5e0 (*(int32_t *)0x00d3b5e0u)
#define DAT_00e38dc8 (*(char *)0x00e38dc8u)
extern int FUN_0053e590();

void FUN_004256a0(int param_1,int param_2,short *param_3,int param_4)

{
  if (0 < param_4) {
    do {
      DAT_00d3b5e0 = *param_3 + param_1;
      DAT_00d3b5dc = param_3[1] + param_2;
      if ((*(char *)((int)param_3 + 0xf) == '\x04') || (*(char *)((int)param_3 + 0xf) == '\x05')) {
        DAT_00d3b5c0 = (int)param_3[2];
        if (*(char *)((int)param_3 + 0xd) != '\0') {
          DAT_00d3b5c0 = param_3[2] + -1;
        }
      }
      else {
        DAT_00d3b5c0 = (int)param_3[2];
        if ((char)param_3[6] != '\0') {
          DAT_00d3b5c0 = DAT_00d3b5c0 + 1;
        }
      }
      FUN_0053e590(DAT_00d3b5c0,DAT_00d3b5e0,DAT_00d3b5dc);
      if (*(char *)((int)param_3 + 0xd) != '\0') {
        DAT_00e38dc8 = 1;
      }
      param_3 = param_3 + 0xc;
      param_4 = param_4 + -1;
    } while (param_4 != 0);
  }
  return;
}
