#include "restore_types.h"
#define DAT_00605b38 (*(undefined4 *)0x00605b38u)
#define DAT_010d6a16 (*(undefined4 *)0x010d6a16u)
#define DAT_010d6a28 (*(undefined4 *)0x010d6a28u)
#define DAT_010d6a32 (*(undefined4 *)0x010d6a32u)
#define DAT_0126517c (*(undefined *)0x0126517cu)

uint FUN_0052f790(int param_1,uint param_2)

{
  uint uVar1;
  int iVar2;
  
  if ((int)param_2 < 1) {
    param_2 = 1;
  }
  iVar2 = param_1;
  if (DAT_0126517c <= param_1) {
    iVar2 = 0;
  }
  if ((&DAT_00605b38)[(byte)(&DAT_010d6a32)[iVar2 * 0x40]] != '\0') {
    iVar2 = param_1;
    if (DAT_0126517c <= param_1) {
      iVar2 = 0;
    }
    if ((&DAT_010d6a28)[iVar2 * 0x40] == '\0') {
      if (DAT_0126517c <= param_1) {
        param_1 = 0;
      }
      if ((int)(param_2 + 1) <= (int)(short)(&DAT_010d6a16)[param_1 * 0x20]) {
        return param_2 + 1;
      }
    }
    else {
      if ((param_2 & 0x80) == 0) {
        uVar1 = (param_2 & 0x7f) + 1;
        iVar2 = param_1;
        if (DAT_0126517c <= param_1) {
          iVar2 = 0;
        }
        if ((int)uVar1 <= (int)(short)(&DAT_010d6a16)[iVar2 * 0x20]) {
          return uVar1;
        }
        if (DAT_0126517c <= param_1) {
          param_1 = 0;
        }
        return CONCAT31((int3)(char)((ushort)(&DAT_010d6a16)[param_1 * 0x20] >> 8),
                        (char)(&DAT_010d6a16)[param_1 * 0x20]) | 0x80;
      }
      uVar1 = (param_2 & 0x7f) - 1;
      if (0 < (int)uVar1) {
        return uVar1 | 0x80;
      }
    }
    param_2 = 1;
  }
  return param_2;
}
