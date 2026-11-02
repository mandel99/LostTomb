#include "restore_types.h"
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd7b4 (*(undefined4 *)0x00bcd7b4u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern undefined4 FUN_004b4420();

int FUN_004bcd90(int param_1,int param_2,int param_3)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  int iVar4;
  int local_4;
  
  local_4 = 0;
  DAT_00bcd5ec = param_3;
  param_3 = 1;
  pcVar2 = &DAT_00bcd7b4;
  do {
    if (((pcVar2[-0x2a] == '\x01') && (*pcVar2 != '\x04')) && (pcVar2[-0x2e] == ']')) {
      iVar1 = (int)*(short *)(pcVar2 + -0x24);
      iVar4 = (int)*(short *)(pcVar2 + -0x22);
      if (iVar1 < param_1) {
        DAT_00d3b4fc = param_1 - iVar1;
      }
      else if (param_1 < iVar1) {
        DAT_00d3b4fc = iVar1 - param_1;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar4 < param_2) {
        _DAT_00d3b4f8 = param_2 - iVar4;
      }
      else if (param_2 < iVar4) {
        _DAT_00d3b4f8 = iVar4 - param_2;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar3 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar3 = DAT_00d3b4fc;
      }
      if (iVar3 < DAT_00bcd5ec) {
        iVar1 = FUN_004b4420(param_1,param_2,iVar1,iVar4);
        if (iVar1 != 0) {
          local_4 = param_3;
          DAT_00bcd5ec = iVar3;
        }
      }
    }
    pcVar2 = pcVar2 + 0x184;
    param_3 = param_3 + 1;
  } while ((int)pcVar2 < 0xc8ad70);
  if (local_4 == 0) {
    return 0;
  }
  _DAT_00d3b4e0 = (int)(short)(&DAT_00bcd60e)[local_4 * 0xc2];
  _DAT_00d3b4e4 = (int)(short)(&DAT_00bcd60c)[local_4 * 0xc2];
  return local_4;
}
