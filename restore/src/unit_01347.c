#include "restore_types.h"
#define DAT_005cd6f8 (*(undefined4 *)0x005cd6f8u)
#define DAT_005cd6fc (*(undefined4 *)0x005cd6fcu)
#define DAT_007090e4 (*(undefined4 *)0x007090e4u)
#define DAT_007090e8 (*(undefined4 *)0x007090e8u)
#define DAT_007090ec (*(undefined4 *)0x007090ecu)
#define DAT_00bcb9f2 (*(undefined4 *)0x00bcb9f2u)
#define DAT_00bcba02 (*(undefined4 *)0x00bcba02u)
#define DAT_00bcba04 (*(undefined4 *)0x00bcba04u)
#define DAT_00bcba06 (*(undefined4 *)0x00bcba06u)
#define DAT_00bcba18 (*(undefined4 *)0x00bcba18u)
#define DAT_00bcba24 (*(undefined4 *)0x00bcba24u)
#define DAT_00bcd5f9 (*(undefined4 *)0x00bcd5f9u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd624 (*(undefined4 *)0x00bcd624u)
#define DAT_00bcd626 (*(undefined4 *)0x00bcd626u)
#define DAT_00bcd628 (*(undefined4 *)0x00bcd628u)
#define DAT_00bcd62a (*(undefined4 *)0x00bcd62au)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd658 (*(undefined4 *)0x00bcd658u)
#define DAT_00bcd65a (*(undefined4 *)0x00bcd65au)
#define DAT_00bcd65d (*(undefined4 *)0x00bcd65du)
#define DAT_00bcd662 (*(undefined4 *)0x00bcd662u)
#define DAT_00bcd663 (*(undefined4 *)0x00bcd663u)
#define DAT_00bcd66a (*(undefined4 *)0x00bcd66au)
#define DAT_00bcd67a (*(undefined4 *)0x00bcd67au)
#define DAT_00bcd67c (*(undefined4 *)0x00bcd67cu)
#define DAT_00bcd67e (*(undefined4 *)0x00bcd67eu)
#define DAT_00bcd680 (*(undefined4 *)0x00bcd680u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5eef (*(undefined4 *)0x00ea5eefu)
#define DAT_00ea5ef0 (*(undefined4 *)0x00ea5ef0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
#define s_wavs_drums_wav_005df6d8 (*(char (*)[15])0x005df6d8u)
#define s_wavs_horn1_wav_005df6b8 (*(char (*)[15])0x005df6b8u)
#define s_wavs_horn2_wav_005df6c8 (*(char (*)[15])0x005df6c8u)
extern undefined4 FUN_0047d350();
extern int FUN_004998f0();
extern int FUN_00499e50();
extern void FUN_004a5da0();
extern void FUN_004b17d0();
extern void FUN_004b1d50();
extern void FUN_004b3250();
extern int FUN_004b45c0();
extern void FUN_004b53b0();
extern undefined4 FUN_0051cba0();
extern undefined1 FUN_0051cd80();

void FUN_004991a0(void)

