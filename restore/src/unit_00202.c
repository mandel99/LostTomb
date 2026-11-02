#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)

undefined4 FUN_00417490(int param_1,int *param_2,int *param_3)

{
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  switch(DAT_00bca2e0 / 2) {
  case 0: ;
    *param_2 = *(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) + 8;
    *param_3 = *(short *)(&DAT_007999e0 + param_1 * 0x108) + 0xf;
    return 1;
  case 1: ;
    *param_2 = *(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) + -1;
    *param_3 = *(short *)(&DAT_007999e0 + param_1 * 0x108) + 8;
    return 1;
  case 2: ;
    *param_2 = *(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) + 6;
    *param_3 = *(short *)(&DAT_007999e0 + param_1 * 0x108) + -1;
    return 1;
  case 3: ;
    *param_2 = *(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2) + 0xf;
    *param_3 = *(short *)(&DAT_007999e0 + param_1 * 0x108) + 6;
  }
  return 1;
}
