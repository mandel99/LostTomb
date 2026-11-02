#include "restore_types.h"
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
extern undefined4 FUN_00560bfd();
extern int FUN_00560c26();
extern void FUN_00560d96();

void FUN_00560ca6(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_00560bfd();
  if (iVar1 != 0) {
    FUN_00560d96(&DAT_007027d4,param_1);
    FUN_00560c26();
  }
  return;
}
