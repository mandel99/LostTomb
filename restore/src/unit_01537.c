#include "restore_types.h"
#define DAT_007090e4 (*(undefined4 *)0x007090e4u)
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd792 (*(undefined4 *)0x00bcd792u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern undefined4 FUN_004b4420();
extern uint FUN_004bce90();

int FUN_004bcf70(int param_1,int param_2,int param_3)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *local_c;
  int local_8;
  int local_4;
  
  local_4 = 0;
  DAT_00bcd5ec = param_3;
  local_8 = 0;
  param_3 = 1;
  local_c = &DAT_00bcd792;
  do {
    if ((((char)local_c[-4] != '\x01') || (cVar1 = FUN_004bce90(param_3), cVar1 != '\0')) ||
       ((char)local_c[0x11] == '\x04')) goto switchD_004bcfe2_caseD_e;
    switch((char)local_c[-6]) {
    case '\v': ;
    case '\f': ;
    case '\r': ;
      iVar2 = (int)local_c[-1];
      iVar3 = (int)*local_c;
      if (iVar2 < param_1) {
        DAT_00d3b4fc = param_1 - iVar2;
      }
      else if (param_1 < iVar2) {
        DAT_00d3b4fc = iVar2 - param_1;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar3 < param_2) {
        _DAT_00d3b4f8 = param_2 - iVar3;
      }
      else if (param_2 < iVar3) {
        _DAT_00d3b4f8 = iVar3 - param_2;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar4 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar4 = _DAT_00d3b4f8;
      }
      if (iVar4 <= *(int *)(&DAT_007090e4 + (uint)(byte)(&DAT_00bcd602)[DAT_00d056f8 * 0x184] * 0x38
                           )) {
        iVar5 = 0x11;
        break;
      }
default: ;
      goto switchD_004bcfe2_caseD_e;
    case '\x14': ;
    case '\x19': ;
      iVar2 = (int)local_c[-1];
      iVar3 = (int)*local_c;
      if (iVar2 < param_1) {
        DAT_00d3b4fc = param_1 - iVar2;
      }
      else if (param_1 < iVar2) {
        DAT_00d3b4fc = iVar2 - param_1;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar3 < param_2) {
        _DAT_00d3b4f8 = param_2 - iVar3;
      }
      else if (param_2 < iVar3) {
        _DAT_00d3b4f8 = iVar3 - param_2;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar4 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar4 = _DAT_00d3b4f8;
      }
      iVar5 = 5;
      break;
    case 'M': ;
      iVar2 = (int)local_c[-1];
      iVar5 = (-(uint)((char)local_c[0x8d] != '\0') & 0xf) + 10;
      iVar3 = (int)*local_c;
      if (iVar2 < param_1) {
        DAT_00d3b4fc = param_1 - iVar2;
      }
      else if (param_1 < iVar2) {
        DAT_00d3b4fc = iVar2 - param_1;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar3 < param_2) {
        _DAT_00d3b4f8 = param_2 - iVar3;
      }
      else if (param_2 < iVar3) {
        _DAT_00d3b4f8 = iVar3 - param_2;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar4 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar4 = _DAT_00d3b4f8;
      }
      break;
    case 'N': ;
      iVar2 = (int)local_c[-1];
      iVar3 = (int)*local_c;
      if (iVar2 < param_1) {
        DAT_00d3b4fc = param_1 - iVar2;
      }
      else if (param_1 < iVar2) {
        DAT_00d3b4fc = iVar2 - param_1;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar3 < param_2) {
        _DAT_00d3b4f8 = param_2 - iVar3;
      }
      else if (param_2 < iVar3) {
        _DAT_00d3b4f8 = iVar3 - param_2;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar4 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar4 = _DAT_00d3b4f8;
      }
      iVar5 = 0xf;
    }
    if ((local_8 - DAT_00bcd5ec < iVar5 - iVar4) &&
       (iVar2 = FUN_004b4420(param_1,param_2,iVar2,iVar3), iVar2 != 0)) {
      local_4 = param_3;
      DAT_00bcd5ec = iVar4;
      local_8 = iVar5;
    }
switchD_004bcfe2_caseD_e: ;
    local_c = local_c + 0xc2;
    param_3 = param_3 + 1;
    if (0xc8ad4d < (int)local_c) {
      if (local_4 == 0) {
        return 0;
      }
      _DAT_00d3b4e0 = (int)(short)(&DAT_00bcd60e)[local_4 * 0xc2];
      _DAT_00d3b4e4 = (int)(short)(&DAT_00bcd60c)[local_4 * 0xc2];
      return local_4;
    }
  } while( true );
}
