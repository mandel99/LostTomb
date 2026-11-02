#include "restore_types.h"
extern void FUN_00568cb7();
extern void FUN_00568ce4();

void __cdecl FUN_00564225(int flag,char *argument,char *number)

{
  undefined4 local_c;
  undefined4 local_8;
  
  if (flag != 0) {
    FUN_00568cb7(&local_c);
    *(undefined4 *)argument = local_c;
    *(undefined4 *)(argument + 4) = local_8;
    return;
  }
  FUN_00568ce4(&number,number);
  *(char **)argument = number;
  return;
}
