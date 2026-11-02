#include "restore_types.h"
#define DAT_00bcd631 (*(undefined4 *)0x00bcd631u)
extern void FUN_004b3f00();

void FUN_004b28a0(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  
  if (0 < param_2) {
    iVar2 = param_1 * 0x184;
    do {
      cVar1 = (&DAT_00bcd631)[iVar2];
      (&DAT_00bcd631)[iVar2] = cVar1 + '\x01';
      if ((char)(cVar1 + '\x01') < '\x0f') {
        FUN_004b3f00(param_1);
      }
      else {
        (&DAT_00bcd631)[iVar2] = 0xf;
      }
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
