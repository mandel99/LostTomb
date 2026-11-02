#include "restore_types.h"
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
extern void FUN_00469140();

void FUN_004690f0(int param_1,int param_2)

{
  *(short *)(&DAT_00a9cc1c + param_1 * 2) = *(short *)(&DAT_00a9cc1c + param_1 * 2) + 1;
  if (param_2 < *(short *)(&DAT_00a9cc1c + param_1 * 2)) {
    FUN_00469140((param_1 - DAT_00bca2d8) % 0xe4,(param_1 - DAT_00bca2d8) / 0xe4,param_1);
  }
  return;
}
