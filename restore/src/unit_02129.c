#include "restore_types.h"
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern int FUN_004f27b0();

uint FUN_004f28e0(undefined4 param_1)

{
  int iVar1;
  uint uVar2;
  
  iVar1 = FUN_004f27b0(param_1,&param_1);
  if (iVar1 < 1) {
    return 0xffffffff;
  }
  uVar2 = (uint)(short)(&DAT_00799a3c)[iVar1 * 0x84];
  if (uVar2 != 0) {
    iVar1 = uVar2 * 0x184;
    if ((&DAT_00bcd602)[iVar1] == 'P') {
      return ((&DAT_00bcd630)[iVar1] != '\v') - 1 & uVar2;
    }
    if ((&DAT_00bcd602)[iVar1] == 'Q') {
      return ((&DAT_00bcd630)[iVar1] != '\n') - 1 & uVar2;
    }
  }
  return 0;
}
