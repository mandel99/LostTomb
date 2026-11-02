#include "restore_types.h"
#define DAT_005d19c8 (*(undefined4 *)0x005d19c8u)
#define DAT_005d19ca (*(undefined4 *)0x005d19cau)
#define DAT_00784de0 (*(undefined *)0x00784de0u)

int FUN_004f44e0(void)

{
  int iVar1;
  short *psVar2;
  
  psVar2 = &DAT_00784de0;
  while ((iVar1 = (int)*psVar2, iVar1 == 0 || ((&DAT_005d19c8)[iVar1 * 8] != 1))) {
    psVar2 = psVar2 + 1;
    if (0x784de5 < (int)psVar2) {
      return 4;
    }
  }
  return (int)(short)(&DAT_005d19ca)[iVar1 * 8];
}
