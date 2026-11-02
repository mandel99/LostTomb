#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_00484990(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  int local_4;
  
  iVar2 = param_2 + 2;
  _DAT_00d3b4e0 = param_2 + -2;
  iVar1 = param_1 + 2;
  param_1 = param_1 + -2;
  local_4 = 0;
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar1) {
    iVar1 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar2) {
    iVar2 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + param_1;
  if (_DAT_00d3b4e0 <= iVar2) {
    param_2 = (iVar2 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = param_2 + _DAT_00d3b4e0;
    do {
      _DAT_00d3b4e4 = param_1;
      if (param_1 <= iVar1) {
        psVar3 = (short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
        iVar2 = (iVar1 - param_1) + 1;
        DAT_00bc9768 = DAT_00bc9768 + iVar2;
        _DAT_00d3b4e4 = iVar2 + param_1;
        do {
          iVar4 = (int)*psVar3;
          if (((0 < iVar4) && (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) != '\0')) &&
             (0 < (short)(&DAT_007999f4)[iVar4 * 0x84])) {
            local_4 = local_4 + 1;
          }
          psVar3 = psVar3 + 1;
          iVar2 = iVar2 + -1;
        } while (iVar2 != 0);
      }
      DAT_00bc9768 = DAT_00bc9768 + (param_1 - iVar1) + 0xe3;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return local_4;
}
