#include "restore_types.h"
#define DAT_00bcb9c0 (*(undefined *)0x00bcb9c0u)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcb9f2 (*(undefined4 *)0x00bcb9f2u)
#define DAT_00bcba2a (*(undefined4 *)0x00bcba2au)
#define DAT_00bcba2e (*(undefined4 *)0x00bcba2eu)
#define DAT_00bcba3b (*(undefined4 *)0x00bcba3bu)
#define DAT_00bcba3f (*(undefined4 *)0x00bcba3fu)
#define DAT_00bcba44 (*(undefined4 *)0x00bcba44u)
#define DAT_00bcba46 (*(undefined4 *)0x00bcba46u)
#define DAT_00bcba4a (*(undefined4 *)0x00bcba4au)
#define DAT_00bcba54 (*(undefined4 *)0x00bcba54u)
#define DAT_00bcd5f8 (*(undefined4 *)0x00bcd5f8u)
#define DAT_00bcd5f9 (*(undefined4 *)0x00bcd5f9u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd72c (*(undefined4 *)0x00bcd72cu)
#define DAT_00bcd734 (*(undefined4 *)0x00bcd734u)
#define DAT_00bcd738 (*(undefined4 *)0x00bcd738u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d3b198 (*(undefined *)0x00d3b198u)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
#define DAT_00e38e60 (*(undefined1 *)0x00e38e60u)
#define DAT_00eadccc (*(undefined *)0x00eadcccu)
#define s_wavs_cohort1_wav_00576238 (*(char (*)[17])0x00576238u)
#define s_wavs_cohort2_wav_00576224 (*(char (*)[17])0x00576224u)
#define s_wavs_cohort3_wav_00576210 (*(char (*)[17])0x00576210u)
#define s_wavs_cohort4_wav_005761fc (*(char (*)[17])0x005761fcu)
extern undefined4 FUN_0047d350();
extern void FUN_004b7b50();

void FUN_00406720(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  int iVar7;
  int iVar8;
  char *pcVar9;
  
  iVar1 = DAT_00eadccc;
  iVar4 = DAT_00d3b198;
  iVar8 = DAT_00eadccc * 0x90;
  if (-1 < *(short *)(&DAT_00bcba4a + iVar8)) {
    return;
  }
  if (DAT_00e38e60 != '\0') {
    return;
  }
  if ((DAT_00d3b198 == 3) && (*(short *)(&DAT_00bcb9f2 + iVar8) != 6)) {
    *(short *)(&DAT_00bcba2a + iVar8) = *(short *)(&DAT_00bcb9f2 + iVar8);
  }
  if (((*(short *)(&DAT_00bcb9cc + iVar8) == 0xd) &&
      (psVar6 = (short *)(&DAT_00bcb9d0 + iVar8),
      (&DAT_00bcd630)[*(short *)(&DAT_00bcb9d0 + iVar8) * 0x184] == '\x13')) && (iVar4 != 1)) {
    iVar5 = 0;
    iVar3 = DAT_00d056f8;
    do {
      if (*psVar6 == 0) break;
      iVar2 = (int)*psVar6;
      iVar7 = iVar2 * 0x184;
      if ((((&DAT_00bcd630)[iVar7] != '\x04') && ((&DAT_00bcba3b)[iVar8] == '\0')) &&
         ((byte)(&DAT_00bcba2e)[iVar8] < 2)) {
        if ((&DAT_00bcba2e)[iVar8] == 1) {
          FUN_004b7b50(iVar1,10);
          iVar3 = DAT_00d056f8;
        }
        (&DAT_00bcd5f8)[iVar7] = 0;
        *(undefined4 *)(&DAT_00bcd72c + iVar7) = 0;
        *(int *)(&DAT_00bcd734 + iVar7) = (int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2];
        *(int *)(&DAT_00bcd738 + iVar7) = (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2];
        (&DAT_00bcd630)[iVar7] = 0xc;
        (&DAT_00bcd5f9)[iVar3 * 0x184] = 0;
        iVar4 = DAT_00d3b198;
      }
      iVar5 = iVar5 + 1;
      psVar6 = psVar6 + 1;
    } while (iVar5 < 0x10);
  }
  switch((int)(iVar4)) {
  case 0: ;
    if ((&DAT_00bcba3f)[iVar8] == '\0') {
      *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 1;
      pcVar9 = s_wavs_cohort1_wav_00576238;
    }
    else {
      *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 2;
      pcVar9 = s_wavs_cohort1_wav_00576238;
    }
    break;
  case 1: ;
    if (*(short *)(&DAT_00bcb9cc + iVar8) == 0xd) {
      if (*(short *)(&DAT_00bcba46 + iVar8) == 0) {
        if (*(short *)(&DAT_00bcb9f2 + iVar8) != 0) {
          *(short *)(&DAT_00bcba44 + iVar8) = *(short *)(&DAT_00bcb9f2 + iVar8);
        }
        *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 0;
        pcVar9 = s_wavs_cohort2_wav_00576224;
      }
      else {
LAB_00406904: ;
        pcVar9 = s_wavs_cohort2_wav_00576224;
      }
    }
    else {
      if ((&DAT_00bcba3f)[iVar8] == '\0') {
        *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 3;
        goto LAB_00406904;
      }
      *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 4;
      pcVar9 = s_wavs_cohort2_wav_00576224;
    }
    break;
  case 2: ;
    *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 0xf;
    pcVar9 = s_wavs_cohort3_wav_00576210;
    break;
  case 3: ;
    *(undefined2 *)(&DAT_00bcb9f2 + iVar8) = 6;
    pcVar9 = s_wavs_cohort4_wav_005761fc;
    break;
default: ;
    goto switchD_00406863_default;
  }
  FUN_0047d350(pcVar9,1,0);
switchD_00406863_default: ;
  DAT_00bcb9c0 = iVar1;
  if (*(short *)(&DAT_00bcb9f2 + iVar8) == 6) {
    *(undefined4 *)(&DAT_00bcba54 + iVar8) = 0xffffffff;
    DAT_00e38dda = 1;
    DAT_00e38dca = 1;
    return;
  }
  if (*(short *)(&DAT_00bcb9f2 + iVar8) != 0) {
    if ((*(short *)(&DAT_00bcba46 + iVar8) == 0) || (DAT_00e38dda = 9, DAT_00d3b198 != 1)) {
      DAT_00e38dda = 0x1e;
    }
    DAT_00e38dca = 1;
    *(undefined4 *)(&DAT_00bcba54 + iVar8) = 0xffffffff;
    return;
  }
  *(undefined4 *)(&DAT_00bcba54 + iVar8) = 0xffffffff;
  DAT_00e38dda = 0x49;
  DAT_00e38dca = 1;
  return;
}
