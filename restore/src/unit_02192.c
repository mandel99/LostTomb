#include "restore_types.h"
extern int FUN_004f6630();

bool FUN_004f6650(void)

{
  int iVar1;
  int in_ECX;
  
  iVar1 = FUN_004f6630();
  return iVar1 == *(int *)(in_ECX + 0xc) + -1;
}
