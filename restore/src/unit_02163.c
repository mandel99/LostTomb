#include "restore_types.h"
#define DAT_005d19c8 (*(undefined4 *)0x005d19c8u)
#define DAT_005d19ca (*(undefined4 *)0x005d19cau)
#define DAT_00784de0 (*(undefined *)0x00784de0u)

undefined4 FUN_004f52f0(int param_1)

{
  short sVar1;
  int iVar2;
  undefined3 uVar3;
  
  iVar2 = (int)(short)(&DAT_00784de0)[param_1];
  uVar3 = (undefined3)((uint)(&DAT_005d19c8 + iVar2 * 8) >> 8);
  if (((&DAT_005d19c8)[iVar2 * 8] == 1) &&
     ((((sVar1 = (&DAT_005d19ca)[iVar2 * 8], sVar1 == 1 || (sVar1 == 2)) || (sVar1 == 3)) ||
      ((sVar1 == 4 || (sVar1 == 5)))))) {
    return CONCAT31(uVar3,1);
  }
  return CONCAT31(uVar3,(&DAT_005d19ca)[iVar2 * 8] == 4);
}
