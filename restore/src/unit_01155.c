#include "restore_types.h"
#define DAT_00af55f8 (*(undefined4 *)0x00af55f8u)
#define DAT_00af5600 (*(undefined4 *)0x00af5600u)
#define DAT_00af5d18 (*(undefined4 *)0x00af5d18u)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)

char FUN_0047ecc0(int param_1,uint param_2)

{
  unsigned int offset=(unsigned int)param_1 * 4u;
  char cVar1;
  
  cVar1 = (*(uint *)(0x00af5600u + offset) & param_2) != 0;
  if ((*(uint *)(0x00af5d20u + offset) & param_2) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5d18u + offset) & param_2) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af55f8u + offset) & param_2) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  return cVar1;
}
