#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a9a (*(undefined4 *)0x00799a9au)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_00484890(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  
  iVar3 = param_2 + 4;
  _DAT_00d3b4e0 = param_2 + -4;
  iVar2 = param_1 + 4;
  param_1 = param_1 + -4;
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar2) {
    iVar2 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar3) {
    iVar3 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + param_1;
  if (_DAT_00d3b4e0 <= iVar3) {
    param_2 = (iVar3 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = param_2 + _DAT_00d3b4e0;
    do {
      _DAT_00d3b4e4 = param_1;
      if (param_1 <= iVar2) {
        psVar4 = (short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
        iVar3 = (iVar2 - param_1) + 1;
        DAT_00bc9768 = DAT_00bc9768 + iVar3;
        _DAT_00d3b4e4 = iVar3 + param_1;
        do {
          iVar1 = (int)*psVar4;
          if (0 < iVar1) {
            if (((&DAT_007999e8)[iVar1 * 0x84] == 0x58) || ((&DAT_007999e8)[iVar1 * 0x84] == 0x59))
            {
              *(undefined1 *)(&DAT_00799a9a + iVar1 * 0x84) = 0;
            }
          }
          psVar4 = psVar4 + 1;
          iVar3 = iVar3 + -1;
        } while (iVar3 != 0);
      }
      DAT_00bc9768 = DAT_00bc9768 + (param_1 - iVar2) + 0xe3;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return 1;
}
