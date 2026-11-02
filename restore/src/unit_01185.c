#include "restore_types.h"
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

undefined4 FUN_00480d60(int param_1)

{
  int iVar1;
  int iVar2;
  int iVar3;
  byte *pbVar4;
  int iVar5;
  short *psVar6;
  int iVar7;
  int local_c;
  
  if (*(short *)(&DAT_00b350dc + *(int *)(&DAT_00bcd618 + param_1 * 0x184) * 2) != param_1) {
    iVar1 = param_1 * 0xc2;
    iVar2 = param_1 * 0xc2;
    local_c = 1;
    do {
      iVar7 = (short)(&DAT_00bcd60c)[iVar1] + local_c;
      param_1 = (short)(&DAT_00bcd60e)[iVar2] + local_c;
      iVar5 = (short)(&DAT_00bcd60c)[iVar1] - local_c;
      _DAT_00d3b4e0 = (short)(&DAT_00bcd60e)[iVar2] - local_c;
      if (iVar5 < 0) {
        iVar5 = 0;
      }
      if (_DAT_00d3b4e0 < 0) {
        _DAT_00d3b4e0 = 0;
      }
      if (DAT_00bca2d0 <= iVar7) {
        iVar7 = DAT_00bca2d0 + -1;
      }
      if (DAT_00bca2d4 <= param_1) {
        param_1 = DAT_00bca2d4 + -1;
      }
      iVar3 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + iVar5;
      for (; _DAT_00d3b4e0 <= param_1; _DAT_00d3b4e0 = _DAT_00d3b4e0 + 1) {
        _DAT_00d3b4e4 = iVar5;
        if (iVar5 <= iVar7) {
          psVar6 = (short *)(&DAT_00b350dc + iVar3 * 2);
          pbVar4 = (byte *)(&DAT_00af598c + iVar3);
          do {
            if (((*pbVar4 & 4) != 0) && (*psVar6 == 0)) {
              return 0;
            }
            _DAT_00d3b4e4 = _DAT_00d3b4e4 + 1;
            iVar3 = iVar3 + 1;
            pbVar4 = pbVar4 + 4;
            psVar6 = psVar6 + 1;
          } while (_DAT_00d3b4e4 <= iVar7);
        }
        iVar3 = iVar3 + (iVar5 - iVar7) + 0xe3;
      }
      local_c = local_c + 1;
    } while (local_c < 6);
  }
  return 1;
}
