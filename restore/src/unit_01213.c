#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_00483160(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  
  iVar3 = param_2 + 2;
  _DAT_00d3b4e0 = param_2 + -2;
  iVar2 = param_1 + 2;
  param_1 = param_1 + -2;
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
          if (iVar1 != 0) {
            if (((9 < (short)(&DAT_007999e8)[iVar1 * 0x84]) &&
                ((short)(&DAT_007999e8)[iVar1 * 0x84] < 0x1e)) &&
               ((&DAT_007999d8)[iVar1 * 0x108] != '\0')) {
              (&DAT_00799a5f)[iVar1 * 0x108] = 0;
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
  return 0;
}
