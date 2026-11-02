#include "restore_types.h"
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)

uint FUN_0047ed60(int param_1)

{
  unsigned int offset=(unsigned int)param_1 * 4u;
  if ((*(uint *)(0x00af55fcu + offset) & 4) == 0) {
    return 0;
  }
  if ((*(uint *)(0x00af5990u + offset) & 4) == 0) {
    return 0;
  }
  if ((*(uint *)(0x00af5d1cu + offset) & 4) == 0) {
    return 0;
  }
  return (*(uint *)(0x00af5988u + offset) & 0xff) >> 2 & 1;
}
