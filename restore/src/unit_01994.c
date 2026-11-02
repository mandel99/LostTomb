#include "restore_types.h"
#define DAT_007999ee (*(undefined4 *)0x007999eeu)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a38 (*(undefined4 *)0x00799a38u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)

void FUN_004e65a0(int param_1)

{
  int *in_ECX;
  
  (&DAT_00799a30)[param_1 * 0x42] = 9;
  *(undefined4 *)(&DAT_00799a3c + param_1 * 0x84) = 1;
  *(undefined4 *)(&DAT_00799a38 + param_1 * 0x108) = 3;
  *(undefined1 *)(&DAT_00799a40 + param_1 * 0x84) = 1;
  *(undefined2 *)(&DAT_007999ee + param_1 * 0x108) = 0xffff;
  (**(code **)(*in_ECX + 0x18))(param_1);
  return;
}
