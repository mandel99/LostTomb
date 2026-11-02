#include "restore_types.h"
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
extern void FUN_0048ae50();

void FUN_0048aef0(int param_1,int param_2,undefined4 param_3)

{
  short sVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  
  iVar7 = param_2 * 0x184;
  iVar4 = (int)(short)(&DAT_00bcd656)[param_2 * 0xc2];
  sVar1 = (&DAT_007999e8)[iVar4 * 0x84];
  if ((0x6d < sVar1) && (sVar1 < 0x73)) {
switchD_0048af3d_caseD_20: ;
    iVar6 = iVar4 * 0x108;
    switch(*(undefined2 *)((int)&DAT_00799aa4 + iVar6 + 2)) {
    case 1: ;
      uVar5 = 0x10;
      break;
default: ;
      uVar5 = 0xffffffff;
      break;
    case 3: ;
      uVar5 = 0x18;
      break;
    case 4: ;
      uVar5 = 0x14;
      break;
    case 5: ;
      uVar5 = 0xb;
      break;
    case 6: ;
      uVar5 = 0x19;
      break;
    case 7: ;
      uVar5 = 0xc;
      break;
    case 8: ;
      uVar5 = 10;
      break;
    case 9: ;
      uVar5 = 0xe;
      break;
    case 10: ;
      uVar5 = 0x12;
      break;
    case 0xb: ;
      uVar5 = 0x16;
      break;
    case 0xc: ;
      uVar5 = 0x17;
      break;
    case 0xd: ;
      uVar5 = 9;
      break;
    case 0xe: ;
      uVar5 = 0x1c;
      break;
    case 0xf: ;
      uVar5 = 0x15;
      break;
    case 0x10: ;
      uVar5 = 0x11;
      break;
    case 0x11: ;
      uVar5 = 0x1d;
      break;
    case 0x12: ;
      uVar5 = 0x1e;
      break;
    case 0x14: ;
      uVar5 = 0x20;
      break;
    case 0x15: ;
      uVar5 = 2;
      break;
    case 0x16: ;
      uVar5 = 8;
      break;
    case 0x17: ;
      uVar5 = 0x1f;
      break;
    case 0x18: ;
      uVar5 = 0xd;
      break;
    case 0x19: ;
      uVar5 = 0x21;
      break;
    case 0x1a: ;
      uVar5 = 0x22;
    }
    if (((&DAT_007999e8)[iVar4 * 0x84] == 0xcc) && ((byte)(&DAT_00bcd603)[iVar7] == uVar5)) {
      *(short *)(&DAT_00799ade + iVar6) = *(short *)(&DAT_00799ade + iVar6) + -1;
    }
    else {
      *(short *)(&DAT_00799adc + iVar6) = *(short *)(&DAT_00799adc + iVar6) + -1;
    }
LAB_0048b075: ;
    if (param_1 == *(short *)(&DAT_00bcd66c + iVar7)) {
      (&DAT_00bcd630)[iVar7] = 0xf;
      *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
      *(undefined2 *)(&DAT_00bcd61c + iVar7) = *(undefined2 *)(&DAT_00bcd624 + iVar7);
      *(undefined2 *)(&DAT_00bcd61e + iVar7) = *(undefined2 *)(&DAT_00bcd626 + iVar7);
      *(undefined2 *)(&DAT_00bcd66c + iVar7) = 0;
      return;
    }
    *(short *)(&DAT_00bcd66c + iVar7) = *(short *)(&DAT_00bcd66c + iVar7) - (short)param_1;
    iVar4 = (FUN_0048ae50(param_2,param_3), restore_unresolved_return(0x0048ae50u));
    if (iVar4 != 0) {
      switch((&DAT_007999e8)[iVar4 * 0x84]) {
      case 0x47: ;
      case 0xbf: ;
        (&DAT_00bcd630)[iVar7] = 10;
        break;
      case 0x48: ;
      case 0x49: ;
      case 0xc1: ;
        (&DAT_00bcd630)[iVar7] = 9;
        break;
default: ;
        (&DAT_00bcd630)[iVar7] = 0xb;
        break;
      case 0x5f: ;
        (&DAT_00bcd630)[iVar7] = 0x10;
      }
      uVar3 = DAT_00d3b4e4;
      *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
      uVar2 = DAT_00d3b4e0;
      *(undefined2 *)(&DAT_00bcd61c + iVar7) = uVar3;
      *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar2;
      return;
    }
    (&DAT_00bcd630)[iVar7] = 8;
    *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
    return;
  }
  switch((int)(sVar1)) {
  case 0x20: ;
  case 0x2f: ;
  case 0x33: ;
  case 0x35: ;
  case 0x4a: ;
  case 0x71: ;
  case 0xb1: ;
  case 0xb2: ;
  case 0xbb: ;
  case 0xbc: ;
  case 0xbd: ;
  case 0xc2: ;
  case 0xcb: ;
  case 0xcc: ;
  case 0xcd: ;
  case 0xe2: ;
  case 0xe7: ;
  case 0xe8: ;
  case 0xe9: ;
    goto switchD_0048af3d_caseD_20;
default: ;
    if ((&DAT_007999e8)[iVar4 * 0x84] == 0x47) {
      sVar1 = *(short *)(&DAT_00bcd66c + iVar7);
      *(short *)(&DAT_00799adc + iVar4 * 0x108) = *(short *)(&DAT_00799adc + iVar4 * 0x108) + -1;
      *(short *)(&DAT_00799ade + iVar4 * 0x108) = *(short *)(&DAT_00799ade + iVar4 * 0x108) - sVar1;
    }
    goto LAB_0048b075;
  }
}
