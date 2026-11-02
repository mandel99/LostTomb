#include "restore_types.h"
#define DAT_007090d0 (*(undefined4 *)0x007090d0u)
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785da6 (*(undefined4 *)0x00785da6u)
#define DAT_00bca428 (*(undefined4 *)0x00bca428u)
#define DAT_00bca45c (*(undefined4 *)0x00bca45cu)
#define DAT_00bcb9cb (*(undefined4 *)0x00bcb9cbu)
#define DAT_00bcb9d0 (*(undefined4 *)0x00bcb9d0u)
#define DAT_00bcb9f0 (*(undefined4 *)0x00bcb9f0u)
#define DAT_00bcba12 (*(undefined4 *)0x00bcba12u)
#define DAT_00bcba14 (*(undefined4 *)0x00bcba14u)
#define DAT_00bcba80 (*(undefined4 *)0x00bcba80u)
#define DAT_00bcba9a (*(undefined4 *)0x00bcba9au)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00d05630 (*(undefined *)0x00d05630u)
#define DAT_00d05634 (*(undefined *)0x00d05634u)
#define DAT_00d05638 (*(undefined *)0x00d05638u)
#define DAT_00d0563c (*(undefined *)0x00d0563cu)
#define DAT_00ea5ed5 (*(undefined4 *)0x00ea5ed5u)
#define DAT_00ea5ed6 (*(undefined4 *)0x00ea5ed6u)
#define DAT_00ea5eda (*(undefined4 *)0x00ea5edau)
#define DAT_00ea5edb (*(undefined4 *)0x00ea5edbu)
#define DAT_00ea5edc (*(undefined4 *)0x00ea5edcu)
#define DAT_00ea5edd (*(undefined4 *)0x00ea5eddu)
#define DAT_00ea5edf (*(undefined4 *)0x00ea5edfu)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern undefined4 FUN_0040b5e0();
extern void FUN_0040f8f0();
extern undefined4 FUN_0047d350();
extern undefined4 FUN_004ea9a0();
extern int FUN_004ea9c0();

void FUN_004b7c80(void)

