#include "restore_types.h"
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)

bool FUN_0047ed00(int param_1,uint param_2)

{
  unsigned int offset=(unsigned int)param_1 * 4u;
  if ((*(uint *)(0x00af5990u + offset) & param_2) != 0) {
    return true;
  }
  return (*(uint *)(0x00af5988u + offset) & param_2) != 0;
}
