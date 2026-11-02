#include "restore_types.h"
#define DAT_0077de54 (*(undefined4 *)0x0077de54u)
#define DAT_0077de58 (*(undefined4 *)0x0077de58u)
#define DAT_0077de5c (*(undefined4 *)0x0077de5cu)
#define DAT_0077de5e (*(undefined4 *)0x0077de5eu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00799a02 (*(undefined4 *)0x00799a02u)
#define DAT_00799a04 (*(undefined4 *)0x00799a04u)
#define DAT_00799a06 (*(undefined4 *)0x00799a06u)
#define DAT_00799a08 (*(undefined4 *)0x00799a08u)
#define DAT_00799a76 (*(undefined4 *)0x00799a76u)
#define DAT_00799a78 (*(undefined4 *)0x00799a78u)
#define DAT_00799a7a (*(undefined4 *)0x00799a7au)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a84 (*(undefined4 *)0x00799a84u)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00799a8e (*(undefined4 *)0x00799a8eu)
#define DAT_00799aa4 (*(undefined4 *)0x00799aa4u)
#define DAT_00799adc (*(undefined4 *)0x00799adcu)
#define DAT_00799ade (*(undefined4 *)0x00799adeu)
#define DAT_009784d8 (*(undefined4 *)0x009784d8u)
#define DAT_00bcba4c (*(undefined4 *)0x00bcba4cu)
#define DAT_00bcd5f8 (*(undefined4 *)0x00bcd5f8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd603 (*(undefined4 *)0x00bcd603u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00bcd654 (*(undefined4 *)0x00bcd654u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd666 (*(undefined4 *)0x00bcd666u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd68a (*(undefined4 *)0x00bcd68au)
#define DAT_00bcd714 (*(undefined4 *)0x00bcd714u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00ea5f04 (*(undefined4 *)0x00ea5f04u)
#define DAT_00ea61f0 (*(undefined4 *)0x00ea61f0u)
#define DAT_00ea6210 (*(undefined4 *)0x00ea6210u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_00488050();
extern undefined4 FUN_004a43a0();
extern void FUN_004b3250();
extern void FUN_004b5420();

void FUN_004b4980(int param_1)

