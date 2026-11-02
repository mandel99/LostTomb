#include "restore_types.h"
#define DAT_0070ce0c (*(undefined4 *)0x0070ce0cu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799abc (*(undefined4 *)0x00799abcu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_009ab9dc (*(undefined4 *)0x009ab9dcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

void FUN_00483260(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  
  iVar5 = param_2 + 2;
  _DAT_00d3b4e0 = param_2 + -2;
  _DAT_009ab9dc = 0;
  iVar4 = param_1 + 2;
  param_1 = param_1 + -2;
  if (param_1 < 0) {
    param_1 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar4) {
    iVar4 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar5) {
    iVar5 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + param_1;
  if (_DAT_00d3b4e0 <= iVar5) {
    param_2 = (iVar5 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = param_2 + _DAT_00d3b4e0;
    do {
      _DAT_00d3b4e4 = param_1;
      if (param_1 <= iVar4) {
        psVar3 = (short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
        iVar5 = (iVar4 - param_1) + 1;
        DAT_00bc9768 = DAT_00bc9768 + iVar5;
        _DAT_00d3b4e4 = iVar5 + param_1;
        do {
          iVar2 = (int)*psVar3;
          if (iVar2 != 0) {
            iVar1 = (&DAT_00799abc)[iVar2 * 0x42];
            if (_DAT_009ab9dc < iVar1) {
              _DAT_009ab9dc = iVar1;
            }
            (&DAT_00799abc)[iVar2 * 0x42] = iVar1 - param_3;
            if (iVar1 - param_3 <
                *(int *)(&DAT_0070ce0c + (short)(&DAT_007999ea)[iVar2 * 0x84] * 0x58)) {
              (&DAT_00799abc)[iVar2 * 0x42] =
                   *(int *)(&DAT_0070ce0c + (short)(&DAT_007999ea)[iVar2 * 0x84] * 0x58);
            }
            if (1000 < (int)(&DAT_00799abc)[iVar2 * 0x42]) {
              (&DAT_00799abc)[iVar2 * 0x42] = 1000;
            }
          }
          psVar3 = psVar3 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
      }
      DAT_00bc9768 = DAT_00bc9768 + (param_1 - iVar4) + 0xe3;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
