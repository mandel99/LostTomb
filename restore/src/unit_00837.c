#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

int FUN_00458c30(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  char *pcVar3;
  int local_8;
  int local_4;
  
  iVar2 = 1;
  local_8 = 1000;
  local_4 = 0;
  iVar1 = 0;
  if (0 < DAT_0078d790) {
    pcVar3 = &DAT_00799ae5;
    do {
      if ((((pcVar3[-5] == '\x01') && (*pcVar3 != '\0')) && (0 < *(short *)(pcVar3 + 0x1b))) &&
         ((0 < *(short *)(pcVar3 + 0x19) && (*(short *)(pcVar3 + 0x29) == 0)))) {
        iVar1 = (int)*(short *)(pcVar3 + 1);
        _DAT_00d3b4f8 = (int)*(short *)(pcVar3 + 3);
        if (iVar1 < param_1) {
          DAT_00d3b4fc = param_1 - iVar1;
        }
        else if (param_1 < iVar1) {
          DAT_00d3b4fc = iVar1 - param_1;
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
        iVar1 = _DAT_00d3b4f8;
        if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
          iVar1 = DAT_00d3b4fc;
        }
        if (iVar1 < local_8) {
          local_8 = iVar1;
          local_4 = iVar2;
        }
      }
      iVar2 = iVar2 + 1;
      pcVar3 = pcVar3 + 0x108;
      iVar1 = local_4;
    } while (iVar2 <= DAT_0078d790);
  }
  return iVar1;
}
