#include "restore_types.h"
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)

undefined4 FUN_0047e090(int param_1)

{
  unsigned int offset=(unsigned int)param_1 * 4u;
  if ((((*(uint *)(0x00af55fcu + offset) & 0x8000040) != 0) ||
      ((*(uint *)(0x00af5d1cu + offset) & 0x8000040) != 0)) &&
     (((*(uint *)(0x00af5990u + offset) & 0x8000040) != 0 ||
      ((*(uint *)(0x00af5988u + offset) & 0x8000040) != 0)))) {
    return 1;
  }
  return 0;
}
