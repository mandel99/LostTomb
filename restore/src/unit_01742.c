#include "restore_types.h"
#define DAT_00e38aec (*(char *)0x00e38aecu)
extern void FUN_004cbea0();
extern void FUN_004ce370();

void FUN_004ce310(undefined4 param_1,undefined4 param_2)

{
  FUN_004cbea0(param_2,&DAT_00e38aec,3);
  FUN_004ce370(param_1);
  return;
}
