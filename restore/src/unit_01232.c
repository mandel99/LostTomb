#include "restore_types.h"
#define DAT_0070ce18 (*(undefined4 *)0x0070ce18u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a26 (*(undefined4 *)0x00799a26u)
#define DAT_009ab9e8 (*(undefined4 *)0x009ab9e8u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_00484a90(int param_1,int param_2)

{
  int iVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  int local_8;
  
  iVar4 = param_2 + 2;
  iVar3 = param_1 + 2;
  param_1 = param_1 + -2;
  _DAT_00d3b4e0 = param_2 + -2;
  local_8 = 0;
  DAT_009ab9e8 = 0;
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar3) {
    iVar3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar4) {
    iVar4 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + param_1;
  if (_DAT_00d3b4e0 <= iVar4) {
    param_2 = (iVar4 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = param_2 + _DAT_00d3b4e0;
    do {
      _DAT_00d3b4e4 = param_1;
      if (param_1 <= iVar3) {
        psVar2 = (short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
        iVar4 = (iVar3 - param_1) + 1;
        DAT_00bc9768 = DAT_00bc9768 + iVar4;
        _DAT_00d3b4e4 = iVar4 + param_1;
        do {
          iVar1 = (int)*psVar2;
          if (((0 < iVar1) && (*(char *)((int)&DAT_007999dc + iVar1 * 0x108 + 1) != '\0')) &&
             (0 < (short)(&DAT_007999f4)[iVar1 * 0x84])) {
            if (DAT_009ab9e8 < *(int *)(&DAT_0070ce18 + (short)(&DAT_007999ea)[iVar1 * 0x84] * 0x58)
               ) {
              DAT_009ab9e8 = *(int *)(&DAT_0070ce18 + (short)(&DAT_007999ea)[iVar1 * 0x84] * 0x58);
            }
            (&DAT_00799a26)[iVar1 * 0x108] = 0x32;
            local_8 = local_8 + 1;
          }
          psVar2 = psVar2 + 1;
          iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
      }
      DAT_00bc9768 = DAT_00bc9768 + (param_1 - iVar3) + 0xe3;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return local_8;
}
