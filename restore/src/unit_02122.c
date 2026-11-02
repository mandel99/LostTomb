#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd646 (*(undefined4 *)0x00bcd646u)
#define DAT_00bcd648 (*(undefined4 *)0x00bcd648u)
#define DAT_00bcd718 (*(undefined4 *)0x00bcd718u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern void FUN_004ef450();
extern void FUN_004f02f0();
extern bool FUN_004f1d10();
extern int FUN_004f2190();
extern void FUN_004f2410();

void FUN_004f2230(int param_1)

{
  short sVar1;
  int iVar2;
  char cVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int *piVar9;
  int local_8;
  
  iVar7 = param_1 * 0x184;
  if (*(short *)(&DAT_00bcd718 + iVar7) < 1) {
    (&DAT_00bcd630)[iVar7] = 0xc;
    return;
  }
  cVar3 = FUN_004f1d10(param_1,1);
  if (cVar3 != '\x01') {
    return;
  }
  FUN_004f2410(param_1);
  iVar6 = (int)*(short *)(&DAT_00bcd646 + iVar7);
  FUN_004ef450(iVar6 - (short)(&DAT_007999ea)[iVar6 * 0x84]);
  iVar8 = 0;
  local_8 = iVar6;
  if (0 < DAT_00eaaf58) {
    piVar9 = &DAT_00eaac44;
    do {
      iVar2 = *piVar9;
      if (iVar2 == iVar6) {
        if ((short)(&DAT_00799a3c)[iVar2 * 0x84] < 1) {
LAB_004f2308: ;
          (&DAT_00bcd630)[iVar7] = 0xb;
          *(undefined2 *)(&DAT_00bcd62e + iVar7) = 0;
          (&DAT_00799a3c)[iVar2 * 0x84] = (undefined2)param_1;
          return;
        }
        iVar4 = (short)(&DAT_00799a3c)[iVar2 * 0x84] * 0x184;
        if ((((&DAT_00bcd602)[iVar4] != 'Q') || ((&DAT_00bcd630)[iVar4] != '\v')) &&
           (((&DAT_00bcd602)[iVar4] != 'P' || ((&DAT_00bcd630)[iVar4] != '\f')))) goto LAB_004f2308;
      }
      iVar8 = iVar8 + 1;
      piVar9 = piVar9 + 1;
    } while (iVar8 < DAT_00eaaf58);
  }
  do {
    iVar8 = 0;
    if (0 < DAT_00eaaf58) {
      piVar9 = &DAT_00eaac44;
      do {
        iVar2 = *piVar9;
        if (iVar2 == local_8) {
          if ((short)(&DAT_00799a3c)[iVar2 * 0x84] < 1) {
LAB_004f23c7: ;
            sVar1 = *(short *)(&DAT_00bcd648 + iVar7);
            (&DAT_00799a3c)[iVar2 * 0x84] = (undefined2)param_1;
            uVar5 = FUN_004f2190(iVar6,(int)sVar1,1);
            FUN_004f02f0(param_1,uVar5);
            return;
          }
          iVar4 = (short)(&DAT_00799a3c)[iVar2 * 0x84] * 0x184;
          if ((((&DAT_00bcd602)[iVar4] != 'Q') || ((&DAT_00bcd630)[iVar4] != '\v')) &&
             (((&DAT_00bcd602)[iVar4] != 'P' || ((&DAT_00bcd630)[iVar4] != '\f'))))
          goto LAB_004f23c7;
        }
        iVar8 = iVar8 + 1;
        piVar9 = piVar9 + 1;
      } while (iVar8 < DAT_00eaaf58);
    }
    local_8 = FUN_004f2190(local_8);
    if (local_8 == iVar6) {
      (&DAT_00bcd630)[iVar7] = 0xc;
      *(undefined2 *)(&DAT_00bcd718 + iVar7) = 0;
      return;
    }
  } while( true );
}
