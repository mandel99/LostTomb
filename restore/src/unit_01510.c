#include "restore_types.h"
#define DAT_00a835fc (*(undefined4 *)0x00a835fcu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

void FUN_004ba180(int param_1,int param_2,int param_3,char param_4)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  
  iVar3 = param_1 + param_3;
  param_1 = param_1 - param_3;
  iVar4 = param_2 + param_3;
  _DAT_00d3b4e0 = param_2 - param_3;
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
  iVar2 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + param_1;
  if (_DAT_00d3b4e0 <= iVar4) {
    param_3 = (iVar4 - _DAT_00d3b4e0) + 1;
    _DAT_00d3b4e0 = param_3 + _DAT_00d3b4e0;
    do {
      _DAT_00d3b4e4 = param_1;
      if (param_1 <= iVar3) {
        psVar5 = (short *)(&DAT_00b350dc + iVar2 * 2);
        iVar4 = (iVar3 - param_1) + 1;
        _DAT_00d3b4e4 = iVar4 + param_1;
        do {
          cVar1 = (&DAT_00a835fc)[iVar2];
          (&DAT_00a835fc)[iVar2] = cVar1 + param_4;
          if (((0 < *psVar5) && (10 < (byte)(&DAT_00bcd602)[*psVar5 * 0x184])) &&
             ((byte)(&DAT_00bcd602)[*psVar5 * 0x184] < 0xe)) {
            (&DAT_00a835fc)[iVar2] = cVar1 + param_4 + '\x02';
          }
          iVar2 = iVar2 + 1;
          psVar5 = psVar5 + 1;
          iVar4 = iVar4 + -1;
        } while (iVar4 != 0);
      }
      iVar2 = iVar2 + (param_1 - iVar3) + 0xe3;
      param_3 = param_3 + -1;
    } while (param_3 != 0);
  }
  return;
}
