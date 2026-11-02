#include "restore_types.h"
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a5f (*(undefined4 *)0x00799a5fu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern void FUN_0042fb30();

void FUN_0042ff20(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  _DAT_00d3b4e0 = param_2 + -2;
  iVar3 = param_1 + 2;
  iVar2 = param_1 + -2;
  if (iVar2 < 0) {
    iVar2 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar3) {
    iVar3 = DAT_00bca2d0 + -1;
  }
  param_1 = param_2 + 2;
  if (DAT_00bca2d4 <= param_2 + 2) {
    param_1 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar2;
  for (; iVar1 = iVar2, _DAT_00d3b4e0 <= param_1; _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1) {
    while (_DAT_00d3b4e4 = iVar1, _DAT_00d3b4e4 <= iVar3) {
      iVar1 = (int)*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
      if ((((iVar1 != 0) && ((&DAT_007999d8)[iVar1 * 0x108] != '\0')) &&
          (9 < (short)(&DAT_007999e8)[iVar1 * 0x84])) &&
         ((((short)(&DAT_007999e8)[iVar1 * 0x84] < 0x1e && ((&DAT_007999f4)[iVar1 * 0x84] != 0)) &&
          ((&DAT_00799a5f)[iVar1 * 0x108] == '\0')))) {
        FUN_0042fb30(iVar1);
      }
      DAT_00bc9768 = DAT_00bc9768 + 1;
      iVar1 = _DAT_00d3b4e4 + 1;
    }
    DAT_00bc9768 = DAT_00bc9768 + (iVar2 - iVar3) + 0xe3;
  }
  return;
}
