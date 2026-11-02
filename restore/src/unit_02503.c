#include "restore_types.h"
#define DAT_0077af00 (*(undefined4 *)0x0077af00u)
#define DAT_0077af04 (*(undefined4 *)0x0077af04u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd65e (*(undefined4 *)0x00bcd65eu)
#define DAT_00bcd666 (*(undefined4 *)0x00bcd666u)
#define DAT_00bcd674 (*(undefined4 *)0x00bcd674u)
#define DAT_00bcd675 (*(undefined4 *)0x00bcd675u)
#define DAT_00bcd677 (*(undefined4 *)0x00bcd677u)
#define DAT_00bcd67a (*(undefined4 *)0x00bcd67au)
#define DAT_00bcd67c (*(undefined4 *)0x00bcd67cu)
#define DAT_00bcd729 (*(undefined4 *)0x00bcd729u)
#define DAT_00d38374 (*(undefined *)0x00d38374u)
#define DAT_00d386dc (*(char *)0x00d386dcu)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00ea1880 (*(undefined4 *)0x00ea1880u)
#define DAT_00ea1888 (*(undefined4 *)0x00ea1888u)
#define DAT_00ea446c (*(undefined4 *)0x00ea446cu)
#define DAT_00ea4474 (*(undefined4 *)0x00ea4474u)
#define DAT_00ea4490 (*(undefined4 *)0x00ea4490u)
#define DAT_00ea4494 (*(undefined4 *)0x00ea4494u)
#define DAT_00ea4498 (*(undefined4 *)0x00ea4498u)
#define DAT_00ea449c (*(undefined4 *)0x00ea449cu)
#define DAT_00ea5b48 (*(undefined4 *)0x00ea5b48u)
#define DAT_00ea5bf4 (*(undefined4 *)0x00ea5bf4u)
#define DAT_00ea5db0 (*(undefined4 *)0x00ea5db0u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea6090 (*(undefined4 *)0x00ea6090u)
#define DAT_00ea6210 (*(undefined4 *)0x00ea6210u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eadcdc (*(undefined *)0x00eadcdcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern int * FUN_0044bb90();
extern uint FUN_0044d700();
extern undefined4 FUN_0045cf50();
extern undefined4 FUN_0045d180();
extern undefined4 FUN_00495b60();
extern uint FUN_004bf9d0();
extern undefined4 FUN_004c5120();
extern bool FUN_004c52d0();
extern int FUN_0053c060();

