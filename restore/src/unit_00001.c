#include "restore_types.h"
#define LAB_0056b498 (*(undefined1 *)0x0056b498u)
#define PTR_FUN_0056f438 (*(unsigned char * *)0x0056f438u)
extern void FUN_004e5290();
extern void FUN_0051f780();
extern void FUN_00561363();

void FUN_00401390(void)

{
  undefined4 *in_ECX;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_0056b498;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *in_ECX = &PTR_FUN_0056f438;
  local_4 = 0;
  FUN_00561363(in_ECX + 0x13,0x34,0x10,FUN_0051f780);
  local_4 = 0xffffffff;
  FUN_004e5290();
  ExceptionList = local_c;
  return;
}