{
  byte bVar1;
  short sVar2;
  char cVar3;
  short *psVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  undefined1 *puVar9;
  undefined1 *puVar10;
  byte *pbVar11;
  byte *pbVar12;
  char *pcVar13;
  short sVar14;
  undefined4 *puVar15;
  int local_8;
  int *local_4;
  
  local_8 = 0;
  puVar9 = &DAT_00bcba80;
  do {
    puVar10 = puVar9 + 0x90;
    puVar15 = (undefined4 *)(puVar9 + -0x20);
    for (iVar6 = 8; iVar6 != 0; iVar6 = iVar6 + -1) {
      *puVar15 = 0;
      puVar15 = puVar15 + 1;
    }
    *puVar9 = 0;
    puVar9[-0x25] = 1;
    *(undefined2 *)(puVar9 + 0x22) = 0;
    *(undefined2 *)(puVar9 + 0x24) = 0;
    puVar9 = puVar10;
  } while ((int)puVar10 < 0xbcd610);
  sVar14 = 1;
  pbVar12 = &DAT_00bcd786;
  do {
    if (((pbVar12[4] == 1) && (pbVar12[0x2e] != 4)) &&
       ((((bVar1 = *pbVar12, 10 < bVar1 && (bVar1 < 0xe)) ||
         ((((0x2a < bVar1 && (bVar1 < 0x36)) || (bVar1 == 0x39)) ||
          (((cVar3 = FUN_004ea9a0(bVar1), cVar3 != '\0' ||
            (cVar3 = FUN_004ea9c0(*pbVar12), cVar3 != '\0')) || (bVar1 = *pbVar12, bVar1 == 99))))))
        || (((bVar1 == 0x36 || (bVar1 == 0x37)) || (bVar1 == 0x38)))))) {
      sVar2 = *(short *)(pbVar12 + 0x56);
      iVar6 = sVar2 * 0x90;
      (&DAT_00bcb9f0)[iVar6] = (&DAT_00bcb9f0)[iVar6] + '\x01';
      *(short *)(&DAT_00bcba14 + iVar6) =
           *(short *)(&DAT_00bcba14 + iVar6) + *(short *)(&DAT_007090d0 + (uint)*pbVar12 * 0x38);
      *(ushort *)(&DAT_00bcba12 + iVar6) = *(short *)(&DAT_00bcba12 + iVar6) + (ushort)pbVar12[0x13]
      ;
      if (pbVar12[0x59] != 1) {
        (&DAT_00bcb9cb)[iVar6] = 0;
      }
      iVar7 = 0;
      psVar4 = (short *)(&DAT_00bcb9d0 + iVar6);
      do {
        if (*psVar4 == 0) {
          pbVar12[0x58] = (byte)iVar7;
          *(short *)(&DAT_00bcb9d0 + (iVar7 + sVar2 * 0x48) * 2) = sVar14;
          break;
        }
        iVar7 = iVar7 + 1;
        psVar4 = psVar4 + 1;
      } while (iVar7 < 0x10);
    }
    pbVar12 = pbVar12 + 0x184;
    sVar14 = sVar14 + 1;
  } while ((int)pbVar12 < 0xc8ad42);
  DAT_00d0563c = 0;
  DAT_00d05638 = 0;
  DAT_00d05634 = 0;
  DAT_00d05630 = 0;
  local_4 = &DAT_00bca45c;
  pbVar12 = &DAT_00bcba80;
  do {
    if ((pbVar12[-0x28] == 1) && (pbVar11 = pbVar12 + -0x20, pbVar12[0x41] == 0)) {
      bVar1 = *pbVar12;
      if (pbVar12[0x1a] == 0) {
        if (bVar1 == 0) {
          iVar6 = 0;
          if (0xbca45c < (int)local_4) {
            piVar8 = &DAT_00bca45c;
            do {
              if (*piVar8 == (int)*(short *)(pbVar12 + 0x50)) goto LAB_004b7f0f;
              iVar6 = iVar6 + 1;
              piVar8 = piVar8 + 1;
            } while (iVar6 < local_8);
          }
          *local_4 = (int)*(short *)(pbVar12 + 0x50);
          (&DAT_00bca428)[local_8] = 1;
          local_8 = local_8 + 1;
          local_4 = local_4 + 1;
LAB_004b7f0f: ;
          FUN_0040f8f0(pbVar12 + -0x28,0x90,0);
        }
        else {
          DAT_00d0563c = DAT_00d0563c + 1;
          DAT_00d05638 = DAT_00d05638 + (uint)bVar1;
        }
      }
      else if (bVar1 != 0) {
        sVar14 = *(short *)(pbVar12 + 0x32);
        pbVar12[0x32] = 1;
        pbVar12[0x33] = 0;
        iVar6 = 0;
        if (*pbVar12 != 0) {
          do {
            if ((*(short *)pbVar11 != 0) && ((&DAT_00bcd609)[*(short *)pbVar11 * 0x184] != '\b')) {
              pbVar12[0x32] = 0;
              pbVar12[0x33] = 0;
            }
            iVar6 = iVar6 + 1;
            pbVar11 = pbVar11 + 2;
          } while (iVar6 < (int)(uint)*pbVar12);
        }
        DAT_00d05634 = DAT_00d05634 + 1;
        DAT_00d05630 = DAT_00d05630 + (uint)*pbVar12;
        if (*(short *)(pbVar12 + -0x24) == 0xd) {
          if ((sVar14 == 0) && (*(short *)(pbVar12 + 0x32) == 1)) {
            FUN_0047d350(0x9e);
          }
          DAT_00d05630 = DAT_00d05630 + (uint)(*pbVar12 >> 1);
        }
      }
    }
    pbVar12 = pbVar12 + 0x90;
    if (0xbcd60f < (int)pbVar12) {
      iVar7 = 1;
      iVar6 = DAT_00eaac28 * 0x49d8;
      pcVar13 = &DAT_00bcba9a;
      (&DAT_00ea5edd)[iVar6] = 0;
      (&DAT_00ea5edf)[iVar6] = 0;
      (&DAT_00ea5edc)[iVar6] = 0;
      (&DAT_00ea5eda)[iVar6] = 0;
      (&DAT_00ea5edb)[iVar6] = 0;
      (&DAT_00ea5ed5)[iVar6] = 0;
      (&DAT_00ea5ed6)[iVar6] = 0;
      do {
        if (pcVar13[-0x42] == '\x01') {
          if (*pcVar13 == '\0') {
            iVar6 = 0;
            if (0 < local_8) {
              sVar14 = *(short *)(pcVar13 + 0x36);
              piVar8 = &DAT_00bca45c;
              do {
                if ((int)sVar14 == *piVar8) {
                  (&DAT_00bca428)[iVar6] = 0;
                }
                iVar6 = iVar6 + 1;
                piVar8 = piVar8 + 1;
              } while (iVar6 < local_8);
            }
          }
          else {
            iVar6 = FUN_0040b5e0(iVar7);
            iVar5 = DAT_00eaac28 * 0x49d8;
            (&DAT_00ea5edc)[iVar5] = (&DAT_00ea5edc)[iVar5] + '\x01';
            if (iVar6 == 0) {
              (&DAT_00ea5edb)[iVar5] = (&DAT_00ea5edb)[iVar5] + '\x01';
            }
            else {
              (&DAT_00ea5eda)[iVar5] = (&DAT_00ea5eda)[iVar5] + '\x01';
            }
            if ((pcVar13[0x25] != '\0') && (pcVar13[-0x1a] != '\0')) {
              (&DAT_00ea5edd)[iVar5] = (&DAT_00ea5edd)[iVar5] + '\x01';
              if (iVar6 == 0) {
                (&DAT_00ea5ed6)[iVar5] = (&DAT_00ea5ed6)[iVar5] + '\x01';
              }
              else {
                (&DAT_00ea5ed5)[iVar5] = (&DAT_00ea5ed5)[iVar5] + '\x01';
              }
            }
            (&DAT_00ea5edf)[iVar5] = (&DAT_00ea5edf)[iVar5] + pcVar13[-0x1a];
          }
        }
        pcVar13 = pcVar13 + 0x90;
        iVar7 = iVar7 + 1;
      } while ((int)pcVar13 < 0xbcd62a);
      iVar6 = 0;
      if (0 < local_8) {
        piVar8 = &DAT_00bca45c;
        do {
          if (((((&DAT_00bca428)[iVar6] != '\0') && (iVar7 = *piVar8, -1 < iVar7)) &&
              (iVar7 < DAT_00785d70)) && (iVar7 * 0x7c != -0x785d74)) {
            (&DAT_00785da6)[iVar7 * 0x7c] = 0;
          }
          iVar6 = iVar6 + 1;
          piVar8 = piVar8 + 1;
        } while (iVar6 < local_8);
      }
      return;
    }
  } while( true );
}
