#include "restore_types.h"
#define LAB_0056e128 (*(undefined1 *)0x0056e128u)
#define PTR_FUN_00570268 (*(unsigned char * *)0x00570268u)
extern void FUN_0051f780();
extern void FUN_00545fa0();
extern void FUN_00561363();

void FUN_00558ad0(void)

{
  undefined4 *in_ECX;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_0056e128;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *in_ECX = &PTR_FUN_00570268;
  local_4 = 0;
  FUN_00561363(in_ECX + 0x72f,0x34,0x9e,FUN_0051f780);
  local_4 = 0xffffffff;
  FUN_00545fa0();
  ExceptionList = local_c;
  return;
}
