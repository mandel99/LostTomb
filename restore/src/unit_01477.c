#include "restore_types.h"
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00bcb9ce (*(undefined4 *)0x00bcb9ceu)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

int FUN_004b75c0(int param_1)

{
  short sVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  int iVar7;
  int local_4;
  
  iVar2 = param_1 * 0x90;
  local_4 = 0;
  param_1 = 10000;
  iVar3 = 1;
  psVar6 = &DAT_00799af0;
  do {
    if ((((char)psVar6[-8] == '\x01') &&
        (((sVar1 = *psVar6, sVar1 == 0x5e || (sVar1 == 0xb9)) || (sVar1 == 0xba)))) &&
       (*(int *)(&DAT_00705eb8 + sVar1 * 0x28) <= (int)psVar6[0x18])) {
      iVar5 = (int)psVar6[-5];
      iVar7 = (int)psVar6[-4];
      _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + *(short *)(&DAT_00bcb9ce + iVar2) * 0x108);
      iVar4 = (int)*(short *)((int)&DAT_007999dc + *(short *)(&DAT_00bcb9ce + iVar2) * 0x108 + 2);
      if (iVar5 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar5;
      }
      else if (iVar4 < iVar5) {
        DAT_00d3b4fc = iVar5 - iVar4;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar7 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar7;
      }
      else if (_DAT_00d3b4f8 < iVar7) {
        _DAT_00d3b4f8 = iVar7 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar4 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar4 = DAT_00d3b4fc;
      }
      if (iVar4 < param_1) {
        param_1 = iVar4;
        local_4 = iVar3;
      }
    }
    psVar6 = psVar6 + 0x84;
    iVar3 = iVar3 + 1;
  } while ((int)psVar6 < 0x89b6e8);
  return local_4;
}
