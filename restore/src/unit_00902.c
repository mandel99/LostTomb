#include "restore_types.h"
#define DAT_005794e8 (*(undefined *)0x005794e8u)
#define DAT_00579574 (*(undefined *)0x00579574u)
#define DAT_007027b9 (*(char *)0x007027b9u)
#define DAT_007027ba (*(char *)0x007027bau)
#define DAT_007027bf (*(char *)0x007027bfu)
#define DAT_00705eb8 (*(undefined4 *)0x00705eb8u)
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799ae5 (*(undefined4 *)0x00799ae5u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_00ea1894 (*(undefined4 *)0x00ea1894u)
#define DAT_00ea41dc (*(undefined4 *)0x00ea41dcu)
#define DAT_00ea41e0 (*(undefined4 *)0x00ea41e0u)
#define DAT_00ea4388 (*(undefined4 *)0x00ea4388u)
#define DAT_00ea438c (*(undefined4 *)0x00ea438cu)
#define DAT_00ea5e1c (*(undefined4 *)0x00ea5e1cu)
#define DAT_00ea5e20 (*(undefined4 *)0x00ea5e20u)
#define DAT_00ea5e24 (*(undefined4 *)0x00ea5e24u)
#define DAT_00ea5e28 (*(undefined4 *)0x00ea5e28u)
#define DAT_00ea60ac (*(undefined4 *)0x00ea60acu)
#define DAT_00ea60b0 (*(undefined4 *)0x00ea60b0u)
extern void FUN_00417ae0();
extern undefined4 FUN_004806a0();
extern undefined4 FUN_004c5340();
extern int FUN_004cde40();
extern int FUN_004cde60();

void FUN_0045d630(int param_1)

{
  int iVar1;
  short *psVar2;
  int *piVar3;
  undefined4 uVar4;
  int iVar5;
  int *piVar6;
  char *pcVar7;
  int iVar8;
  undefined4 *puVar9;
  bool bVar10;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24 [9];
  
  iVar8 = param_1 * 0x49d8;
  piVar3 = local_24;
  for (iVar5 = 9; iVar5 != 0; iVar5 = iVar5 + -1) {
    *piVar3 = 0;
    piVar3 = piVar3 + 1;
  }
  puVar9 = &DAT_00ea60ac + param_1 * 0x1276;
  for (iVar5 = 9; iVar1 = DAT_0078d790, iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar9 = 0;
    puVar9 = puVar9 + 1;
  }
  *(undefined4 *)(&DAT_00ea41dc + iVar8) = 0;
  *(undefined4 *)(&DAT_00ea41e0 + iVar8) = 0;
  (&DAT_00ea4388)[param_1 * 0x1276] = 0;
  (&DAT_00ea438c)[param_1 * 0x1276] = 0;
  (&DAT_00ea5e20)[param_1 * 0x1276] = 0;
  (&DAT_00ea5e24)[param_1 * 0x1276] = 0;
  (&DAT_00ea5e28)[param_1 * 0x1276] = 0;
  local_30 = 0;
  local_34 = 0;
  local_2c = 1;
  if (0 < iVar1) {
    pcVar7 = &DAT_00799ae5;
    local_28 = 0x84;
    do {
      if (pcVar7[-5] == '\x01') {
        if ((*(short *)(pcVar7 + 0xb) == 0x47) || (*(short *)(pcVar7 + 0xb) == 0xbf)) {
          pcVar7[0x3f] = '\0';
          iVar5 = FUN_004806a0((int)*(short *)(pcVar7 + 1),(int)*(short *)(pcVar7 + 3));
          if (iVar5 != 0) {
            pcVar7[0x3f] = '\x01';
            iVar5 = FUN_004cde60((int)*(short *)(pcVar7 + 0x3b),
                                 *(undefined4 *)(&DAT_00705eb8 + *(short *)(pcVar7 + 0xb) * 0x28));
            if (iVar5 < 100) {
              (&DAT_00ea5e20)[param_1 * 0x1276] = (&DAT_00ea5e20)[param_1 * 0x1276] + 1;
            }
            if (iVar5 < 0x32) {
              iVar5 = 0;
              psVar2 = (short *)(pcVar7 + 0x53);
              iVar1 = 8;
              do {
                iVar5 = iVar5 + *psVar2;
                psVar2 = psVar2 + 1;
                iVar1 = iVar1 + -1;
              } while (iVar1 != 0);
              (&DAT_00ea5e24)[param_1 * 0x1276] = (&DAT_00ea5e24)[param_1 * 0x1276] + 1;
              if (0 < iVar5) {
                (&DAT_00ea5e28)[param_1 * 0x1276] = (&DAT_00ea5e28)[param_1 * 0x1276] + 1;
              }
            }
            else {
              local_30 = local_30 + *(short *)(pcVar7 + 0x5d);
              iVar5 = 9;
              local_34 = local_34 + *(short *)(pcVar7 + 0x61);
              psVar2 = (short *)(pcVar7 + 0x51);
              piVar3 = &DAT_00ea60ac + param_1 * 0x1276;
              do {
                *piVar3 = *piVar3 + (int)*psVar2;
                psVar2 = psVar2 + 1;
                piVar3 = piVar3 + 1;
                iVar5 = iVar5 + -1;
              } while (iVar5 != 0);
              iVar5 = 8;
              do {
                iVar5 = iVar5 + -1;
              } while (iVar5 != 0);
              bVar10 = DAT_00e38ebc == 0;
              (&DAT_00ea438c)[param_1 * 0x1276] = (&DAT_00ea438c)[param_1 * 0x1276] + 1;
              if ((((bVar10) &&
                   ((((DAT_00e38ea8 != DAT_005794e8 || (DAT_007027ba != '\0')) ||
                     (DAT_007027b9 == '\0')) ||
                    ((local_34 < 400 || ((FUN_00417ae0(2), restore_unresolved_return(0x00417ae0u)), DAT_00e38ebc == 0)))))) &&
                  (DAT_00e38ea8 == DAT_00579574)) && ((DAT_007027bf == '\0' && (799 < local_30)))) {
                FUN_00417ae0(7);
              }
            }
          }
        }
        else if ((*pcVar7 != '\0') && (*(short *)(pcVar7 + 0x17) != 0)) {
          piVar3 = local_24;
          iVar5 = 0;
          do {
            piVar3 = piVar3 + 1;
            switch((int)(iVar5)) {
            case 0: ;
              iVar1 = 0;
              break;
            case 1: ;
              iVar1 = 1;
              break;
            case 2: ;
              iVar1 = 2;
              break;
            case 3: ;
              iVar1 = 3;
              break;
            case 4: ;
              iVar1 = 4;
              break;
            case 5: ;
              iVar1 = 5;
              break;
            case 6: ;
              iVar1 = 6;
              break;
            case 7: ;
              iVar1 = 7;
              break;
default: ;
              iVar1 = -1;
              break;
            case 0xc: ;
              iVar1 = 9;
              break;
            case 0xe: ;
              iVar1 = 8;
              break;
            case 0x10: ;
              iVar1 = 10;
              break;
            case 0x12: ;
              iVar1 = 0xb;
            }
            *piVar3 = *piVar3 + (int)(short)(&DAT_00799a2c)[iVar1 + local_28];
            iVar1 = iVar5 + 2;
            iVar5 = iVar5 + 1;
          } while (iVar1 < 9);
        }
      }
      local_2c = local_2c + 1;
      local_28 = local_28 + 0x84;
      pcVar7 = pcVar7 + 0x108;
    } while (local_2c <= DAT_0078d790);
  }
  piVar6 = local_24;
  piVar3 = &DAT_00ea60b0 + param_1 * 0x1276;
  iVar5 = 8;
  do {
    piVar6 = piVar6 + 1;
    if ((*piVar3 != 0) || (*piVar6 != 0)) {
      *(int *)(&DAT_00ea41dc + iVar8) = *(int *)(&DAT_00ea41dc + iVar8) + *piVar3;
      *(int *)(&DAT_00ea41e0 + iVar8) = *(int *)(&DAT_00ea41e0 + iVar8) + 1;
    }
    piVar3 = piVar3 + 1;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  uVar4 = FUN_004c5340(param_1);
  iVar5 = FUN_004cde40(*(undefined4 *)(&DAT_00ea1894 + iVar8),uVar4);
  (&DAT_00ea5e1c)[param_1 * 0x1276] = iVar5;
  if (iVar5 < 1) {
    (&DAT_00ea4388)[param_1 * 0x1276] = (uint)(0 < *(int *)(&DAT_00ea41dc + iVar8));
  }
  else {
    (&DAT_00ea4388)[param_1 * 0x1276] = *(int *)(&DAT_00ea41dc + iVar8) / iVar5;
  }
  if (DAT_00784bbc != 0) {
    *(undefined4 *)(&DAT_00ea41e0 + iVar8) = 1;
    (&DAT_00ea4388)[param_1 * 0x1276] = 0xc;
  }
  return;
}
