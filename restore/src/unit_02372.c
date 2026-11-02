#include "restore_types.h"
extern void FUN_00521aa0();

void FUN_00521a60(undefined4 param_1,undefined4 param_2)

{
  int local_10;
  int local_c;
  undefined1 local_8 [4];
  undefined1 local_4 [4];
  
  FUN_00521aa0(local_4,local_8,&local_10,&local_c);
  *(bool *)param_1 = 0 < local_10;
  *(bool *)param_2 = 0 < local_c;
  return;
}
