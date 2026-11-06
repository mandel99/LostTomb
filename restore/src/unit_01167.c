#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a17 (*(undefined4 *)0x00799a17u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(int32_t *)0x00bca2d0u)
#define DAT_00bca2d4 (*(int32_t *)0x00bca2d4u)
#define DAT_00bca2d8 (*(uint32_t *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

void FUN_0047fc40(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  short *psVar4;
  int iVar5;
  
  iVar2 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2);
  iVar3 = iVar2 + param_2;
  iVar2 = iVar2 - param_2;
  iVar1 = *(short *)(&DAT_007999e0 + param_1 * 0x108) + param_2;
  _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + param_1 * 0x108) - param_2;
  if (iVar2 < 0) {
    iVar2 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= iVar3) {
    iVar3 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar1) {
    iVar1 = DAT_00bca2d4 + -1;
  }
  iVar5 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar2;
  if (_DAT_00d3b4e0 <= iVar1) {
    param_2 = (iVar1 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = param_2 + _DAT_00d3b4e0;
    do {
      _DAT_00d3b4e4 = iVar2;
      if (iVar2 <= iVar3) {
        psVar4 = (short *)(&DAT_00b4e6fc + iVar5 * 2);
        iVar1 = (iVar3 - iVar2) + 1;
        iVar5 = iVar5 + iVar1;
        _DAT_00d3b4e4 = iVar1 + iVar2;
        do {
          if (*psVar4 != 0) {
            (&DAT_00799a17)[*psVar4 * 0x108] = 1;
          }
          psVar4 = psVar4 + 1;
          iVar1 = iVar1 + -1;
        } while (iVar1 != 0);
      }
      iVar5 = iVar5 + (iVar2 - iVar3) + 0xe3;
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
