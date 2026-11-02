#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a12 (*(undefined4 *)0x00799a12u)
#define DAT_00799a14 (*(undefined4 *)0x00799a14u)

int FUN_0045d510(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  
  iVar2 = 0;
  iVar1 = 0;
  do {
    param_1 = (int)*(short *)(&DAT_00799a12 + param_1 * 0x108);
    if (param_1 < 1) {
      return 0;
    }
    if (((&DAT_007999ea)[param_1 * 0x84] != 0) &&
       ((short)(&DAT_007999ea)[param_1 * 0x84] == param_2)) {
      iVar1 = iVar1 + (short)(&DAT_00799a14)[param_1 * 0x84];
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 < 8);
  return iVar1;
}
