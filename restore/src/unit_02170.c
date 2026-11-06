#include "restore_types.h"
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00ea6160 (*(undefined4 *)0x00ea6160u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

int FUN_004f5770(int param_1,int param_2)

{
  int iVar1;
  uint3 uVar2;
  
  param_1 = DAT_00eaac28 * 0x1276 + param_1;
  iVar1 = param_1 * 4;
  uVar2 = (uint3)((uint)iVar1 >> 8);
  if ((param_2 <= (int)(&DAT_00ea6160)[param_1]) &&
     ((param_2 < 1 || (*(int *)(0x00ea42f8u + (uint)iVar1) == 0)))) {
    return (uint)uVar2 << 8;
  }
  return CONCAT31(uVar2,1);
}
