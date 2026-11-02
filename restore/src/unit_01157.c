#include "restore_types.h"
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)

bool FUN_0047ed30(int param_1,uint param_2)

{
  unsigned int offset=(unsigned int)param_1 * 4u;
  if ((*(uint *)(0x00af55fcu + offset) & param_2) != 0) {
    return true;
  }
  return (*(uint *)(0x00af5d1cu + offset) & param_2) != 0;
}
