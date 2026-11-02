#include "restore_types.h"
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)

int FUN_004a34f0(int param_1,int *param_2)

{
  short sVar1;
  int iVar2;
  int iVar3;
  
  iVar3 = 0;
  sVar1 = *(short *)(&DAT_00bcd660 + param_1 * 0x184);
  *param_2 = 1;
  do {
    iVar2 = sVar1 * 0x184;
    if ((&DAT_00bcd602)[iVar2] == 'U') {
      return (int)sVar1;
    }
    iVar3 = iVar3 + 1;
    *param_2 = *param_2 + 1;
    sVar1 = *(short *)(&DAT_00bcd660 + iVar2);
  } while (iVar3 < 8);
  return 0;
}
