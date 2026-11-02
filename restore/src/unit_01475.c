#include "restore_types.h"
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

void FUN_004b7510(int param_1,int param_2)

{
  int iVar1;
  char *pcVar2;
  
  pcVar2 = &DAT_00bcd630;
  do {
    if (((pcVar2[-0x2a] == '\x01') && (pcVar2[-0x2e] == '*')) && (*pcVar2 != '\x04')) {
      iVar1 = (int)*(short *)(pcVar2 + -0x24);
      _DAT_00d3b4f8 = (int)*(short *)(pcVar2 + -0x22);
      if (param_1 < iVar1) {
        DAT_00d3b4fc = iVar1 - param_1;
      }
      else if (iVar1 < param_1) {
        DAT_00d3b4fc = param_1 - iVar1;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (param_2 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - param_2;
      }
      else if (_DAT_00d3b4f8 < param_2) {
        _DAT_00d3b4f8 = param_2 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar1 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar1 = DAT_00d3b4fc;
      }
      if (iVar1 < 2) {
        pcVar2[-0x2a] = '\x02';
      }
    }
    pcVar2 = pcVar2 + 0x184;
  } while ((int)pcVar2 < 0xc8ad70);
  return;
}
