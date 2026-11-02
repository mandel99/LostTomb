#include "restore_types.h"
extern int * FUN_0044bb90();
extern uint FUN_0044d700();

undefined4 FUN_00446610(void)

{
  char cVar1;
  int iVar2;
  
  iVar2 = FUN_0044bb90(0,0);
  while( true ) {
    if (iVar2 == 0) {
      return 0;
    }
    cVar1 = FUN_0044d700();
    if (cVar1 != '\0') break;
    iVar2 = FUN_0044bb90(iVar2,0);
  }
  return 1;
}
