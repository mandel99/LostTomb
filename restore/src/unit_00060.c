#include "restore_types.h"
#define DAT_00bcb9cb (*(undefined4 *)0x00bcb9cbu)
#define DAT_00bcb9cc (*(undefined4 *)0x00bcb9ccu)
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcba2e (*(undefined4 *)0x00bcba2eu)
#define DAT_00bcba3b (*(undefined4 *)0x00bcba3bu)
#define DAT_00bcba4a (*(undefined4 *)0x00bcba4au)
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
#define s_wavs_cohort5_wav_0057624c (*(char (*)[17])0x0057624cu)
extern undefined4 FUN_0047d350();
extern void FUN_004b6c20();
extern void FUN_004b7b50();

void FUN_00406fb0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  int iVar6;
  int iVar7;
  
  iVar1 = DAT_00eadccc;
  iVar7 = DAT_00eadccc * 0x90;
  if (((*(short *)(&DAT_00bcba4a + iVar7) < 0) && ((&DAT_00bcb9cb)[iVar7] != '\x01')) &&
     (DAT_00e38e60 == '\0')) {
    if (((*(short *)(&DAT_00bcb9cc + iVar7) == 0xd) &&
        (psVar5 = (short *)(&DAT_00bcb9d0 + iVar7),
        (&DAT_00bcd630)[*(short *)(&DAT_00bcb9d0 + iVar7) * 0x184] == '\x13')) &&
       (DAT_00d3b198 != 1)) {
      iVar4 = 0;
      iVar3 = DAT_00d056f8;
      do {
        if (*psVar5 == 0) break;
        iVar2 = (int)*psVar5;
        iVar6 = iVar2 * 0x184;
        if ((((&DAT_00bcd630)[iVar6] != '\x04') && ((&DAT_00bcba3b)[iVar7] == '\0')) &&
           ((byte)(&DAT_00bcba2e)[iVar7] < 2)) {
          if ((&DAT_00bcba2e)[iVar7] == 1) {
            FUN_004b7b50(iVar1,10);
            iVar3 = DAT_00d056f8;
          }
          (&DAT_00bcd5f8)[iVar6] = 0;
          *(undefined4 *)(&DAT_00bcd72c + iVar6) = 0;
          *(int *)(&DAT_00bcd734 + iVar6) = (int)(short)(&DAT_00bcd60c)[iVar2 * 0xc2];
          *(int *)(&DAT_00bcd738 + iVar6) = (int)(short)(&DAT_00bcd60e)[iVar2 * 0xc2];
          (&DAT_00bcd630)[iVar6] = 0xc;
          (&DAT_00bcd5f9)[iVar3 * 0x184] = 0;
        }
        iVar4 = iVar4 + 1;
        psVar5 = psVar5 + 1;
      } while (iVar4 < 0x10);
    }
    FUN_004b6c20(iVar1,0xe);
    FUN_0047d350(s_wavs_cohort5_wav_0057624c,1,0);
    DAT_00e38dda = 1;
    DAT_00e38dca = 1;
  }
  return;
}
