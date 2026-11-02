#include "restore_types.h"
#define DAT_00af5984 (*(undefined4 *)0x00af5984u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

undefined4 FUN_00480e90(int param_1)

{
  byte *pbVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int local_14;
  int local_10;
  
  if (((*(byte *)(&DAT_00af598c + *(int *)(&DAT_00bcd618 + param_1 * 0x184)) & 4) == 0) ||
     (*(short *)(&DAT_00b350dc + *(int *)(&DAT_00bcd618 + param_1 * 0x184) * 2) != param_1)) {
    local_10 = 1;
    do {
      iVar4 = (short)(&DAT_00bcd60c)[param_1 * 0xc2] + local_10;
      local_14 = (short)(&DAT_00bcd60e)[param_1 * 0xc2] + local_10;
      iVar3 = (short)(&DAT_00bcd60c)[param_1 * 0xc2] - local_10;
      _DAT_00d3b4e0 = (short)(&DAT_00bcd60e)[param_1 * 0xc2] - local_10;
      if (iVar3 < 0) {
        iVar3 = 0;
      }
      if (_DAT_00d3b4e0 < 0) {
        _DAT_00d3b4e0 = 0;
      }
      if (DAT_00bca2d0 <= iVar4) {
        iVar4 = DAT_00bca2d0 + -1;
      }
      if (DAT_00bca2d4 <= local_14) {
        local_14 = DAT_00bca2d4 + -1;
      }
      iVar2 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar3;
      for (; _DAT_00d3b4e0 <= local_14; _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1) {
        _DAT_00d3b4e4 = iVar3;
        if (iVar3 <= iVar4) {
          psVar5 = (short *)(&DAT_00b350dc + iVar2 * 2);
          pbVar1 = &DAT_00af5984 + iVar2 * 4;
          do {
            if ((((pbVar1[8] & 4) != 0) &&
                (((*psVar5 == 0 || (*psVar5 == param_1)) && ((pbVar1[-0x718] & 4) != 0)))) &&
               ((((pbVar1[0x10] & 4) != 0 && ((pbVar1[0x728] & 4) != 0)) && ((*pbVar1 & 4) != 0))))
            {
              return 1;
            }
            _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
            iVar2 = iVar2 + 1;
            psVar5 = psVar5 + 1;
            pbVar1 = pbVar1 + 4;
          } while (_DAT_00d3b4e4 <= iVar4);
        }
        iVar2 = iVar2 + (iVar3 - iVar4) + 0xe3;
      }
      local_10 = local_10 + 1;
    } while (local_10 < 6);
  }
  return 0;
}
