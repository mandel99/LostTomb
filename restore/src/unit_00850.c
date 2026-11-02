#include "restore_types.h"
#define DAT_005d06ec (*(undefined4 *)0x005d06ecu)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799a18 (*(undefined4 *)0x00799a18u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00799b20 (*(undefined4 *)0x00799b20u)
#define DAT_00ea43a0 (*(undefined4 *)0x00ea43a0u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
extern void FUN_0043f060();
extern undefined4 FUN_00459240();
extern undefined4 FUN_00488050();
extern void FUN_004a36b0();
extern int FUN_004cde40();

void FUN_00459670(void)

{
  short sVar1;
  uint uVar2;
  char cVar3;
  int *piVar4;
  uint *puVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  short *psVar9;
  int local_2c;
  uint local_28 [10];
  
  puVar5 = local_28;
  iVar8 = 10;
  piVar4 = (int *)(&DAT_00ea43a0 + DAT_00eaac28 * 0x49d8);
  do {
    *puVar5 = (uint)(*piVar4 < piVar4[-1]);
    piVar4 = piVar4 + 5;
    puVar5 = puVar5 + 1;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  local_2c = 1;
  if (0 < DAT_0078d790) {
    psVar9 = &DAT_00799af0;
    do {
      if ((((char)psVar9[-8] == '\x01') &&
          ((((iVar8 = *(int *)(&DAT_005d06ec + *psVar9 * 0x14), iVar8 != 1 && (iVar8 != 0)) ||
            (cVar3 = FUN_00488050(local_2c), cVar3 == '\0')) && (-1 < iVar8)))) &&
         ((iVar8 != 3 || (psVar9[0x14] == 0)))) {
        psVar9[0x18] = 0;
        if ((iVar8 == 1) || (iVar8 == 0)) {
          iVar7 = FUN_00459240(local_2c);
joined_r0x00459778: ;
          if (iVar7 != 0) goto LAB_004597f8;
        }
        else if (iVar8 == 2) {
          sVar1 = *psVar9;
          if ((((sVar1 == 0x1f) || (sVar1 == 0x1e)) || (sVar1 == 0x21)) && (psVar9[0x14] != 0))
          goto LAB_004597f8;
        }
        else if ((iVar8 == 8) && ((*psVar9 == 0x70 || (*psVar9 == 0xcd)))) {
          iVar7 = FUN_00459240(local_2c);
          goto joined_r0x00459778;
        }
        if (0 < psVar9[5]) {
          uVar2 = local_28[iVar8];
          if (uVar2 == 0) {
            psVar9[0x18] = *(short *)(&DAT_00705eb8 + *psVar9 * 0x28);
          }
          else {
            iVar6 = FUN_004cde40(*(undefined4 *)
                                  (&DAT_00ea43a0 + iVar8 * 0x14 + DAT_00eaac28 * 0x49d8),
                                 (int)psVar9[5]);
            iVar7 = iVar6 / 100;
            if (*(int *)(&DAT_00705eb8 + *psVar9 * 0x28) <= iVar6 / 100) {
              iVar7 = *(int *)(&DAT_00705eb8 + *psVar9 * 0x28);
            }
            psVar9[0x18] = (short)iVar7;
            local_28[iVar8] = uVar2 + iVar7;
          }
        }
      }
LAB_004597f8: ;
      local_2c = local_2c + 1;
      psVar9 = psVar9 + 0x84;
    } while (local_2c <= DAT_0078d790);
  }
  puVar5 = local_28;
  iVar8 = 10;
  do {
    if (*puVar5 != 0) {
      *puVar5 = *puVar5 - 1;
    }
    puVar5 = puVar5 + 1;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  piVar4 = (int *)(&DAT_00ea43a0 + DAT_00eaac28 * 0x49d8);
  puVar5 = local_28;
  iVar8 = 10;
  do {
    if ((int)*puVar5 < *piVar4) {
      *puVar5 = *piVar4 - *puVar5;
    }
    else {
      *puVar5 = 0;
    }
    piVar4 = piVar4 + 5;
    puVar5 = puVar5 + 1;
    iVar8 = iVar8 + -1;
  } while (iVar8 != 0);
  iVar8 = 1;
  if (0 < DAT_0078d790) {
    psVar9 = &DAT_00799b20;
    do {
      if ((char)psVar9[-0x20] == '\x01') {
        iVar7 = *(int *)(&DAT_005d06ec + psVar9[-0x18] * 0x14);
        if (iVar7 == 0) {
          cVar3 = FUN_00488050(iVar8);
          if (cVar3 == '\0') {
LAB_004598bc: ;
            iVar6 = FUN_00459240(iVar8);
            if (iVar6 == 0) {
LAB_004598c9: ;
              if (0 < psVar9[-0x13]) {
                uVar2 = local_28[iVar7];
                puVar5 = local_28 + iVar7;
                if (uVar2 != 0) {
                  sVar1 = *psVar9;
                  if ((int)sVar1 < *(int *)(&DAT_00705eb8 + psVar9[-0x18] * 0x28)) {
                    iVar7 = *(int *)(&DAT_00705eb8 + psVar9[-0x18] * 0x28) - (int)sVar1;
                    if ((int)uVar2 < iVar7) {
                      uVar2 = *puVar5;
                      *puVar5 = 0;
                      *psVar9 = sVar1 + (short)uVar2;
                    }
                    else {
                      *psVar9 = sVar1 + (short)iVar7;
                      *puVar5 = uVar2 - iVar7;
                    }
                  }
                }
              }
            }
          }
        }
        else if ((-1 < iVar7) && (iVar7 != 8)) {
          if (iVar7 == 1) goto LAB_004598bc;
          if ((iVar7 != 2) || (psVar9[-4] == 0)) goto LAB_004598c9;
        }
      }
      iVar8 = iVar8 + 1;
      psVar9 = psVar9 + 0x84;
    } while (iVar8 <= DAT_0078d790);
  }
  iVar8 = 1;
  if (0 < DAT_0078d790) {
    psVar9 = &DAT_00799af0;
    do {
      if (((char)psVar9[-8] == '\x01') && (*psVar9 == 0x88)) {
        if ((psVar9[0x18] == 0) || ((short)(&DAT_00799a18)[*(int *)(psVar9 + 0x5a) * 0x84] < 1)) {
          FUN_0043f060(iVar8,1,3);
          FUN_004a36b0(iVar8);
        }
        else {
          FUN_0043f060(iVar8,1,1);
        }
      }
      iVar8 = iVar8 + 1;
      psVar9 = psVar9 + 0x84;
    } while (iVar8 <= DAT_0078d790);
  }
  return;
}
