#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00ea3fc2 (*(undefined4 *)0x00ea3fc2u)
extern int FUN_0045b570();

int FUN_0045b4d0(int param_1,undefined4 param_2,int param_3)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = param_3;
  iVar3 = (int)*(short *)(&DAT_00ea3fc2 + param_1 * 0x49d8);
  param_3 = 1;
  iVar2 = iVar1;
  do {
    if (iVar2 < 1) break;
    iVar3 = iVar3 + 1;
    if (3999 < iVar3) {
      iVar3 = 1;
    }
    if (((&DAT_007999d8)[iVar3 * 0x108] == '\x01') &&
       (((&DAT_007999e8)[iVar3 * 0x84] == 0x48 || ((&DAT_007999e8)[iVar3 * 0x84] == 0xc1)))) {
      *(short *)(&DAT_00ea3fc2 + param_1 * 0x49d8) = (short)iVar3;
      iVar2 = FUN_0045b570(param_1,iVar3,param_2,iVar2);
    }
    param_3 = param_3 + 1;
  } while (param_3 < 4000);
  return iVar1 - iVar2;
}