{
  short sVar1;
  char cVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined4 *puVar8;
  
  iVar4 = DAT_00eaac28;
  iVar7 = param_1 * 0x184;
  cVar2 = (&DAT_00bcd602)[iVar7];
  if (cVar2 == '9') {
    (&DAT_00ea5f04)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5f04)[DAT_00eaac28 * 0x1276] + 1;
  }
  if ((cVar2 == ']') || (cVar2 == 'd')) {
    iVar5 = iVar4 * 0x49d8;
    *(int *)(&DAT_00ea6210 + iVar5) = *(int *)(&DAT_00ea6210 + iVar5) + -1;
  }
  if ((((cVar2 == '\\') || (cVar2 == 'e')) || (cVar2 == 'M')) && ((&DAT_00bcd68a)[iVar7] != 0)) {
    (&DAT_00bcba4c)[(uint)(byte)(&DAT_00bcd68a)[iVar7] * 0x24] = 0;
  }
  if (cVar2 == '\x05') {
    if ((&DAT_00bcd652)[param_1 * 0xc2] != 0) {
      (&DAT_00799a04)[(short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x84] = 0;
    }
    goto LAB_004b4e5b;
  }
  if (cVar2 == '\'') {
    if ((&DAT_00bcd652)[param_1 * 0xc2] != 0) {
      (&DAT_00799a04)[(short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x84] = 0;
    }
    goto LAB_004b4e5b;
  }
  if ((cVar2 == 'N') || (cVar2 == 'M')) {
    FUN_004a43a0(param_1);
    goto LAB_004b4e5b;
  }
  if (cVar2 == '>') {
    *(undefined2 *)(&DAT_00799a08 + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x108) = 0;
    goto LAB_004b4e5b;
  }
  if ((((((cVar2 == '\x06') || (cVar2 == '\x0e')) ||
        ((cVar2 == ';' || (((cVar2 == '<' || (cVar2 == '=')) || (cVar2 == 'G')))))) ||
       ((cVar2 == 'A' || (cVar2 == 'D')))) || (cVar2 == 'R')) ||
     ((((cVar2 == 'F' || (cVar2 == 'T')) ||
       ((cVar2 == 'S' || (((cVar2 == 'g' || (cVar2 == 'h')) || (cVar2 == 'f')))))) ||
      (((cVar2 == 'm' || (cVar2 == 'B')) || (cVar2 == '(')))))) goto LAB_004b4e5b;
  if (((cVar2 == 'K') || (cVar2 == 'Z')) ||
     ((cVar2 == 'I' &&
      (*(char *)(&DAT_00799a7e + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x84) != '\0')))) {
    *(char *)(&DAT_00799a7e + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x84) =
         *(char *)(&DAT_00799a7e + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x84) + -1;
    goto LAB_004b4e5b;
  }
  if (cVar2 == 'b') {
    iVar4 = iVar4 * 0x49d8;
    if (*(int *)(&DAT_00ea61f0 + iVar4) < 1) {
      *(int *)(&DAT_00ea61f0 + iVar4) = 0;
    }
    else {
      *(int *)(&DAT_00ea61f0 + iVar4) = *(int *)(&DAT_00ea61f0 + iVar4) + -1;
    }
    goto LAB_004b4e5b;
  }
  if (cVar2 == '\x0f') {
    *(undefined2 *)(&DAT_00799a76 + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x108) = 0;
    goto LAB_004b4e5b;
  }
  if (cVar2 == '\x10') {
    *(undefined2 *)(&DAT_00799a78 + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x108) = 0;
    goto LAB_004b4e5b;
  }
  if (cVar2 == '\x11') {
    *(undefined2 *)(&DAT_00799a7a + (short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x42) = 0;
    goto LAB_004b4e5b;
  }
  if (cVar2 == 'U') {
    sVar1 = (&DAT_00bcd656)[param_1 * 0xc2];
    cVar2 = FUN_00488050((int)sVar1);
    if (cVar2 != '\0') {
      *(undefined2 *)(&DAT_00799a8c + sVar1 * 0x108) = 0;
    }
    goto LAB_004b4e5b;
  }
  iVar4 = (int)(short)(&DAT_00bcd652)[param_1 * 0xc2];
  if (cVar2 == '&') {
    iVar5 = 3;
    psVar3 = (short *)((int)&DAT_00799a84 + iVar4 * 0x108 + 2);
    do {
      if (*psVar3 == param_1) {
        *psVar3 = 0;
      }
      psVar3 = psVar3 + 1;
      iVar5 = iVar5 + -1;
    } while (iVar5 != 0);
    goto LAB_004b4e5b;
  }
  if (iVar4 == 0) goto LAB_004b4e5b;
  if ((short)(&DAT_00799a02)[iVar4 * 0x84] == param_1) {
    (&DAT_00799a02)[iVar4 * 0x84] = 0;
  }
  else if (*(short *)(&DAT_00799a8e + iVar4 * 0x108) == param_1) {
    *(undefined2 *)(&DAT_00799a8e + iVar4 * 0x108) = 0;
  }
  if ((cVar2 != '\x04') && (cVar2 != '\t')) goto LAB_004b4e5b;
  iVar4 = (int)(short)(&DAT_00bcd656)[param_1 * 0xc2];
  sVar1 = (&DAT_007999e8)[iVar4 * 0x84];
  if ((sVar1 < 0x6e) || (0x72 < sVar1)) {
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
      goto switchD_004b4cb8_caseD_20;
    }
  }
  else {
switchD_004b4cb8_caseD_20: ;
    if (*(short *)(&DAT_00bcd66c + DAT_00d056f8 * 0x184) != 0) {
      switch(*(undefined2 *)
              ((int)&DAT_00799aa4 + (short)(&DAT_00bcd656)[DAT_00d056f8 * 0xc2] * 0x108 + 2)) {
      case 1: ;
        uVar6 = 0x10;
        break;
default: ;
        uVar6 = 0xffffffff;
        break;
      case 3: ;
        uVar6 = 0x18;
        break;
      case 4: ;
        uVar6 = 0x14;
        break;
      case 5: ;
        uVar6 = 0xb;
        break;
      case 6: ;
        uVar6 = 0x19;
        break;
      case 7: ;
        uVar6 = 0xc;
        break;
      case 8: ;
        uVar6 = 10;
        break;
      case 9: ;
        uVar6 = 0xe;
        break;
      case 10: ;
        uVar6 = 0x12;
        break;
      case 0xb: ;
        uVar6 = 0x16;
        break;
      case 0xc: ;
        uVar6 = 0x17;
        break;
      case 0xd: ;
        uVar6 = 9;
        break;
      case 0xe: ;
        uVar6 = 0x1c;
        break;
      case 0xf: ;
        uVar6 = 0x15;
        break;
      case 0x10: ;
        uVar6 = 0x11;
        break;
      case 0x11: ;
        uVar6 = 0x1d;
        break;
      case 0x12: ;
        uVar6 = 0x1e;
        break;
      case 0x14: ;
        uVar6 = 0x20;
        break;
      case 0x15: ;
        uVar6 = 2;
        break;
      case 0x16: ;
        uVar6 = 8;
        break;
      case 0x17: ;
        uVar6 = 0x1f;
        break;
      case 0x18: ;
        uVar6 = 0xd;
        break;
      case 0x19: ;
        uVar6 = 0x21;
        break;
      case 0x1a: ;
        uVar6 = 0x22;
      }
      if (((&DAT_007999e8)[iVar4 * 0x84] == 0xcc) && ((byte)(&DAT_00bcd603)[iVar7] == uVar6)) {
        *(short *)(&DAT_00799ade + iVar4 * 0x108) = *(short *)(&DAT_00799ade + iVar4 * 0x108) + -1;
      }
      else {
        *(short *)(&DAT_00799adc + iVar4 * 0x108) = *(short *)(&DAT_00799adc + iVar4 * 0x108) + -1;
      }
      goto LAB_004b4e5b;
    }
  }
  iVar4 = (short)(&DAT_00bcd656)[DAT_00d056f8 * 0xc2] * 0x108;
  if (((&DAT_007999e8)[(short)(&DAT_00bcd656)[DAT_00d056f8 * 0xc2] * 0x84] == 0x47) &&
     (sVar1 = *(short *)(&DAT_00bcd66c + DAT_00d056f8 * 0x184), sVar1 != 0)) {
    *(short *)(&DAT_00799adc + iVar4) = *(short *)(&DAT_00799adc + iVar4) + -1;
    *(short *)(&DAT_00799ade + iVar4) = *(short *)(&DAT_00799ade + iVar4) - sVar1;
  }
LAB_004b4e5b: ;
  if ((&DAT_00bcd666)[iVar7] != 0) {
    uVar6 = (uint)(byte)(&DAT_00bcd666)[iVar7];
    iVar4 = uVar6 * 0x6a;
    *(undefined2 *)((int)&DAT_0077de54 + iVar4) = 0;
    if (*(short *)((int)&DAT_0077de58 + iVar4 + 2) == param_1) {
      *(undefined2 *)((int)&DAT_0077de58 + iVar4 + 2) = 0;
    }
    else if ((short)(&DAT_0077de5c)[uVar6 * 0x35] == param_1) {
      (&DAT_0077de5c)[uVar6 * 0x35] = 0;
    }
    else if (*(short *)(&DAT_0077de5e + iVar4) == param_1) {
      *(undefined2 *)(&DAT_0077de5e + iVar4) = 0;
    }
  }
  if (*(int *)(&DAT_00bcd714 + iVar7) != 0) {
    (&DAT_009784d8)[*(int *)(&DAT_00bcd714 + iVar7) * 0x2c] = 0;
  }
  if ((&DAT_00bcd654)[param_1 * 0xc2] != 0) {
    (&DAT_00799a06)[(short)(&DAT_00bcd652)[param_1 * 0xc2] * 0x84] = 0;
  }
  FUN_004b3250(param_1);
  FUN_004b5420(param_1);
  puVar8 = (undefined4 *)(&DAT_00bcd5f8 + iVar7);
  for (iVar4 = 0x61; iVar4 != 0; iVar4 = iVar4 + -1) {
    *puVar8 = 0;
    puVar8 = puVar8 + 1;
  }
  return;
}
