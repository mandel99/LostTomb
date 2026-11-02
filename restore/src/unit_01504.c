#include "restore_types.h"
#define DAT_005dcb1c (*(undefined4 *)0x005dcb1cu)
#define DAT_005dcb20 (*(undefined4 *)0x005dcb20u)
#define DAT_00bcb9a4 (*(undefined *)0x00bcb9a4u)
#define DAT_00bcb9a8 (*(undefined *)0x00bcb9a8u)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcb9f6 (*(undefined4 *)0x00bcb9f6u)
#define DAT_00bcb9f8 (*(undefined4 *)0x00bcb9f8u)
#define DAT_00bcbaa8 (*(undefined4 *)0x00bcbaa8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd688 (*(undefined4 *)0x00bcd688u)
#define DAT_00d0562c (*(undefined *)0x00d0562cu)
#define DAT_00d05630 (*(undefined *)0x00d05630u)
#define DAT_00d05638 (*(undefined *)0x00d05638u)
#define DAT_00d0563c (*(undefined *)0x00d0563cu)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define DAT_00ea5ee8 (*(undefined4 *)0x00ea5ee8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern int * FUN_0044bb40();
extern int * FUN_0044bb90();
extern void FUN_004b3250();
extern void FUN_004b9f90();
extern void FUN_004ba040();
extern void FUN_004ba090();
extern undefined4 FUN_004ba290();
extern uint FUN_004ba3b0();
extern void FUN_004ba690();
extern undefined4 FUN_004ba7e0();
extern bool FUN_0051a420();

void FUN_004b8cf0(void)

