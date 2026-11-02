#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799a4f (*(undefined4 *)0x00799a4fu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_0047fd30(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int iVar6;
  int local_4;
  
  iVar6 = (int)*(short *)((int)&DAT_007999dc + param_1 * 0x108 + 2);
  iVar2 = iVar6 + param_2;
  iVar6 = iVar6 - param_2;
  iVar3 = *(short *)(&DAT_007999e0 + param_1 * 0x108) + param_2;
  _DAT_00d3b4e0 = *(short *)(&DAT_007999e0 + param_1 * 0x108) - param_2;
  if (iVar6 < 0) {
    iVar6 = 0;
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
  local_4 = 0;
  iVar4 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar6;
  if (_DAT_00d3b4e0 <= iVar3) {
    do {
      _DAT_00d3b4e4 = iVar6;
      if (iVar6 <= iVar2) {
        psVar5 = (short *)(&DAT_00b4e6fc + iVar4 * 2);
        do {
          if (((0 < *psVar5) &&
              (iVar1 = *psVar5 * 0x108, *(char *)((int)&DAT_007999dc + iVar1 + 1) != '\0')) &&
             (local_4 = local_4 + 1, (&DAT_00799a4f)[iVar1] == '\0')) {
            return 0;
          }
          _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
          iVar4 = iVar4 + 1;
          psVar5 = psVar5 + 1;
        } while (_DAT_00d3b4e4 <= iVar2);
      }
      iVar4 = iVar4 + (iVar6 - iVar2) + 0xe3;
      _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1;
    } while (_DAT_00d3b4e0 <= iVar3);
    if (local_4 != 0) {
      return 1;
    }
  }
  return 2;
}
