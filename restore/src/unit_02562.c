#include "restore_types.h"
#define LAB_0056bd88 (*(undefined1 *)0x0056bd88u)
#define PTR_FUN_00570160 (*(unsigned char * *)0x00570160u)
extern void FUN_0051f780();
extern void FUN_00545fa0();
extern void FUN_00561363();

void FUN_00545630(void)

{
  undefined4 *in_ECX;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_0056bd88;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *in_ECX = &PTR_FUN_00570160;
  local_4 = 0;
  FUN_00561363(in_ECX + 0x797,0x34,0xa7,FUN_0051f780);
  local_4 = 0xffffffff;
  FUN_00545fa0();
  ExceptionList = local_c;
  return;
}
