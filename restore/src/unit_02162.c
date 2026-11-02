#include "restore_types.h"
#define DAT_005d19c8 (*(undefined4 *)0x005d19c8u)
#define DAT_005d19ca (*(undefined4 *)0x005d19cau)
#define DAT_005d19cc (*(undefined4 *)0x005d19ccu)
#define DAT_00784de0 (*(undefined *)0x00784de0u)

int FUN_004f52b0(int param_1)

{
  int iVar1;
  int iVar2;
  
  iVar1 = 0;
  iVar2 = (int)(short)(&DAT_00784de0)[param_1];
  if (((&DAT_005d19c8)[iVar2 * 8] == 2) && ((&DAT_005d19ca)[iVar2 * 8] == 2)) {
    iVar1 = ((&DAT_005d19cc)[iVar2 * 8] != 0) + 0xcf;
  }
  return iVar1;
}
