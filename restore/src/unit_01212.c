#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a20 (*(undefined4 *)0x00799a20u)
#define DAT_009ab9e4 (*(undefined4 *)0x009ab9e4u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_007078e0 (*(undefined4 *)0x007078e0u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern int FUN_00475b50();

int FUN_00483000(int param_1,int param_2)

{
  int iVar1;
  short sVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int local_4;
  
  iVar4 = 0;
  iVar3 = param_1 + 2;
  iVar5 = param_1 + -2;
  _DAT_00d3b4e0 = param_2 + -2;
  local_4 = 0;
  DAT_009ab9e4 = 1000;
  if (iVar5 < 0) {
    iVar5 = 0;
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
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar5;
  for (; iVar1 = iVar5, _DAT_00d3b4e0 <= param_1; _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1) {
    while (_DAT_00d3b4e4 = iVar1, _DAT_00d3b4e4 <= iVar3) {
      if (*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2) != 0) {
        iVar4 = FUN_00475b50((int)*(short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2));
        sVar2 = *(short *)(&DAT_00799a20 + iVar4 * 0x108);
        if (DAT_009ab9e4 < sVar2) {
          DAT_009ab9e4 = (int)sVar2;
        }
        sVar2 = sVar2 - (short)_DAT_007078e0;
        *(short *)(&DAT_00799a20 + iVar4 * 0x108) = sVar2;
        if (sVar2 < 0) {
          *(short *)(&DAT_00799a20 + iVar4 * 0x108) = 0;
        }
        if (((0 < iVar4) && (*(char *)((int)&DAT_007999dc + iVar4 * 0x108 + 1) != '\0')) &&
           (0 < (short)(&DAT_007999f4)[iVar4 * 0x84])) {
          local_4 = local_4 + 1;
        }
      }
      DAT_00bc9768 = DAT_00bc9768 + 1;
      iVar4 = local_4;
      iVar1 = _DAT_00d3b4e4 + 1;
    }
    DAT_00bc9768 = DAT_00bc9768 + (iVar5 - iVar3) + 0xe3;
  }
  return iVar4;
}
