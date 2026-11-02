#include "restore_types.h"
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)

int FUN_00492360(int param_1)

{
  int iVar1;
  
  iVar1 = (int)*(short *)(&DAT_00bcd660 + param_1 * 0x184);
  if ((&DAT_00bcd602)[iVar1 * 0x184] != '\x13') {
    if ((&DAT_00bcd602)[iVar1 * 0x184] == '\x15') {
      return (int)*(short *)(&DAT_00bcd660 + iVar1 * 0x184);
    }
    iVar1 = 0;
  }
  return iVar1;
}
