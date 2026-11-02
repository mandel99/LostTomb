#include "restore_types.h"
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int FUN_004e92c0();

int FUN_004e9460(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = FUN_004e92c0(DAT_00eaac28,2);
  iVar2 = FUN_004e92c0(DAT_00eaac28,3);
  iVar3 = FUN_004e92c0(DAT_00eaac28,4);
  return iVar3 + iVar1 + iVar2;
}
