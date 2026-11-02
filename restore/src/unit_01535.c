#include "restore_types.h"
#define DAT_00bcba4a (*(undefined4 *)0x00bcba4au)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd72a (*(undefined4 *)0x00bcd72au)

uint FUN_004bce90(int param_1)

{
  uint uVar1;
  
  uVar1 = param_1 * 0x184;
  switch((&DAT_00bcd602)[uVar1]) {
  case 0xb: ;
  case 0xc: ;
  case 0xd: ;
    uVar1 = (short)(&DAT_00bcd658)[param_1 * 0xc2] * 0x90;
    if (-1 < *(short *)(&DAT_00bcba4a + uVar1)) {
      return CONCAT31((int3)(uVar1 >> 8),1);
    }
    break;
  case 0x4d: ;
  case 0x4e: ;
    if (-1 < *(short *)(&DAT_00bcd72a + uVar1)) {
      return CONCAT31((int3)(uVar1 >> 8),1);
    }
  }
  return uVar1 & 0xffffff00;
}
