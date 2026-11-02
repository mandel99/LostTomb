#include "restore_types.h"
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799ae0 (*(undefined4 *)0x00799ae0u)
#define DAT_0079af92 (*(undefined4 *)0x0079af92u)
#define DAT_00e950e0 (*(undefined4 *)0x00e950e0u)
extern void FUN_004695d0();
extern void FUN_0046d100();
extern void FUN_00516f90();
extern void FUN_005173a0();
extern void FUN_0053b090();
#include <stdlib.h>

/* Original 004c45a0 compares signed 16-bit record fields, stride 264. */
static int __cdecl compare_record_age(const void *left, const void *right)
{
  int a = *(const short *)(0x007999eau + *(const int *)left * 0x108u);
  int b = *(const short *)(0x007999eau + *(const int *)right * 0x108u);
  return (a > b) - (a < b);
}

void FUN_004c45e0(void)

{
  undefined4 *puVar1;
  int iVar2;
  char *pcVar3;
  int iVar4;
  uint uVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  short *psVar9;
  int local_54;
  int local_50 [20];
  
  iVar7 = 0;
  local_54 = 0x13;
  iVar4 = 1;
  piVar6 = local_50;
  pcVar3 = &DAT_00799ae0;
  do {
    if (((9 < *(short *)(pcVar3 + 0x10)) && (*(short *)(pcVar3 + 0x10) < 0x1e)) &&
       (*pcVar3 == '\x01')) {
      *piVar6 = iVar4;
      iVar7 = iVar7 + 1;
      piVar6 = piVar6 + 1;
      if (0x13 < iVar7) goto LAB_004c4641;
    }
    pcVar3 = pcVar3 + 0x108;
    iVar4 = iVar4 + 1;
  } while ((int)pcVar3 < 0x89b6d8);
  if (0x13 < iVar7) {
LAB_004c4641: ;
    qsort(local_50,20,sizeof(local_50[0]),compare_record_age);
    iVar8 = 0x15;
    psVar9 = &DAT_0079af92;
    iVar4 = local_50[19];
    do {
      if (((9 < psVar9[-1]) && (psVar9[-1] < 0x1e)) &&
         (((char)psVar9[-9] == '\x01' && (*(short *)(0x007999eau + iVar4 * 0x108u) < *psVar9)))) {
        iVar4 = 0;
        piVar6 = local_50;
        do {
          iVar2 = iVar4;
          if (*(short *)(0x007999eau + *piVar6 * 0x108u) < *psVar9) break;
          iVar4 = iVar4 + 1;
          piVar6 = piVar6 + 1;
          iVar2 = local_54;
        } while (iVar4 < 0x14);
        local_54 = iVar2;
        if (local_54 < 0x13) {
          iVar4 = 0x13 - local_54;
          puVar1 = (undefined4 *)&local_50[20];
          do {
            puVar1[-1] = puVar1[-2];
            iVar4 = iVar4 + -1;
            puVar1 = puVar1 + -1;
          } while (iVar4 != 0);
        }
        local_50[local_54] = iVar8;
        iVar4 = local_50[19];
      }
      psVar9 = psVar9 + 0x84;
      iVar8 = iVar8 + 1;
    } while ((int)psVar9 < 0x89b6ea);
  }
  if (0 < iVar7) {
    piVar6 = local_50;
    do {
      iVar4 = *piVar6;
      DAT_00e950e0 = 0;
      FUN_0046d100(iVar4,0);
      FUN_004695d0(iVar4,1);
      FUN_0053b090(0x11);
      iVar4 = iVar4 * 0x108;
      uVar5 = *(const byte *)(0x007999dbu + iVar4);
      iVar8 = (int)*(short *)((int)&DAT_007999dc + iVar4 + 2);
      FUN_00516f90(iVar8 - uVar5,(int)*(short *)(0x007999e0u + iVar4) - uVar5,iVar8 + uVar5,
                   uVar5 + (int)*(short *)(0x007999e0u + iVar4),1);
      iVar8 = (int)*(short *)((int)&DAT_007999dc + iVar4 + 2);
      uVar5 = *(const byte *)(0x007999dbu + iVar4);
      FUN_005173a0(iVar8 - uVar5,(int)*(short *)(0x007999e0u + iVar4) - uVar5,iVar8 + uVar5,
                   uVar5 + (int)*(short *)(0x007999e0u + iVar4));
      piVar6 = piVar6 + 1;
      iVar7 = iVar7 + -1;
    } while (iVar7 != 0);
  }
  return;
}
