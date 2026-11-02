#include "restore_types.h"
#define DAT_007090e4 (*(undefined4 *)0x007090e4u)
#define DAT_00bcd5ec (*(undefined4 *)0x00bcd5ecu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd7b4 (*(undefined4 *)0x00bcd7b4u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)

int FUN_004bcb10(int param_1,int param_2,int param_3)

{
  int iVar1;
  char *pcVar2;
  int iVar3;
  int local_8;
  int local_4;
  
  iVar3 = 0;
  DAT_00bcd5ec = param_3;
  local_8 = 0;
  local_4 = 0;
  param_3 = 1;
  pcVar2 = &DAT_00bcd7b4;
  do {
    if ((pcVar2[-0x2a] != '\x01') || (*pcVar2 == '\x04')) goto switchD_004bcb6b_default;
    switch(pcVar2[-0x2e]) {
    case '+': ;
    case ',': ;
    case '-': ;
    case '6': ;
    case '7': ;
    case '8': ;
    case 'c': ;
      iVar3 = (int)*(short *)(pcVar2 + -0x24);
      _DAT_00d3b4f8 = (int)*(short *)(pcVar2 + -0x22);
      if (iVar3 < param_1) {
        DAT_00d3b4fc = param_1 - iVar3;
      }
      else if (param_1 < iVar3) {
        DAT_00d3b4fc = iVar3 - param_1;
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
      iVar3 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar3 = DAT_00d3b4fc;
      }
      if (iVar3 <= *(int *)(&DAT_007090e4 + (uint)(byte)(&DAT_00bcd602)[DAT_00d056f8 * 0x184] * 0x38
                           )) {
        iVar1 = 0x11;
        goto LAB_004bcc88;
      }
      break;
    case '.': ;
    case '/': ;
    case '0': ;
    case '1': ;
    case '2': ;
    case '3': ;
    case '4': ;
    case '5': ;
    case '9': ;
    case ':': ;
    case ';': ;
    case '<': ;
    case '=': ;
    case '>': ;
    case '?': ;
    case '@': ;
    case 'A': ;
    case 'B': ;
    case 'C': ;
    case 'D': ;
    case 'E': ;
    case 'F': ;
    case 'G': ;
    case 'H': ;
    case 'I': ;
    case 'J': ;
    case 'K': ;
    case 'L': ;
    case 'M': ;
    case 'N': ;
    case 'O': ;
    case 'P': ;
    case 'Q': ;
    case 'R': ;
    case 'S': ;
    case 'T': ;
    case 'U': ;
    case 'V': ;
    case 'W': ;
    case 'X': ;
    case 'Y': ;
    case 'Z': ;
    case '[': ;
    case '^': ;
    case '_': ;
    case '`': ;
    case 'a': ;
    case 'b': ;
      break;
    case '\\': ;
    case 'e': ;
      iVar1 = (-(uint)(pcVar2[0xf8] != '\0') & 0xf) + 10;
      _DAT_00d3b4f8 = (int)*(short *)(pcVar2 + -0x22);
      iVar3 = (int)*(short *)(pcVar2 + -0x24);
      if (iVar3 < param_1) {
        DAT_00d3b4fc = param_1 - iVar3;
      }
      else if (param_1 < iVar3) {
        DAT_00d3b4fc = iVar3 - param_1;
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
      iVar3 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar3 = DAT_00d3b4fc;
      }
      goto LAB_004bcc88;
    case ']': ;
    case 'd': ;
      iVar3 = (int)*(short *)(pcVar2 + -0x24);
      _DAT_00d3b4f8 = (int)*(short *)(pcVar2 + -0x22);
      if (iVar3 < param_1) {
        DAT_00d3b4fc = param_1 - iVar3;
      }
      else if (param_1 < iVar3) {
        DAT_00d3b4fc = iVar3 - param_1;
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
      iVar3 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar3 = DAT_00d3b4fc;
      }
      iVar1 = 0xf;
LAB_004bcc88: ;
      if (local_4 - DAT_00bcd5ec < iVar1 - iVar3) {
        local_8 = param_3;
        DAT_00bcd5ec = iVar3;
        local_4 = iVar1;
      }
      break;
default: ;
      goto switchD_004bcb6b_default;
    }
    iVar3 = local_8;
switchD_004bcb6b_default: ;
    pcVar2 = pcVar2 + 0x184;
    param_3 = param_3 + 1;
    if (0xc8ad6f < (int)pcVar2) {
      if (iVar3 != 0) {
        _DAT_00d3b4e0 = (int)(short)(&DAT_00bcd60e)[iVar3 * 0xc2];
        _DAT_00d3b4e4 = (int)(short)(&DAT_00bcd60c)[iVar3 * 0xc2];
        return iVar3;
      }
      return 0;
    }
  } while( true );
}
