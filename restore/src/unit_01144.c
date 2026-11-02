#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00af55fc (*(undefined4 *)0x00af55fcu)
#define DAT_00af5988 (*(undefined4 *)0x00af5988u)
#define DAT_00af5990 (*(undefined4 *)0x00af5990u)
#define DAT_00af5d1c (*(undefined4 *)0x00af5d1cu)
#define DAT_00b4e534 (*(undefined4 *)0x00b4e534u)
#define DAT_00b4e6fa (*(undefined4 *)0x00b4e6fau)
#define DAT_00b4e6fe (*(undefined4 *)0x00b4e6feu)
#define DAT_00b4e8c4 (*(undefined4 *)0x00b4e8c4u)
#define DAT_00bc97c0 (*(undefined *)0x00bc97c0u)
#define DAT_00bc97c8 (*(undefined *)0x00bc97c8u)
#define DAT_00bc97d0 (*(undefined *)0x00bc97d0u)
#define DAT_00bc97d8 (*(undefined *)0x00bc97d8u)

char FUN_0047e0d0(int param_1)

{
  char cVar1;
  
  DAT_00bc97c0 = *(uint *)(&DAT_00af55fc + param_1 * 4) & 0x8000040;
  DAT_00bc97c8 = *(uint *)(&DAT_00af5990 + param_1 * 4) & 0x8000040;
  DAT_00bc97d0 = *(uint *)(&DAT_00af5d1c + param_1 * 4) & 0x8000040;
  DAT_00bc97d8 = *(uint *)(&DAT_00af5988 + param_1 * 4) & 0x8000040;
  if (((*(uint *)(&DAT_00af55fc + param_1 * 4) & 8) != 0) &&
     (((&DAT_007999e8)[*(short *)(&DAT_00b4e534 + param_1 * 2) * 0x84] == 0x8a ||
      ((&DAT_007999e8)[*(short *)(&DAT_00b4e534 + param_1 * 2) * 0x84] == 0x88)))) {
    DAT_00bc97c0 = 1;
  }
  if (((*(uint *)(&DAT_00af5990 + param_1 * 4) & 8) != 0) &&
     (((&DAT_007999e8)[*(short *)(&DAT_00b4e6fe + param_1 * 2) * 0x84] == 0x8a ||
      ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fe + param_1 * 2) * 0x84] == 0x88)))) {
    DAT_00bc97c8 = 1;
  }
  if (((*(uint *)(&DAT_00af5d1c + param_1 * 4) & 8) != 0) &&
     (((&DAT_007999e8)[*(short *)(&DAT_00b4e8c4 + param_1 * 2) * 0x84] == 0x8a ||
      ((&DAT_007999e8)[*(short *)(&DAT_00b4e8c4 + param_1 * 2) * 0x84] == 0x88)))) {
    DAT_00bc97d0 = 1;
  }
  if (((*(uint *)(&DAT_00af5988 + param_1 * 4) & 8) != 0) &&
     (((&DAT_007999e8)[*(short *)(&DAT_00b4e6fa + param_1 * 2) * 0x84] == 0x8a ||
      ((&DAT_007999e8)[*(short *)(&DAT_00b4e6fa + param_1 * 2) * 0x84] == 0x88)))) {
    DAT_00bc97d8 = 1;
  }
  cVar1 = DAT_00bc97c0 != 0;
  if (DAT_00bc97c8 != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if (DAT_00bc97d0 != 0) {
    cVar1 = cVar1 + '\x01';
  }
  if (DAT_00bc97d8 != 0) {
    cVar1 = cVar1 + '\x01';
  }
  return cVar1;
}
