#include "restore_types.h"
#define DAT_00a9cc1c (*(undefined4 *)0x00a9cc1cu)

int FUN_00433d30(int param_1)

{
  return (int)*(short *)(&DAT_00a9cc1c + param_1 * 2) / 100;
}
