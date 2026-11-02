#include "restore_types.h"
#define DAT_005d25ac (*(undefined4 *)0x005d25acu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a22 (*(undefined4 *)0x00799a22u)
#define DAT_00e94e0c (*(undefined *)0x00e94e0cu)

uint FUN_00431690(int param_1)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  
  if (DAT_00e94e0c == 0x33) {
    return 1;
  }
  if ((DAT_00e94e0c == 8) && (iVar1 = 0, *(short *)(&DAT_00799a22 + param_1 * 0x108) != 0)) {
LAB_004316b8: ;
    return CONCAT31((int3)((uint)iVar1 >> 8),1);
  }
  piVar3 = *(int **)(&DAT_005d25ac + DAT_00e94e0c * 4);
  uVar2 = DAT_00e94e0c;
  if ((piVar3 != (int *)0x0) && (iVar1 = *piVar3, uVar2 = 0, iVar1 != 0)) {
    do {
      if (iVar1 == (short)(&DAT_007999e8)[param_1 * 0x84]) goto LAB_004316b8;
      iVar1 = piVar3[1];
      piVar3 = piVar3 + 1;
      uVar2 = 0;
    } while (iVar1 != 0);
  }
  return uVar2 & 0xffffff00;
}
