#include "restore_types.h"
#define DAT_00bcbabc (*(undefined4 *)0x00bcbabcu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00d0563c (*(undefined *)0x00d0563cu)
#define DAT_00ea5c9c (*(undefined4 *)0x00ea5c9cu)
#define DAT_00ea5ea4 (*(undefined4 *)0x00ea5ea4u)
#define DAT_00ea5f98 (*(undefined4 *)0x00ea5f98u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_004b3250();

void FUN_004b8b20(void)

{
  char cVar1;
  short *psVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  short *psVar6;
  
  psVar6 = &DAT_00bcbabc;
  do {
    if (((char)psVar6[-0x32] == '\x01') && ((char)psVar6[-0x11] != '\0')) {
      if (*psVar6 != 0) {
        *psVar6 = *psVar6 + -1;
      }
      if (psVar6[-4] != 0) {
        psVar6[-4] = psVar6[-4] + -1;
      }
      if (psVar6[-3] != 0) {
        psVar6[-3] = psVar6[-3] + -1;
      }
      if (psVar6[-10] != 0) {
        psVar6[-10] = psVar6[-10] + -1;
      }
      iVar5 = DAT_00eaac28;
      if ((int)(&DAT_00ea5ea4)[DAT_00eaac28 * 0x1276] < 1) {
        psVar6[-10] = 0;
        psVar6[-3] = 0;
        psVar6[-4] = 0;
      }
      psVar3 = psVar6 + -0x2e;
      iVar4 = 0x10;
      psVar2 = psVar3;
      do {
        if ((&DAT_00bcd630)[*psVar2 * 0x184] == '\x05') {
          psVar6[-10] = 6;
        }
        psVar2 = psVar2 + 1;
        iVar4 = iVar4 + -1;
      } while (iVar4 != 0);
      if ((*(char *)((int)psVar6 + 0xf) == '\0') && ((char)psVar6[1] == '\0')) {
        if (psVar6[-0x1d] == 6) {
          if ((&DAT_00ea5c9c)[iVar5 * 0x1276] + (&DAT_00ea5f98)[iVar5 * 0x1276] + DAT_00d0563c < 1)
          {
            psVar6[-0x1d] = psVar6[-1];
          }
          else {
            iVar5 = 0x10;
            do {
              iVar4 = (int)*psVar3;
              if (0 < iVar4) {
                if (((&DAT_00bcd630)[iVar4 * 0x184] != '\x05') &&
                   ((&DAT_00bcd630)[iVar4 * 0x184] != '\x04')) {
                  (&DAT_00bcd630)[iVar4 * 0x184] = 0xe;
                }
              }
              psVar3 = psVar3 + 1;
              iVar5 = iVar5 + -1;
            } while (iVar5 != 0);
          }
        }
      }
      else {
        iVar5 = 0x10;
        do {
          iVar4 = (int)*psVar3;
          cVar1 = (&DAT_00bcd630)[iVar4 * 0x184];
          if (((cVar1 != '\x05') && (cVar1 != '\x04')) && (cVar1 != '\x03')) {
            (&DAT_00bcd630)[iVar4 * 0x184] = 3;
            FUN_004b3250(iVar4);
          }
          psVar3 = psVar3 + 1;
          iVar5 = iVar5 + -1;
        } while (iVar5 != 0);
      }
    }
    psVar6 = psVar6 + 0x48;
  } while ((int)psVar6 < 0xbcbd8d);
  return;
}
