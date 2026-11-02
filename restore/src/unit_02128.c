#include "restore_types.h"
extern int FUN_004f27b0();

bool FUN_004f28b0(undefined4 param_1)

{
  int iVar1;
  
  iVar1 = FUN_004f27b0(param_1,&param_1);
  if (iVar1 < 1) {
    return false;
  }
  return (char)param_1 != '\0';
}
