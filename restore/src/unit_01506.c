#include "restore_types.h"
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern undefined4 FUN_0051cba0();

void FUN_004b9ee0(int param_1,int param_2)

{
  char cVar1;
  int iVar2;
  char *pcVar3;
  
  pcVar3 = &DAT_00bcba9a;
  do {
    if ((((pcVar3[-0x42] == '\x01') && (*pcVar3 == '\0')) && (pcVar3[0x27] != '\0')) &&
       (pcVar3[-0x1a] != '\0')) {
      iVar2 = (int)*(short *)(pcVar3 + -0x14);
      _DAT_00d3b4f8 = (int)*(short *)(pcVar3 + -0x12);
      if (iVar2 < param_1) {
        DAT_00d3b4fc = param_1 - iVar2;
      }
      else if (param_1 < iVar2) {
        DAT_00d3b4fc = iVar2 - param_1;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (_DAT_00d3b4f8 < param_2) {
        _DAT_00d3b4f8 = param_2 - _DAT_00d3b4f8;
      }
      else if (param_2 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - param_2;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar2 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar2 = DAT_00d3b4fc;
      }
      if (iVar2 < 7) {
        pcVar3[0xc] = '2';
        pcVar3[0xd] = '\0';
        cVar1 = FUN_0051cba0(param_1,param_2,(int)*(short *)(pcVar3 + -0x14),
                             (int)*(short *)(pcVar3 + -0x12));
        pcVar3[0x34] = cVar1;
      }
    }
    pcVar3 = pcVar3 + 0x90;
  } while ((int)pcVar3 < 0xbcd62a);
  return;
}