void FUN_0053c100(int param_1)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  bool bVar8;
  undefined4 local_20 [8];
  
  if (param_1 < 1) {
    return;
  }
  iVar7 = param_1 * 0x184;
  local_20[0] = 8;
  local_20[1] = 0xd;
  local_20[2] = 2;
  local_20[3] = 1;
  local_20[4] = 0xb;
  local_20[5] = 5;
  local_20[6] = 1;
  local_20[7] = 10;
  (&DAT_00bcd675)[iVar7] = 0;
  DAT_00eadcdc = 0xffffffff;
  bVar8 = false;
  for (iVar2 = FUN_0044bb90(0,0); iVar2 != 0; iVar2 = FUN_0044bb90(iVar2,0)) {
    cVar1 = FUN_0044d700();
    if (cVar1 != '\0') {
      bVar8 = true;
      break;
    }
  }
  (&DAT_00bcd729)[iVar7] = 0;
  uVar4 = DAT_00eadcdc;
  switch((&DAT_00bcd602)[iVar7]) {
  case 1: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    if (2 < (byte)(cVar1 + 1U)) {
      (&DAT_00bcd674)[iVar7] = 0;
    }
    uVar4 = (uint)(byte)(&DAT_00bcd674)[iVar7];
    DAT_00eadcdc = uVar4;
    break;
  case 2: ;
    iVar2 = (&DAT_00ea5db0)[DAT_00eaac28 * 0x1276];
    if (iVar2 == 1) {
      (&DAT_00bcd729)[iVar7] = 1;
      (&DAT_00bcd675)[iVar7] = 1;
      DAT_00eadcdc = 1;
      return;
    }
    if (iVar2 != 2) {
      if (iVar2 == 3) {
        (&DAT_00bcd729)[iVar7] = 0xc;
        (&DAT_00bcd675)[iVar7] = 2;
        DAT_00eadcdc = 2;
        return;
      }
      (&DAT_00bcd729)[iVar7] = 1;
      (&DAT_00bcd675)[iVar7] = 3;
      DAT_00eadcdc = 3;
      return;
    }
LAB_0053c538: ;
    (&DAT_00bcd729)[iVar7] = 1;
    (&DAT_00bcd675)[iVar7] = 0;
    DAT_00eadcdc = 0;
    return;
  case 3: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    bVar8 = (byte)(cVar1 + 1U) < 2;
    goto LAB_0053c29b;
  case 4: ;
    cVar1 = (&DAT_00bcd630)[iVar7];
    if (cVar1 == '\b') {
      if ((&DAT_00bcd65e)[iVar7] == '\x02') goto LAB_0053c538;
      if ((&DAT_00bcd65e)[iVar7] == '\x01') goto switchD_0053c342_caseD_b;
    }
    else if (((cVar1 == '\t') || (cVar1 == '\n')) || (cVar1 == '\v')) {
      iVar2 = (int)*(short *)(&DAT_00bcd624 + iVar7);
      iVar5 = (int)*(short *)(&DAT_00bcd61c + iVar7);
      iVar6 = (int)*(short *)(&DAT_00bcd626 + iVar7);
      _DAT_00d3b4f8 = (int)*(short *)(&DAT_00bcd61e + iVar7);
      if (iVar2 < iVar5) {
        iVar5 = iVar5 - iVar2;
      }
      else if (iVar5 < iVar2) {
        iVar5 = iVar2 - iVar5;
      }
      else {
        iVar5 = 0;
      }
      if (iVar6 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar6;
      }
      else if (_DAT_00d3b4f8 < iVar6) {
        _DAT_00d3b4f8 = iVar6 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      DAT_00d3b4fc = iVar5;
      if (iVar5 < _DAT_00d3b4f8) {
LAB_0053c662: ;
        iVar5 = _DAT_00d3b4f8;
        _DAT_00d3b4f8 = iVar5;
      }
LAB_0053c664: ;
      if (0x18 < iVar5) {
switchD_0053c342_caseD_8: ;
        (&DAT_00bcd675)[iVar7] = 2;
        DAT_00eadcdc = 2;
        return;
      }
    }
    goto switchD_0053c1c6_caseD_f;
  case 5: ;
    if (5 < (int)(&DAT_00ea446c)[DAT_00eaac28 * 0x1276]) {
LAB_0053c3e8: ;
      (&DAT_00bcd675)[iVar7] = 10;
      DAT_00eadcdc = 10;
      return;
    }
    if (5 < (int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276]) {
      (&DAT_00bcd675)[iVar7] = 0xb;
      DAT_00eadcdc = 0xb;
      return;
    }
    goto switchD_0053c1c6_caseD_f;
default: ;
    (&DAT_00bcd675)[iVar7] = 0xff;
    DAT_00eadcdc = 0xffffffff;
    return;
  case 7: ;
    if ((((&DAT_00ea4494)[DAT_00eaac28 * 0x1276] + (&DAT_00ea4490)[DAT_00eaac28 * 0x1276]) * 100) /
        (int)((&DAT_00ea449c)[DAT_00eaac28 * 0x1276] + (&DAT_00ea4498)[DAT_00eaac28 * 0x1276] +
             (&DAT_00ea4494)[DAT_00eaac28 * 0x1276] + (&DAT_00ea4490)[DAT_00eaac28 * 0x1276]) < 0x32
       ) {
      uVar4 = 10;
      (&DAT_00bcd729)[iVar7] = 0xc;
      DAT_00eadcdc = 10;
    }
    else if ((byte)(&DAT_00bcd65e)[iVar7] < 7) {
      if ((&DAT_00bcd65e)[iVar7] == 0) {
        (&DAT_00bcd729)[iVar7] = 0xc;
      }
      else {
        uVar4 = 0xc;
        DAT_00eadcdc = 0xc;
        (&DAT_00bcd729)[iVar7] = 0xc;
      }
    }
    else {
      uVar4 = 0xb;
      (&DAT_00bcd729)[iVar7] = 0xc;
      DAT_00eadcdc = 0xb;
    }
    break;
  case 8: ;
    DAT_00eadcdc = (599 < (byte)(&DAT_00bcd65e)[iVar7]) + 10;
    (&DAT_00bcd675)[iVar7] = (char)DAT_00eadcdc;
    return;
  case 9: ;
    if ((&DAT_00bcd630)[iVar7] == '\b') {
      if ((&DAT_00bcd65e)[iVar7] == '\x02') {
        (&DAT_00bcd675)[iVar7] = 1;
        DAT_00eadcdc = 1;
        return;
      }
    }
    else if ((&DAT_00bcd630)[iVar7] == '\t') {
      iVar2 = (int)*(short *)(&DAT_00bcd624 + iVar7);
      iVar5 = (int)*(short *)(&DAT_00bcd61c + iVar7);
      iVar6 = (int)*(short *)(&DAT_00bcd626 + iVar7);
      _DAT_00d3b4f8 = (int)*(short *)(&DAT_00bcd61e + iVar7);
      if (iVar2 < iVar5) {
        iVar5 = iVar5 - iVar2;
      }
      else if (iVar5 < iVar2) {
        iVar5 = iVar2 - iVar5;
      }
      else {
        iVar5 = 0;
      }
      if (iVar6 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar6;
      }
      else if (_DAT_00d3b4f8 < iVar6) {
        _DAT_00d3b4f8 = iVar6 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      DAT_00d3b4fc = iVar5;
      if (iVar5 < _DAT_00d3b4f8) goto LAB_0053c662;
      goto LAB_0053c664;
    }
    goto switchD_0053c1c6_caseD_f;
  case 10: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    if (0xc < (byte)(cVar1 + 1U)) {
      (&DAT_00bcd674)[iVar7] = 0;
    }
    if ((&DAT_00bcd630)[iVar7] != '\r') goto switchD_0053c1c6_caseD_f;
    uVar4 = (uint)(byte)(&DAT_00bcd674)[iVar7];
    DAT_00eadcdc = uVar4;
    break;
  case 0xf: ;
  case 0x10: ;
  case 0x11: ;
  case 0x12: ;
  case 0x1d: ;
  case 0x1e: ;
  case 0x1f: ;
  case 0x28: ;
  case 0x57: ;
  case 0x69: ;
    goto switchD_0053c1c6_caseD_f;
  case 0x13: ;
  case 0x15: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    if (2 < (byte)(cVar1 + 1U)) {
      (&DAT_00bcd674)[iVar7] = 1;
    }
    DAT_00eadcdc = (byte)(&DAT_00bcd674)[iVar7] + 1;
    if ((&DAT_00bcd630)[iVar7] == '\v') {
      if ((*(int *)(&DAT_0077af04 + (uint)(byte)(&DAT_00bcd677)[iVar7] * 0x58) == 0) &&
         (*(int *)(&DAT_0077af00 + (uint)(byte)(&DAT_00bcd677)[iVar7] * 0x58) == 0)) {
        uVar4 = 0;
        (&DAT_00bcd729)[iVar7] = 6;
        DAT_00eadcdc = 0;
      }
      else {
LAB_0053c8ac: ;
        uVar4 = DAT_00eadcdc;
        (&DAT_00bcd729)[iVar7] = 6;
      }
    }
    else {
      if ((&DAT_00bcd630)[iVar7] != '\n') goto LAB_0053c8ac;
      iVar2 = FUN_0045cf50(param_1,DAT_00eaac28,(int)(short)(&DAT_00bcd656)[param_1 * 0xc2],
                           (&DAT_00bcd666)[iVar7]);
      if (iVar2 == 0) {
        iVar2 = FUN_0045d180(param_1,DAT_00eaac28,(int)(short)(&DAT_00bcd656)[param_1 * 0xc2],
                             (&DAT_00bcd666)[iVar7]);
        if (iVar2 == 0) goto LAB_0053c8ac;
        uVar4 = 3;
        (&DAT_00bcd729)[iVar7] = 6;
        DAT_00eadcdc = 3;
      }
      else {
        uVar4 = 4;
        (&DAT_00bcd729)[iVar7] = 6;
        DAT_00eadcdc = 4;
      }
    }
    break;
  case 0x14: ;
    if ((&DAT_00bcd630)[iVar7] == '\r') {
      if ((*(int *)(&DAT_0077af04 + (uint)(byte)(&DAT_00bcd677)[iVar7] * 0x58) == 0) &&
         (*(int *)(&DAT_0077af00 + (uint)(byte)(&DAT_00bcd677)[iVar7] * 0x58) == 0)) {
        uVar4 = 2;
        (&DAT_00bcd729)[iVar7] = 6;
        DAT_00eadcdc = 2;
      }
      else {
        uVar4 = 4;
        (&DAT_00bcd729)[iVar7] = 6;
        DAT_00eadcdc = 4;
      }
    }
    else if ((&DAT_00bcd630)[iVar7] == '\n') {
      DAT_00eadcdc = FUN_00495b60(param_1);
      if (DAT_00eadcdc == 1) {
        (&DAT_00bcd729)[iVar7] = 6;
        uVar4 = 1;
      }
      else {
        (&DAT_00bcd729)[iVar7] = 6;
        uVar4 = -(uint)(DAT_00eadcdc != 2) & 2;
        DAT_00eadcdc = uVar4;
      }
    }
    else {
      uVar4 = 3;
      (&DAT_00bcd729)[iVar7] = 6;
      DAT_00eadcdc = 3;
    }
    break;
  case 0x17: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    bVar8 = (byte)(cVar1 + 1U) < 4;
LAB_0053c29b: ;
    if (!bVar8) {
      (&DAT_00bcd674)[iVar7] = 0;
    }
    uVar4 = (uint)(byte)(&DAT_00bcd674)[iVar7];
    (&DAT_00bcd729)[iVar7] = 0xd;
    DAT_00eadcdc = uVar4;
    break;
  case 0x18: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    if (1 < (byte)(cVar1 + 1U)) {
      (&DAT_00bcd674)[iVar7] = 0;
    }
    uVar4 = (uint)(byte)(&DAT_00bcd674)[iVar7];
    (&DAT_00bcd729)[iVar7] = 0xe;
    DAT_00eadcdc = uVar4;
    break;
  case 0x19: ;
    switch((&DAT_00bcd630)[iVar7]) {
    case 8: ;
    case 0xc: ;
      goto switchD_0053c342_caseD_8;
    case 9: ;
    case 10: ;
      (&DAT_00bcd675)[iVar7] = 0;
      DAT_00eadcdc = 0;
      return;
    case 0xb: ;
    case 0xd: ;
switchD_0053c342_caseD_b: ;
      (&DAT_00bcd675)[iVar7] = 1;
      DAT_00eadcdc = 1;
      return;
    }
    goto switchD_0053c1c6_caseD_f;
  case 0x1a: ;
    (&DAT_00bcd729)[iVar7] = 6;
    uVar4 = (uint)((&DAT_00bcd630)[iVar7] != '\x02');
    DAT_00eadcdc = uVar4;
    break;
  case 0x1b: ;
    uVar3 = FUN_004c5120((int)(short)(&DAT_00bcd652)[param_1 * 0xc2]);
    switch((int)(uVar3)) {
    case 0: ;
      if ((DAT_00d38374 < 0x3c) && (DAT_00d386dc != '\0')) {
LAB_0053c911: ;
        DAT_00eadcdc = 0xb;
        goto LAB_0053c91b;
      }
      break;
    case 1: ;
      if ((int)(&DAT_00ea5b48)[DAT_00eaac28 * 0x1276] < 0x14) {
        (&DAT_00bcd729)[iVar7] = 5;
        (&DAT_00bcd675)[iVar7] = 0xb;
        DAT_00eadcdc = 0xb;
        return;
      }
      break;
    case 2: ;
      if (10 < (int)(&DAT_00ea4474)[DAT_00eaac28 * 0x1276]) {
        (&DAT_00bcd729)[iVar7] = 1;
        (&DAT_00bcd675)[iVar7] = 0xb;
        DAT_00eadcdc = 0xb;
        return;
      }
      break;
    case 3: ;
      if (bVar8) {
        (&DAT_00bcd729)[iVar7] = 2;
        (&DAT_00bcd675)[iVar7] = 0xb;
        DAT_00eadcdc = 0xb;
        return;
      }
      break;
    case 4: ;
      cVar1 = FUN_004c52d0((int)(short)(&DAT_00bcd652)[param_1 * 0xc2],2);
      if (cVar1 != '\0') {
        (&DAT_00bcd729)[iVar7] = 8;
        (&DAT_00bcd675)[iVar7] = 0xd;
        DAT_00eadcdc = 0xd;
        return;
      }
      if (*(int *)(&DAT_00ea1880 + DAT_00eaac28 * 0x49d8) < 0x3c) goto LAB_0053c911;
      if ((int)(&DAT_00ea5bf4)[DAT_00eaac28 * 0x1276] < 0x3c) {
        (&DAT_00bcd729)[iVar7] = 10;
        (&DAT_00bcd675)[iVar7] = 0xc;
        DAT_00eadcdc = 0xc;
        return;
      }
    }
    goto switchD_0053c1c6_caseD_f;
  case 0x20: ;
    bVar8 = SBORROW4(*(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8),0x28);
    iVar2 = *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8) + -0x28;
    goto LAB_0053ca6b;
  case 0x21: ;
    (&DAT_00bcd729)[iVar7] = 8;
    uVar4 = (800 < (byte)(&DAT_00bcd65e)[iVar7]) + 1;
    DAT_00eadcdc = uVar4;
    break;
  case 0x22: ;
    bVar8 = SBORROW4(*(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8),0x1e);
    iVar2 = *(int *)(&DAT_00ea1888 + DAT_00eaac28 * 0x49d8) + -0x1e;
LAB_0053ca6b: ;
    if (bVar8 != iVar2 < 0) {
      (&DAT_00bcd729)[iVar7] = 8;
      (&DAT_00bcd675)[iVar7] = 10;
      DAT_00eadcdc = 10;
      return;
    }
    goto switchD_0053c1c6_caseD_f;
  case 0x26: ;
    if (((&DAT_00bcd630)[iVar7] != '\v') && ((&DAT_00bcd630)[iVar7] != '\f'))
    goto switchD_0053c1c6_caseD_f;
    iVar2 = (int)*(short *)(&DAT_00bcd624 + iVar7);
    iVar5 = (int)*(short *)(&DAT_00bcd61c + iVar7);
    iVar6 = (int)*(short *)(&DAT_00bcd626 + iVar7);
    _DAT_00d3b4f8 = (int)*(short *)(&DAT_00bcd61e + iVar7);
    if (iVar2 < iVar5) {
      DAT_00d3b4fc = iVar5 - iVar2;
    }
    else if (iVar5 < iVar2) {
      DAT_00d3b4fc = iVar2 - iVar5;
    }
    else {
      DAT_00d3b4fc = 0;
    }
    if (iVar6 < _DAT_00d3b4f8) {
      _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar6;
    }
    else if (_DAT_00d3b4f8 < iVar6) {
      _DAT_00d3b4f8 = iVar6 - _DAT_00d3b4f8;
    }
    else {
      _DAT_00d3b4f8 = 0;
    }
    iVar2 = DAT_00d3b4fc;
    if (DAT_00d3b4fc < _DAT_00d3b4f8) {
      iVar2 = _DAT_00d3b4f8;
    }
    if (iVar2 < 0x19) goto LAB_0053c8ac;
    uVar4 = 2;
    (&DAT_00bcd729)[iVar7] = 6;
    DAT_00eadcdc = 2;
    break;
  case 0x27: ;
    if ((&DAT_00bcd630)[iVar7] == '\b') {
      (&DAT_00bcd729)[iVar7] = 6;
      (&DAT_00bcd675)[iVar7] = 10;
      DAT_00eadcdc = 10;
      return;
    }
    if ((&DAT_00bcd630)[iVar7] == '\t') {
      (&DAT_00bcd729)[iVar7] = 6;
      (&DAT_00bcd675)[iVar7] = 0xb;
      DAT_00eadcdc = 0xb;
      return;
    }
    goto switchD_0053c1c6_caseD_f;
  case 0x2a: ;
    iVar2 = (&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276];
    if (iVar2 == 0) {
      (&DAT_00bcd729)[iVar7] = 2;
      (&DAT_00bcd675)[iVar7] = 0;
      DAT_00eadcdc = iVar2;
      return;
    }
    if (bVar8) {
      (&DAT_00bcd729)[iVar7] = 2;
      (&DAT_00bcd675)[iVar7] = 1;
      DAT_00eadcdc = 1;
      return;
    }
    if (iVar2 < 0xb) {
      (&DAT_00bcd729)[iVar7] = 2;
      (&DAT_00bcd675)[iVar7] = 2;
      DAT_00eadcdc = 2;
      return;
    }
    if (iVar2 < 0x1f) {
      (&DAT_00bcd729)[iVar7] = 2;
      (&DAT_00bcd675)[iVar7] = 3;
      DAT_00eadcdc = 3;
      return;
    }
    goto switchD_0053c1c6_caseD_f;
  case 0x42: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    if (2 < (byte)(cVar1 + 1U)) {
      (&DAT_00bcd674)[iVar7] = 0;
    }
    uVar4 = (uint)(byte)(&DAT_00bcd674)[iVar7];
    (&DAT_00bcd729)[iVar7] = 6;
    DAT_00eadcdc = uVar4;
    break;
  case 0x49: ;
    (&DAT_00bcd675)[iVar7] = (&DAT_00bcd630)[iVar7] == '\f';
    DAT_00eadcdc = (uint)((&DAT_00bcd630)[iVar7] == '\f');
    return;
  case 0x4b: ;
  case 0x5a: ;
    (&DAT_00bcd729)[iVar7] = 6;
    uVar4 = (uint)((&DAT_00bcd630)[iVar7] == '\v');
    DAT_00eadcdc = uVar4;
    break;
  case 0x4d: ;
    if (((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) || ((&DAT_00bcd630)[iVar7] != '\n')) {
      if (*(int *)(&DAT_00ea6210 + DAT_00eaac28 * 0x49d8) == 0) {
        (&DAT_00bcd729)[iVar7] = 3;
        uVar4 = 3 - bVar8;
        DAT_00eadcdc = uVar4;
      }
      else {
        uVar4 = 1;
        (&DAT_00bcd729)[iVar7] = 3;
        DAT_00eadcdc = 1;
      }
    }
    else {
      uVar4 = 0;
      (&DAT_00bcd729)[iVar7] = 3;
      DAT_00eadcdc = 0;
    }
    break;
  case 0x4e: ;
    if ((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) {
LAB_0053cd2d: ;
      if ((*(int *)(&DAT_00ea6210 + DAT_00eaac28 * 0x49d8) == 0) ||
         (((&DAT_00bcd630)[iVar7] == '\n' || ((&DAT_00bcd630)[iVar7] == '\f')))) {
        (&DAT_00bcd729)[iVar7] = 3;
        uVar4 = 4 - bVar8;
        DAT_00eadcdc = uVar4;
      }
      else {
        uVar4 = 2;
        (&DAT_00bcd729)[iVar7] = 3;
        DAT_00eadcdc = 2;
      }
    }
    else if ((&DAT_00bcd630)[iVar7] == '\n') {
      uVar4 = 0;
      (&DAT_00bcd729)[iVar7] = 3;
      DAT_00eadcdc = 0;
    }
    else {
      if (((&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] == 0) || ((&DAT_00bcd630)[iVar7] != '\f'))
      goto LAB_0053cd2d;
      uVar4 = 1;
      (&DAT_00bcd729)[iVar7] = 3;
      DAT_00eadcdc = 1;
    }
    break;
  case 0x4f: ;
  case 0x51: ;
  case 0x6c: ;
    (&DAT_00bcd729)[iVar7] = 0xe;
    uVar4 = (uint)((&DAT_00bcd630)[iVar7] != '\t');
    DAT_00eadcdc = (uint)((&DAT_00bcd630)[iVar7] != '\t');
    break;
  case 0x50: ;
    (&DAT_00bcd729)[iVar7] = 0xe;
    uVar4 = (uint)((&DAT_00bcd630)[iVar7] != '\n');
    DAT_00eadcdc = uVar4;
    break;
  case 0x55: ;
    switch((&DAT_00bcd630)[iVar7]) {
    case 0xf: ;
    case 0x10: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x14: ;
    case 0x15: ;
    case 0x16: ;
    case 0x17: ;
    case 0x1a: ;
    case 0x1e: ;
      bVar8 = true;
      break;
default: ;
      bVar8 = false;
    }
    cVar1 = FUN_004bf9d0();
    if (cVar1 != '\0') {
      (&DAT_00bcd675)[iVar7] = 0xb;
      DAT_00eadcdc = 0xb;
      return;
    }
    if (bVar8) goto switchD_0053c1c6_caseD_60;
switchD_0053c1c6_caseD_f: ;
    uVar4 = DAT_00eadcdc;
    break;
  case 0x58: ;
    cVar1 = (&DAT_00bcd674)[iVar7];
    (&DAT_00bcd674)[iVar7] = cVar1 + 1U;
    if (3 < (byte)(cVar1 + 1U)) {
      (&DAT_00bcd674)[iVar7] = 0;
    }
    if ((&DAT_00bcd630)[iVar7] == '\x05') {
      if (((((&DAT_00bcd602)[*(short *)(&DAT_00bcd67a + iVar7) * 0x184] == '\x17') ||
           ((&DAT_00bcd602)[(short)(&DAT_00bcd67c)[param_1 * 0xc2] * 0x184] == '\x17')) ||
          ((&DAT_00bcd602)[*(short *)(&DAT_00bcd67a + iVar7) * 0x184] == '\x16')) ||
         ((&DAT_00bcd602)[(short)(&DAT_00bcd67c)[param_1 * 0xc2] * 0x184] == '\x16'))
      goto LAB_0053ccb7;
      uVar4 = (byte)(&DAT_00bcd674)[iVar7] + 0xe;
      DAT_00eadcdc = uVar4;
      break;
    }
    if ((byte)(&DAT_00bcd65e)[iVar7] < 500) {
      if ((byte)(&DAT_00bcd65e)[iVar7] < 100) goto LAB_0053c3e8;
      goto LAB_0053c911;
    }
    DAT_00eadcdc = 0xc;
    goto LAB_0053c91b;
  case 0x59: ;
    if ((&DAT_00ea6090)[DAT_00eaac28 * 0x1276] != 0) {
      (&DAT_00bcd675)[iVar7] = 0xb;
      DAT_00eadcdc = 0xb;
      return;
    }
    if ((byte)(&DAT_00bcd65e)[iVar7] < 500) {
      if ((byte)(&DAT_00bcd65e)[iVar7] < 100) {
        (&DAT_00bcd675)[iVar7] = 0xc;
        DAT_00eadcdc = 0xc;
        return;
      }
LAB_0053ccb7: ;
      DAT_00eadcdc = 0xd;
    }
    else {
      DAT_00eadcdc = 0xe;
    }
    goto LAB_0053c91b;
  case 0x5b: ;
    (&DAT_00bcd729)[iVar7] = 0xb;
    (&DAT_00bcd675)[iVar7] = 10;
    DAT_00eadcdc = 10;
    return;
  case 0x60: ;
switchD_0053c1c6_caseD_60: ;
    (&DAT_00bcd729)[iVar7] = 0xe;
    (&DAT_00bcd675)[iVar7] = 0xc;
    DAT_00eadcdc = 0xc;
    return;
  case 0x62: ;
    (&DAT_00bcd729)[iVar7] = 8;
    (&DAT_00bcd675)[iVar7] = 0;
    DAT_00eadcdc = 0;
    return;
  }
  if (uVar4 == 0xffffffff) {
    switch((&DAT_00bcd602)[iVar7]) {
    case 5: ;
    case 7: ;
    case 10: ;
    case 0xf: ;
    case 0x10: ;
    case 0x11: ;
    case 0x12: ;
    case 0x1b: ;
    case 0x1d: ;
    case 0x1e: ;
    case 0x1f: ;
    case 0x20: ;
    case 0x22: ;
    case 0x23: ;
    case 0x27: ;
    case 0x28: ;
    case 0x57: ;
    case 0x58: ;
    case 0x59: ;
    case 0x69: ;
      uVar4 = FUN_0053c060();
      DAT_00eadcdc = uVar4;
      if ((uVar4 != 9) && (uVar4 != 8)) {
        (&DAT_00bcd729)[iVar7] = *(undefined1 *)(local_20 + uVar4);
        (&DAT_00bcd675)[iVar7] = (char)uVar4;
        return;
      }
LAB_0053c91b: ;
      uVar4 = DAT_00eadcdc;
      (&DAT_00bcd729)[iVar7] = 0xd;
    }
  }
  (&DAT_00bcd675)[iVar7] = (char)uVar4;
  return;
}
