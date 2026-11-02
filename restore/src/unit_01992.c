#include "restore_types.h"
#define LAB_0056b718 (*(undefined1 *)0x0056b718u)
#define PTR_FUN_0056f6e8 (*(unsigned char * *)0x0056f6e8u)
extern void FUN_004e5290();
extern void FUN_0051f780();
extern void FUN_00561363();

void FUN_004e6530(void)

{
  undefined4 *in_ECX;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_0056b718;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *in_ECX = &PTR_FUN_0056f6e8;
  local_4 = 0;
  FUN_00561363(in_ECX + 0x13,0x34,2,FUN_0051f780);
  local_4 = 0xffffffff;
  FUN_004e5290();
  ExceptionList = local_c;
  return;
}