{
  short sVar1;
  short sVar2;
  undefined4 uVar3;
  byte bVar4;
  undefined1 uVar5;
  char cVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  char *pcVar12;
  undefined4 uVar13;
  
  iVar8 = DAT_00d056f8;
  iVar7 = DAT_00d056f8 * 0x184;
  iVar10 = (int)(short)(&DAT_00bcd658)[DAT_00d056f8 * 0xc2];
  (&DAT_00bcd66a)[iVar7] = 2;
  iVar9 = DAT_00eaac28 * 0x49d8;
  uVar3 = *(undefined4 *)(&DAT_007090ec + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38);
  (&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] = (&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] + 1;
  iVar11 = iVar10 * 0x90;
  sVar1 = *(short *)(&DAT_00bcb9f2 + iVar11);
  *(undefined2 *)(&DAT_00bcd628 + iVar7) =
       *(undefined2 *)(&DAT_005cd6f8 + ((uint)(byte)(&DAT_00bcd65a)[iVar7] + sVar1 * 0x10) * 8);
  *(undefined2 *)(&DAT_00bcd62a + iVar7) =
       *(undefined2 *)(&DAT_005cd6fc + ((uint)(byte)(&DAT_00bcd65a)[iVar7] + sVar1 * 0x10) * 8);
  cVar6 = (&DAT_00bcd630)[iVar7];
  if (cVar6 == '\x05') {
    FUN_004b17d0(iVar8);
    return;
  }
  if (cVar6 == '\x04') {
    FUN_004b1d50(iVar8);
    return;
  }
  if (cVar6 == '\x0e') {
    *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
    return;
  }
  if (cVar6 == '\t') {
    FUN_004b53b0(iVar8);
    iVar8 = DAT_00d056f8;
    (&DAT_00bcd5f9)[DAT_00d056f8 * 0x184] = 0;
    FUN_004b3250(iVar8);
    iVar8 = DAT_00d056f8;
    iVar7 = DAT_00d056f8 * 0x184;
    *(short *)(&DAT_00bcd62e + iVar7) = *(short *)(&DAT_00bcd62e + iVar7) + -1;
    if (*(short *)(&DAT_00bcd62e + iVar7) < 1) {
      *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0x32;
      if (((&DAT_00bcd65d)[iVar7] != '\0') && ((&DAT_00bcd65a)[iVar7] == '\0')) {
        if (*(short *)(&DAT_00bcb9f2 + iVar11) == 8) {
          pcVar12 = s_wavs_drums_wav_005df6d8;
        }
        else if (*(short *)(&DAT_00bcb9f2 + iVar11) == 0xc) {
          pcVar12 = s_wavs_horn2_wav_005df6c8;
        }
        else {
          pcVar12 = s_wavs_horn1_wav_005df6b8;
        }
        FUN_0047d350(pcVar12,1,0);
        iVar8 = DAT_00d056f8;
      }
      iVar7 = iVar8 * 0x184;
      sVar1 = *(short *)(&DAT_00bcba18 + iVar11);
      (&DAT_00bcd65d)[iVar7] = 0;
      if (sVar1 == 0) {
        sVar1 = (&DAT_00bcba04)[iVar10 * 0x48];
        *(short *)(&DAT_00bcd61c + iVar7) =
             (&DAT_00bcba02)[iVar10 * 0x48] + *(short *)(&DAT_00bcd628 + iVar7);
        *(short *)(&DAT_00bcd61e + iVar7) = sVar1 + *(short *)(&DAT_00bcd62a + iVar7);
        bVar4 = FUN_0051cba0((int)(short)(&DAT_00bcd60c)[iVar8 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[iVar8 * 0xc2],
                             (int)*(short *)(&DAT_00bcd61c + iVar7),
                             (int)(short)(sVar1 + *(short *)(&DAT_00bcd62a + iVar7)));
        iVar8 = DAT_00d056f8;
        if (bVar4 < 8) {
          (&DAT_00bcd630)[DAT_00d056f8 * 0x184] = 0xb;
        }
      }
      else {
        (&DAT_00bcd630)[iVar7] = 0xc;
      }
    }
    iVar7 = iVar8 * 0x184;
    cVar6 = (&DAT_00bcd602)[iVar7];
    if ((((cVar6 != '+') && (cVar6 != '.')) && (cVar6 != '3')) && ((cVar6 != '4' && (cVar6 != '7')))
       ) {
      return;
    }
    cVar6 = (&DAT_00bcd663)[iVar7];
    (&DAT_00bcd663)[iVar7] = cVar6 + 1U;
    if (*(int *)(&DAT_007090e8 + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38) <
        (int)(uint)(byte)(cVar6 + 1U)) {
      (&DAT_00bcd663)[iVar7] = 0;
      iVar7 = FUN_00499e50(iVar8,*(undefined4 *)
                                  (&DAT_007090e4 + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38));
      uVar3 = _DAT_00d3b4e4;
      iVar8 = DAT_00d056f8;
      if (iVar7 == 0) {
        iVar7 = DAT_00d056f8 * 0x184;
        (&DAT_00bcd662)[iVar7] = 0;
      }
      else {
        (&DAT_00bcd662)[DAT_00d056f8 * 0x184] = 1;
        iVar7 = iVar8 * 0x184;
        uVar5 = FUN_0051cd80((int)(short)(&DAT_00bcd60c)[iVar8 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[iVar8 * 0xc2],uVar3,_DAT_00d3b4e0);
        iVar8 = DAT_00d056f8;
        (&DAT_00bcd609)[iVar7] = uVar5;
      }
    }
    if ((&DAT_00bcd662)[iVar7] == '\0') {
      return;
    }
    if ((&DAT_00bcd662)[iVar7] == '\x01') {
      FUN_004b45c0(iVar8,(int)*(short *)((int)&DAT_00bcd60c + iVar7),
                   (int)*(short *)((int)&DAT_00bcd60e + iVar7),_DAT_00d3b4e4,_DAT_00d3b4e0,0x47);
      iVar8 = DAT_00d056f8;
      *(undefined2 *)(&DAT_00bcba24 + iVar11) = 6;
    }
    cVar6 = (&DAT_00bcd662)[iVar8 * 0x184];
    (&DAT_00bcd662)[iVar8 * 0x184] = cVar6 + 1U;
    if ((byte)(cVar6 + 1U) < 0x65) {
      return;
    }
    (&DAT_00bcd662)[iVar8 * 0x184] = 0;
    return;
  }
  if (cVar6 == '\x03') {
    *(undefined2 *)(&DAT_00bcd61c + iVar7) = *(undefined2 *)(&DAT_00bcd624 + iVar7);
    *(undefined2 *)(&DAT_00bcd61e + iVar7) = *(undefined2 *)(&DAT_00bcd626 + iVar7);
    FUN_004a5da0(iVar8,uVar3);
    cVar6 = (&DAT_00bcd609)[DAT_00d056f8 * 0x184];
    if (((cVar6 != '\t') && (cVar6 != '\n')) && (cVar6 != '\b')) {
      return;
    }
    (&DAT_00bcd606)[DAT_00d056f8 * 0x184] = 2;
    return;
  }
  if (cVar6 == '\n') {
    FUN_004b53b0(iVar8);
    return;
  }
  if (cVar6 == '\v') {
    *(short *)(&DAT_00bcd62e + iVar7) = *(short *)(&DAT_00bcd62e + iVar7) + -1;
    if (*(short *)(&DAT_00bcd62e + iVar7) < 1) {
      sVar1 = (&DAT_00bcba02)[iVar10 * 0x48];
      *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0x32;
      if (((int)*(short *)(&DAT_00bcd61c + iVar7) !=
           (int)sVar1 + (int)*(short *)(&DAT_00bcd628 + iVar7)) ||
         ((int)*(short *)(&DAT_00bcd61e + iVar7) !=
          (int)(short)(&DAT_00bcba04)[iVar10 * 0x48] + (int)*(short *)(&DAT_00bcd62a + iVar7))) {
        sVar2 = (&DAT_00bcba04)[iVar10 * 0x48];
        *(short *)(&DAT_00bcd61c + iVar7) = sVar1 + *(short *)(&DAT_00bcd628 + iVar7);
        *(short *)(&DAT_00bcd61e + iVar7) = sVar2 + *(short *)(&DAT_00bcd62a + iVar7);
        cVar6 = FUN_0051cba0((int)(short)(&DAT_00bcd60c)[iVar8 * 0xc2],
                             (int)(short)(&DAT_00bcd60e)[iVar8 * 0xc2],
                             (int)*(short *)(&DAT_00bcd61c + iVar7),
                             (int)(short)(sVar2 + *(short *)(&DAT_00bcd62a + iVar7)));
        iVar8 = DAT_00d056f8;
        if (cVar6 == '\b') {
          (&DAT_00bcd630)[DAT_00d056f8 * 0x184] = 9;
          return;
        }
        (&DAT_00bcd656)[DAT_00d056f8 * 0xc2] = (&DAT_00bcba06)[iVar10 * 0x48];
        FUN_004b3250(iVar8);
        iVar8 = DAT_00d056f8;
      }
    }
    FUN_004a5da0(iVar8,uVar3);
    iVar7 = DAT_00d056f8 * 0x184;
    cVar6 = (&DAT_00bcd609)[iVar7];
    if ((cVar6 != '\t') && (cVar6 != '\n')) {
      if (cVar6 != '\b') {
        return;
      }
      (&DAT_00bcd630)[iVar7] = 9;
      return;
    }
LAB_004998d6: ;
    (&DAT_00bcd630)[iVar7] = 9;
    return;
  }
  if (cVar6 != '\f') {
    if (cVar6 == '\r') {
      return;
    }
    goto LAB_004998d6;
  }
  if (*(short *)(&DAT_00bcba18 + iVar11) == 0) {
    (&DAT_00bcd630)[iVar7] = 9;
  }
  cVar6 = (&DAT_00bcd602)[iVar7];
  if ((cVar6 != '.') && (cVar6 != '/')) {
    if ((cVar6 == '4') || (cVar6 == '0')) {
      cVar6 = (&DAT_00ea5ef0)[iVar9];
      (&DAT_00ea5ef0)[iVar9] = cVar6 + -1;
      if ((char)(cVar6 + -1) < '\x01') {
        (&DAT_00ea5ef0)[iVar9] = 200;
        uVar13 = 0x17;
        goto LAB_00499744;
      }
    }
    else {
      cVar6 = (&DAT_00ea5eef)[iVar9];
      (&DAT_00ea5eef)[iVar9] = cVar6 + -1;
      if ((char)(cVar6 + -1) < '\x01') {
        (&DAT_00ea5eef)[iVar9] = 200;
        uVar13 = 0x1b;
LAB_00499744: ;
        FUN_0047d350(uVar13);
        iVar8 = DAT_00d056f8;
      }
    }
  }
  iVar7 = iVar8 * 0x184;
  iVar9 = (int)*(short *)(&DAT_00bcd67a + iVar7);
  if ((&DAT_00bcd606)[iVar9 * 0x184] != '\x01') {
    *(undefined2 *)(&DAT_00bcd67a + iVar7) = 0;
    iVar9 = 0;
  }
  if ((&DAT_00bcd630)[iVar9 * 0x184] == '\x04') {
    *(undefined2 *)(&DAT_00bcd67a + iVar7) = 0;
  }
  else if (0 < iVar9) goto LAB_00499850;
  iVar7 = FUN_004998f0((int)(short)(&DAT_00bcd60c)[iVar8 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar8 * 0xc2]);
  iVar8 = DAT_00d056f8;
  if (iVar7 != 0) {
    iVar9 = DAT_00d056f8 * 0x184;
    *(undefined2 *)(&DAT_00bcd61c + iVar9) = (&DAT_00bcd60c)[iVar7 * 0xc2];
    *(undefined2 *)(&DAT_00bcd61e + iVar9) = (&DAT_00bcd60e)[iVar7 * 0xc2];
    *(short *)(&DAT_00bcd67a + iVar9) = (short)iVar7;
    (&DAT_00bcd67c)[iVar7 * 0xc2] = (short)iVar8;
    *(undefined2 *)(&DAT_00bcd680 + iVar9) = (&DAT_00bcd67e)[iVar7 * 0xc2];
    FUN_004b3250(iVar8);
    iVar8 = DAT_00d056f8;
    if (0 < iVar7) {
LAB_00499850: ;
      FUN_004a5da0(iVar8,uVar3);
      iVar8 = DAT_00d056f8;
      iVar7 = DAT_00d056f8 * 0x184;
      cVar6 = (&DAT_00bcd609)[iVar7];
      if ((cVar6 != '\t') && (cVar6 != '\n')) {
        if (cVar6 != '\b') {
          return;
        }
        *(undefined2 *)(&DAT_00bcd61c + iVar7) =
             (&DAT_00bcd60c)[*(short *)(&DAT_00bcd67a + iVar7) * 0xc2];
        *(undefined2 *)(&DAT_00bcd61e + iVar7) =
             (&DAT_00bcd60e)[*(short *)(&DAT_00bcd67a + iVar7) * 0xc2];
        FUN_004b3250(iVar8);
        return;
      }
      (&DAT_00bcd630)[iVar7] = 9;
      *(undefined2 *)(&DAT_00bcd67a + iVar7) = 0;
      return;
    }
  }
  iVar8 = DAT_00d056f8 * 0x184;
  (&DAT_00bcd630)[iVar8] = 9;
  *(undefined2 *)(&DAT_00bcd62e + iVar8) = 0x32;
  return;
}