{
  bool bVar1;
  bool bVar2;
  bool bVar3;
  char cVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  byte bVar8;
  short sVar9;
  short sVar10;
  short *psVar11;
  short *psVar12;
  short *psVar13;
  int local_10;
  int local_c;
  int local_8;
  
  local_c = 0;
  if (DAT_00d0563c < 1) {
    for (iVar6 = FUN_0044bb90(0,1); iVar6 != 0; iVar6 = FUN_0044bb90(iVar6,1)) {
      *(undefined2 *)(iVar6 + 0x4a) = 0;
    }
    FUN_004ba690(0);
    return;
  }
  DAT_00d0562c = DAT_00d0562c + 1;
  if (4 < DAT_00d0562c) {
    DAT_00d0562c = 0;
    FUN_004ba090();
  }
  for (iVar6 = FUN_0044bb90(0,1); iVar6 != 0; iVar6 = FUN_0044bb90(iVar6,1)) {
    *(undefined2 *)(iVar6 + 0x3a) = 0;
    *(undefined2 *)(iVar6 + 0x48) = 0;
  }
  local_10 = 1;
  psVar12 = &DAT_00bcbaa8;
  do {
    if ((((char)psVar12[-0x28] == '\x01') && ((char)psVar12[-7] == '\0')) &&
       (*(char *)((int)psVar12 + 0x19) == '\0')) {
      iVar6 = FUN_0044bb40((int)psVar12[0x14]);
      if ((DAT_00d05630 * 2 < DAT_00d05638) && (psVar12[-0x26] != 0xb)) {
        *(undefined2 *)(iVar6 + 0x4a) = 1;
      }
      if (psVar12[6] != 0) {
        psVar12[6] = psVar12[6] + -1;
      }
      if (psVar12[7] != 0) {
        psVar12[7] = psVar12[7] + -1;
      }
      if (*psVar12 != 0) {
        *psVar12 = *psVar12 + -1;
      }
      if (*(int *)(&DAT_00ea5ee8 + DAT_00eaac28 * 0x49d8) < 1) {
        *psVar12 = 0;
        psVar12[7] = 0;
        psVar12[6] = 0;
      }
      psVar13 = psVar12 + -0x24;
      iVar6 = 0x10;
      psVar11 = psVar13;
      do {
        if ((((&DAT_00bcd630)[*psVar11 * 0x184] == '\x05') &&
            (iVar7 = *(short *)(&DAT_00bcd688 + *psVar11 * 0x184) * 0x184,
            (&DAT_00bcd606)[iVar7] == '\x01')) &&
           (((&DAT_00bcd630)[iVar7] != '\x04' &&
            ((10 < (byte)(&DAT_00bcd602)[iVar7] && ((byte)(&DAT_00bcd602)[iVar7] < 0xe)))))) {
          *psVar12 = 6;
        }
        psVar11 = psVar11 + 1;
        iVar6 = iVar6 + -1;
      } while (iVar6 != 0);
      if ((*(char *)((int)psVar12 + 0x23) == '\0') && ((char)psVar12[0xb] == '\0')) {
        iVar6 = (int)*psVar13;
        if ((iVar6 != 0) && ((&DAT_00bcd606)[iVar6 * 0x184] == '\x01')) {
          sVar9 = (&DAT_00bcd60e)[iVar6 * 0xc2];
          psVar12[-0x11] = (&DAT_00bcd60c)[iVar6 * 0xc2];
          psVar12[-0x10] = sVar9;
        }
        iVar6 = FUN_0044bb40((int)psVar12[0x14]);
        if (*(short *)(iVar6 + 0x3a) == 0) {
          *(short *)(iVar6 + 0x3a) = (short)local_10;
          *(short *)(iVar6 + 0x3c) = psVar12[-0x11];
          *(short *)(iVar6 + 0x3e) = psVar12[-0x10];
          *(short *)(iVar6 + 0x40) = psVar12[-0x13];
          local_c = 0;
          FUN_0051a420((int)psVar12[-0x11],(int)psVar12[-0x10],0xfffffffe,0xfffffffe,0,100000);
          iVar7 = FUN_004ba290((int)psVar12[-0x11],(int)psVar12[-0x10],0x10);
          if (iVar7 == 0) {
            iVar7 = FUN_004ba290((int)psVar12[-0x11],(int)psVar12[-0x10],0x20);
            if (iVar7 != 0) {
              local_c = 2;
            }
          }
          else {
            local_c = 1;
          }
          if (*(short *)(iVar6 + 0x4a) == 0) {
            if (local_c == 1) {
              *(undefined2 *)(iVar6 + 0x42) = DAT_00d3b4e4;
              *(undefined2 *)(iVar6 + 0x44) = DAT_00d3b4e0;
              *(undefined2 *)(iVar6 + 0x46) = 0;
              goto LAB_004b8fcc;
            }
          }
          else {
            local_c = 0;
          }
          cVar4 = FUN_004ba3b0(local_10);
          if (cVar4 == '\0') {
            bVar8 = (char)psVar12[0xc] + 1;
            *(byte *)(psVar12 + 0xc) = bVar8;
            if (0x30 < bVar8) {
              *(undefined1 *)(psVar12 + 0xb) = 1;
            }
          }
          else {
            *(undefined1 *)(psVar12 + 0xc) = 0;
          }
          *(short *)(iVar6 + 0x42) = psVar12[-0xb];
          *(short *)(iVar6 + 0x44) = psVar12[-10];
          *(short *)(iVar6 + 0x46) = psVar12[-9];
        }
LAB_004b8fcc: ;
        bVar1 = false;
        psVar12[4] = *(short *)(iVar6 + 0x48);
        *(short *)(iVar6 + 0x48) = *(short *)(iVar6 + 0x48) + 1;
        psVar12[-1] = psVar12[-1] + 1;
        psVar12[-0xb] = *(short *)(iVar6 + 0x42);
        psVar12[-10] = *(short *)(iVar6 + 0x44);
        psVar12[-9] = *(short *)(iVar6 + 0x46);
        bVar2 = false;
        bVar3 = false;
        local_8 = 0;
        if (psVar12[6] != 0) goto LAB_004b9189;
        if (psVar12[7] == 0) {
          if (psVar12[-1] < 0x20) {
            bVar2 = true;
            psVar12[1] = 4;
          }
          else if (*(short *)(iVar6 + 0x4a) == 0) {
            if ((*(short *)(iVar6 + 0x40) == 8) || (*(short *)(iVar6 + 0x40) == 0xc)) {
              sVar9 = psVar12[4];
              if (sVar9 == 0) {
                sVar10 = 4;
                sVar9 = 2;
              }
              else if (sVar9 == 1) {
                sVar10 = 4;
                sVar9 = 2;
              }
              else if (sVar9 == 2) {
                sVar10 = 5;
                sVar9 = 3;
              }
              else if (sVar9 == 3) {
                sVar10 = 5;
                sVar9 = 3;
              }
              else {
                sVar10 = 6;
                sVar9 = 4;
              }
              sVar5 = 2;
              if (local_c == 0) {
                sVar10 = sVar10 + 6;
                sVar9 = sVar9 + -1;
                sVar5 = 1;
              }
            }
            else if (local_c == 0) {
              sVar5 = 1;
              sVar10 = 0xc;
              sVar9 = 1;
            }
            else {
              sVar5 = 6;
              sVar10 = 4;
              sVar9 = 2;
            }
            if (psVar12[3] == 0) {
              if (psVar12[2] == 0) {
                psVar12[2] = 0;
                psVar12[3] = 0;
                psVar12[1] = psVar12[1] + -1;
                if (psVar12[1] < 1) {
                  psVar12[3] = sVar9;
                  FUN_004ba040(local_10);
                  goto LAB_004b9189;
                }
              }
              else {
                psVar12[1] = 0;
                psVar12[3] = 0;
                psVar12[2] = psVar12[2] + -1;
                bVar2 = true;
                if (psVar12[2] < 1) {
                  psVar12[1] = sVar10;
                  FUN_004ba040(local_10);
                  bVar2 = false;
                }
              }
            }
            else {
              bVar1 = true;
              psVar12[1] = 0;
              psVar12[2] = 0;
              psVar12[3] = psVar12[3] + -1;
              if (psVar12[3] < 1) {
                psVar12[2] = sVar5;
                FUN_004ba040(local_10);
LAB_004b9189: ;
                bVar2 = false;
                bVar1 = true;
              }
            }
          }
          else {
            bVar2 = false;
          }
        }
        else {
          local_8 = (int)psVar12[8];
          bVar3 = true;
        }
        if (0x2d < psVar12[-1]) {
          FUN_004b9f90();
        }
        if (bVar1) {
          psVar12[-0xb] = psVar12[-0x11];
          psVar12[-10] = psVar12[-0x10];
        }
        else if (bVar3) {
          if (local_8 < 1) {
            psVar12[-0xb] = *(short *)(iVar6 + 0x42);
            psVar12[-10] = *(short *)(iVar6 + 0x44);
          }
          else {
            local_8 = local_8 * 0x90;
            if ((&DAT_00bcb9f0)[local_8] != '\0') {
              psVar12[-0xb] = *(short *)(&DAT_00bcb9f6 + local_8);
              psVar12[-10] = *(short *)(&DAT_00bcb9f8 + local_8);
            }
          }
        }
        else if ((bVar2) &&
                (iVar7 = ((int)psVar12[4] +
                         ((uint)(*(byte *)((int)psVar12 + 0x21) >> 1) + *(short *)(iVar6 + 0x40) * 4
                         ) * 0x14) * 8,
                iVar6 = FUN_004ba7e0(local_10,*(int *)(&DAT_005dcb1c + iVar7) +
                                              (int)*(short *)(iVar6 + 0x3c),
                                     *(int *)(&DAT_005dcb20 + iVar7) + (int)*(short *)(iVar6 + 0x3e)
                                    ), sVar9 = DAT_00bcb9a4, iVar6 != 0)) {
          psVar12[-0xb] = DAT_00bcb9a8;
          psVar12[-10] = sVar9;
        }
      }
      else {
        iVar6 = 0x10;
        do {
          iVar7 = (int)*psVar13;
          cVar4 = (&DAT_00bcd630)[iVar7 * 0x184];
          if (((cVar4 != '\x05') && (cVar4 != '\x04')) && (cVar4 != '\x03')) {
            (&DAT_00bcd630)[iVar7 * 0x184] = 3;
            FUN_004b3250(iVar7);
          }
          psVar13 = psVar13 + 1;
          iVar6 = iVar6 + -1;
        } while (iVar6 != 0);
      }
    }
    psVar12 = psVar12 + 0x48;
    local_10 = local_10 + 1;
    if (0xbcd637 < (int)psVar12) {
      FUN_004ba690(0);
      return;
    }
  } while( true );
}
