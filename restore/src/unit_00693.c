#include "restore_types.h"
extern undefined1 FUN_00449370();
extern undefined1 FUN_00449380();

uint FUN_00449390(void)

{
  char cVar1;
  uint uVar2;
  
  uVar2 = FUN_00449370();
  if ((char)uVar2 != '\0') {
    cVar1 = FUN_00449380();
    return (uint)(cVar1 == '\0');
  }
  return uVar2 & 0xffffff00;
}
