#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)

char FUN_0045d570(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = 0;
  iVar2 = 0;
  iVar3 = 0;
  do {
    param_1 = (int)*(short *)(&DAT_00799a12 + param_1 * 0x108);
    if (param_1 < 1) {
      return '\0';
    }
    if ((&DAT_007999ea)[param_1 * 0x84] == 0) {
      iVar3 = iVar3 + 1;
    }
    else {
      iVar2 = iVar2 + (short)(&DAT_00799a14)[param_1 * 0x84];
    }
    iVar1 = iVar1 + 1;
  } while (iVar1 < 8);
  if (0 < iVar3) {
    return '\0';
  }
  return (iVar2 < 0xc80) + '\x01';
}
