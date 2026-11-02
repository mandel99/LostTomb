#include "restore_types.h"
#define DAT_00af55f8 (*(undefined4 *)0x00af55f8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5600 (*(undefined4 *)0x00af5600u)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d18 (*(undefined4 *)0x00af5d18u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00af5d20 (*(undefined4 *)0x00af5d20u)

char FUN_00477690(uint param_1,int param_2)

{
  unsigned int offset=(unsigned int)param_2 * 4u;
  char cVar1;
  
  cVar1 = (*(uint *)(0x00af55fcu + offset) & param_1) != 0;
  if ((*(uint *)(0x00af5600u + offset) & param_1) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5990u + offset) & param_1) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5d20u + offset) & param_1) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5d1cu + offset) & param_1) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5d18u + offset) & param_1) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af5988u + offset) & param_1) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if ((*(uint *)(0x00af55f8u + offset) & param_1) != 0) {
    cVar1 = cVar1 + '\x01';
  }
  return cVar1;
}
