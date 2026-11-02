#include "restore_types.h"
#define LAB_0056e171 (*(undefined1 *)0x0056e171u)
extern void FUN_0055dbf8();
extern undefined4 FUN_0055e92b();

void FUN_0055e58a(CAtlWinModule *this)

{
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_0056e171;
  local_10 = ExceptionList;
  local_8 = 0;
  ExceptionList = &local_10;
  FUN_0055e92b(this);
  local_8 = 0xffffffff;
  FUN_0055dbf8();
  ExceptionList = local_10;
  return;
}
