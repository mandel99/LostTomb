#include "restore_types.h"
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)

char FUN_0047ec80(int param_1,uint param_2)

{
  unsigned int offset=(unsigned int)param_1 * 4u;
  char cVar1;
  
  cVar1 = (*(uint *)(0x00af55fcu + offset) & param_2) != 0;
  if ((*(uint *)(0x00af5990u + offset) & param_2) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5d1cu + offset) & param_2) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5988u + offset) & param_2) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  return cVar1;
}
