#include "restore_types.h"
#define DAT_00bcba54 (*(undefined4 *)0x00bcba54u)
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern undefined4 FUN_004b4420();

int FUN_00499b50(int param_1,int param_2)

{
  char cVar1;
  short sVar2;
  byte bVar3;
  byte bVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  char *pcVar8;
  int iVar9;
  int iVar10;
  int local_c;
  
  iVar9 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
  iVar6 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
  sVar2 = (&DAT_00bcd658)[param_1 * 0xc2];
  local_c = 0;
  DAT_00bcd5ec = param_2;
  bVar3 = 0;
  param_2 = 1;
  pcVar8 = &DAT_00bcd786;
  do {
    if (((pcVar8[4] == '\x01') && (pcVar8[0x2e] != '\x04')) &&
       ((cVar1 = *pcVar8, cVar1 == '\\' ||
        ((((cVar1 == 'e' || (cVar1 == ']')) || (cVar1 == 'd')) ||
         ((*(int *)(&DAT_00bcba54 + sVar2 * 0x90) < 1 ||
          ((int)*(short *)(pcVar8 + 0x56) == *(int *)(&DAT_00bcba54 + sVar2 * 0x90))))))))) {
      switch((int)(cVar1)) {
      case '+': ;
      case ',': ;
      case '-': ;
      case '6': ;
      case '7': ;
      case '8': ;
      case 'c': ;
        iVar5 = (int)*(short *)(pcVar8 + 10);
        iVar7 = (int)*(short *)(pcVar8 + 0xc);
        if (iVar5 < iVar9) {
          DAT_00d3b4fc = iVar9 - iVar5;
        }
        else if (iVar9 < iVar5) {
          DAT_00d3b4fc = iVar5 - iVar9;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar7 < iVar6) {
          _DAT_00d3b4f8 = iVar6 - iVar7;
        }
        else if (iVar6 < iVar7) {
          _DAT_00d3b4f8 = iVar7 - iVar6;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar10 = DAT_00d3b4fc;
        if (DAT_00d3b4fc < _DAT_00d3b4f8) {
          iVar10 = _DAT_00d3b4f8;
        }
        bVar4 = 0x32;
        break;
default: ;
        goto switchD_00499c0d_caseD_2e;
      case 'R': ;
      case 'S': ;
      case 'T': ;
      case 'f': ;
      case 'g': ;
      case 'h': ;
      case 'm': ;
        iVar5 = (int)*(short *)(pcVar8 + 10);
        iVar7 = (int)*(short *)(pcVar8 + 0xc);
        if (iVar5 < iVar9) {
          DAT_00d3b4fc = iVar9 - iVar5;
        }
        else if (iVar9 < iVar5) {
          DAT_00d3b4fc = iVar5 - iVar9;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar7 < iVar6) {
          _DAT_00d3b4f8 = iVar6 - iVar7;
        }
        else if (iVar6 < iVar7) {
          _DAT_00d3b4f8 = iVar7 - iVar6;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar10 = DAT_00d3b4fc;
        if (DAT_00d3b4fc < _DAT_00d3b4f8) {
          iVar10 = _DAT_00d3b4f8;
        }
        bVar4 = 10;
        break;
      case '\\': ;
      case 'e': ;
        bVar4 = (-(pcVar8[0x126] != '\0') & 0x14U) + 0x14;
        iVar7 = (int)*(short *)(pcVar8 + 0xc);
        iVar5 = (int)*(short *)(pcVar8 + 10);
        if (iVar5 < iVar9) {
          DAT_00d3b4fc = iVar9 - iVar5;
        }
        else if (iVar9 < iVar5) {
          DAT_00d3b4fc = iVar5 - iVar9;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar7 < iVar6) {
          _DAT_00d3b4f8 = iVar6 - iVar7;
        }
        else if (iVar6 < iVar7) {
          _DAT_00d3b4f8 = iVar7 - iVar6;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar10 = DAT_00d3b4fc;
        if (DAT_00d3b4fc < _DAT_00d3b4f8) {
          iVar10 = _DAT_00d3b4f8;
        }
        break;
      case ']': ;
      case 'd': ;
        iVar5 = (int)*(short *)(pcVar8 + 10);
        iVar7 = (int)*(short *)(pcVar8 + 0xc);
        if (iVar5 < iVar9) {
          DAT_00d3b4fc = iVar9 - iVar5;
        }
        else if (iVar9 < iVar5) {
          DAT_00d3b4fc = iVar5 - iVar9;
        }
        else {
          DAT_00d3b4fc = 0;
        }
        if (iVar7 < iVar6) {
          _DAT_00d3b4f8 = iVar6 - iVar7;
        }
        else if (iVar6 < iVar7) {
          _DAT_00d3b4f8 = iVar7 - iVar6;
        }
        else {
          _DAT_00d3b4f8 = 0;
        }
        iVar10 = DAT_00d3b4fc;
        if (DAT_00d3b4fc < _DAT_00d3b4f8) {
          iVar10 = _DAT_00d3b4f8;
        }
        bVar4 = 0x1e;
      }
      if (((bVar3 < bVar4) && (iVar10 < DAT_00bcd5ec)) &&
         (((&DAT_00bcd630)[param_1 * 0x184] == '\x15' ||
          (iVar5 = FUN_004b4420(iVar9,iVar6,iVar5,iVar7), iVar5 != 0)))) {
        local_c = param_2;
        DAT_00bcd5ec = iVar10;
        bVar3 = bVar4;
      }
    }
switchD_00499c0d_caseD_2e: ;
    pcVar8 = pcVar8 + 0x184;
    param_2 = param_2 + 1;
    if (0xc8ad41 < (int)pcVar8) {
      if (local_c == 0) {
        return 0;
      }
      _DAT_00d3b4e0 = (int)(short)(&DAT_00bcd60e)[local_c * 0xc2];
      _DAT_00d3b4e4 = (int)(short)(&DAT_00bcd60c)[local_c * 0xc2];
      return local_c;
    }
  } while( true );
}
