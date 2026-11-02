#include "restore_types.h"
extern int FUN_00488030();
extern uint FUN_004894a0();

undefined4 FUN_00488050(undefined4 param_1)

{
  char cVar1;
  
  cVar1 = FUN_00488030(param_1);
  if (cVar1 != '\0') {
    cVar1 = FUN_004894a0(param_1);
    if (cVar1 != '\0') {
      return 1;
    }
  }
  return 0;
}
