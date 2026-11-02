#include "restore_types.h"
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
#define DAT_00f55ba4 (*(undefined4 *)0x00f55ba4u)
extern int FUN_0053d3a0();

int FUN_0053c060(void)

{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  char local_1;
  
  local_1 = '\x01';
  iVar1 = FUN_0053d3a0(&DAT_00f55ba4,&local_1);
  uVar3 = 0;
  iVar2 = 0;
  do {
    uVar3 = uVar3 + (int)((uint)(byte)(&DAT_00f55ba4)[iVar2] * 100) / iVar1;
    iVar2 = iVar2 + 1;
  } while (iVar2 < 8);
  iVar2 = 0;
  if (local_1 != '\0') {
    return 9;
  }
  iVar4 = 0;
  do {
    iVar2 = iVar2 + (int)((uint)(byte)(&DAT_00f55ba4)[iVar4] * 100) / iVar1;
    if ((int)(DAT_00d3b374 & uVar3) <= iVar2) goto LAB_0053c0e4;
    iVar4 = iVar4 + 1;
  } while (iVar4 < 8);
  if (iVar2 < (int)(DAT_00d3b374 & uVar3)) {
    iVar4 = 7;
  }
LAB_0053c0e4: ;
  iVar1 = 8;
  if ((&DAT_00f55ba4)[iVar4] != '\x01') {
    iVar1 = iVar4;
  }
  return iVar1;
}
