#include "restore_types.h"
extern void FUN_00568ca1();
extern undefined4 FUN_00569fad();

void FUN_00568ce4(undefined4 param_1,undefined4 param_2)

{
  undefined1 local_10 [12];
  
  FUN_00569fad(local_10,&param_2,param_2,0,0,0,0);
  FUN_00568ca1(local_10,param_1);
  return;
}
