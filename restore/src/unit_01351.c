#include "restore_types.h"
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd792 (*(undefined4 *)0x00bcd792u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00ea5ee8 (*(undefined4 *)0x00ea5ee8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern undefined4 FUN_004b4420();

int FUN_00499e50(int param_1,int param_2)

{
  int iVar1;
  byte bVar2;
  byte bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  short *psVar8;
  int iVar9;
  int local_8;
  
  DAT_00bcd5ec = param_2;
  iVar1 = *(int *)(&DAT_00ea5ee8 + DAT_00eaac28 * 0x49d8);
  param_2 = 1;
  psVar8 = &DAT_00bcd792;
  iVar5 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
  iVar9 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
  local_8 = 0;
  bVar2 = 0;
  do {
    if (((char)psVar8[-4] != '\x01') || ((char)psVar8[0x11] == '\x04'))
    goto switchD_00499ee0_caseD_0;
    switch((char)psVar8[-6]) {
    case '\0': ;
    case '\x06': ;
    case '\x0e': ;
    case '$': ;
    case '%': ;
    case ')': ;
    case '+': ;
    case ',': ;
    case '-': ;
    case '6': ;
    case '7': ;
    case '8': ;
    case ':': ;
    case ';': ;
    case '<': ;
    case '=': ;
    case '?': ;
    case 'A': ;
    case 'C': ;
    case 'D': ;
    case 'F': ;
    case 'G': ;
    case 'R': ;
    case 'S': ;
    case 'T': ;
    case 'V': ;
    case '\\': ;
    case ']': ;
    case '_': ;
    case 'a': ;
    case 'c': ;
    case 'd': ;
    case 'e': ;
    case 'f': ;
    case 'g': ;
    case 'h': ;
    case 'm': ;
      goto switchD_00499ee0_caseD_0;
default: ;
      if (iVar1 < 4) {
        iVar4 = (int)psVar8[-1];
        iVar7 = (int)*psVar8;
        if (iVar4 < iVar9) {
          DAT_00d3b4fc = iVar9 - iVar4;
        }
        else if (iVar9 < iVar4) {
          DAT_00d3b4fc = iVar4 - iVar9;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar7 < iVar5) {
          _DAT_00d3b4f8 = iVar5 - iVar7;
        }
        else if (iVar5 < iVar7) {
          _DAT_00d3b4f8 = iVar7 - iVar5;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar6 = _DAT_00d3b4f8;
        if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
          iVar6 = DAT_00d3b4fc;
        }
        bVar3 = 10;
        iVar6 = iVar6 + 5;
        break;
      }
      goto switchD_00499ee0_caseD_0;
    case '\v': ;
    case '\f': ;
    case '\r': ;
    case '*': ;
    case '>': ;
      iVar4 = (int)psVar8[-1];
      iVar7 = (int)*psVar8;
      if (iVar4 < iVar9) {
        DAT_00d3b4fc = iVar9 - iVar4;
      }
      else if (iVar9 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar9;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar7 < iVar5) {
        _DAT_00d3b4f8 = iVar5 - iVar7;
      }
      else if (iVar5 < iVar7) {
        _DAT_00d3b4f8 = iVar7 - iVar5;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar6 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar6 = _DAT_00d3b4f8;
      }
      bVar3 = 0x32;
      break;
    case 'M': ;
      bVar3 = (-((char)psVar8[0x8d] != '\0') & 0x14U) + 0x14;
      iVar7 = (int)*psVar8;
      iVar4 = (int)psVar8[-1];
      if (iVar4 < iVar9) {
        DAT_00d3b4fc = iVar9 - iVar4;
      }
      else if (iVar9 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar9;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar7 < iVar5) {
        _DAT_00d3b4f8 = iVar5 - iVar7;
      }
      else if (iVar5 < iVar7) {
        _DAT_00d3b4f8 = iVar7 - iVar5;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar6 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar6 = _DAT_00d3b4f8;
      }
      break;
    case 'N': ;
      iVar4 = (int)psVar8[-1];
      iVar7 = (int)*psVar8;
      if (iVar4 < iVar9) {
        DAT_00d3b4fc = iVar9 - iVar4;
      }
      else if (iVar9 < iVar4) {
        DAT_00d3b4fc = iVar4 - iVar9;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar7 < iVar5) {
        _DAT_00d3b4f8 = iVar5 - iVar7;
      }
      else if (iVar5 < iVar7) {
        _DAT_00d3b4f8 = iVar7 - iVar5;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar6 = DAT_00d3b4fc;
      if (DAT_00d3b4fc < _DAT_00d3b4f8) {
        iVar6 = _DAT_00d3b4f8;
      }
      bVar3 = 0x1e;
    }
    if (((bVar2 < bVar3) && (iVar6 < DAT_00bcd5ec)) &&
       (iVar4 = FUN_004b4420(iVar9,iVar5,iVar4,iVar7), iVar4 != 0)) {
      local_8 = param_2;
      DAT_00bcd5ec = iVar6;
      bVar2 = bVar3;
    }
switchD_00499ee0_caseD_0: ;
    psVar8 = psVar8 + 0xc2;
    param_2 = param_2 + 1;
    if (0xc8ad4d < (int)psVar8) {
      if (local_8 == 0) {
        return 0;
      }
      _DAT_00d3b4e0 = (int)(short)(&DAT_00bcd60e)[local_8 * 0xc2];
      _DAT_00d3b4e4 = (int)(short)(&DAT_00bcd60c)[local_8 * 0xc2];
      return local_8;
    }
  } while( true );
}
