#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_005f3878 (*(undefined4 *)0x005f3878u)
#define DAT_005f5498 (*(undefined4 *)0x005f5498u)
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799aa0 (*(undefined4 *)0x00799aa0u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00eaac44 (*(undefined *)0x00eaac44u)
#define DAT_00eaae90 (*(undefined *)0x00eaae90u)
#define DAT_00eaaf58 (*(undefined *)0x00eaaf58u)
extern bool FUN_004ecea0();
extern void FUN_004ed360();
extern uint FUN_004eea10();
extern undefined4 FUN_004ef100();
extern void FUN_004ef450();
extern int FUN_004efd90();

int FUN_004efe30(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  byte bVar1;
  undefined4 uVar2;
  char cVar3;
  int iVar4;
  char *pcVar5;
  int iVar6;
  short sVar7;
  int iVar8;
  int iVar9;
  bool bVar10;
  int *local_8;
  int local_4;
  
  iVar8 = 1;
  do {
    iVar9 = iVar8 * 0x108;
    if (((&DAT_007999d8)[iVar9] == '\x01') && ((&DAT_007999e8)[iVar8 * 0x84] == 0xb7)) {
      if (*(short *)(&DAT_00799aa0 + iVar8 * 0x42) != 0) {
        FUN_004ef450(iVar8);
        local_4 = 0;
        if (0 < DAT_00eaaf58) {
          local_8 = &DAT_00eaac44;
          do {
            iVar6 = *local_8;
            iVar4 = FUN_004ef100(iVar6);
            if (iVar4 == 0) {
              if ((&DAT_00799a3c)[iVar6 * 0x84] != 0) {
                cVar3 = FUN_004eea10(iVar6);
                if (cVar3 == '\0') {
                  (&DAT_00799a3c)[iVar6 * 0x84] = 0;
                }
                if ((&DAT_00799a3c)[iVar6 * 0x84] != 0) goto switchD_004efef8_default;
              }
              cVar3 = FUN_004efd90(iVar6);
              switch((int)(DAT_005e4ef4)) {
              case 1: ;
              case 3: ;
              case 4: ;
              case 5: ;
switchD_004efef8_caseD_1: ;
                bVar10 = DAT_005e4ef4 == 5;
                (&DAT_00799a3c)[iVar6 * 0x84] = (short)param_3;
                if (bVar10) {
                  iVar9 = (iVar8 + *(int *)(&DAT_005f5498 +
                                           (uint)*(byte *)(&DAT_00799a2c + iVar8 * 0x84) * 0x80)) *
                          0x108;
                  *(undefined2 *)(&DAT_00bcd61c + param_3 * 0x184) =
                       *(undefined2 *)((int)&DAT_007999dc + iVar9 + 2);
                  sVar7 = *(short *)(&DAT_007999e0 + iVar9) + 2;
                }
                else {
                  iVar8 = iVar8 + *(int *)(&DAT_005f3878 +
                                          (uint)*(byte *)(&DAT_00799a2c + iVar8 * 0x84) * 0x3c0);
                  iVar9 = iVar8 * 0x108;
                  *(undefined2 *)(&DAT_00bcd61c + param_3 * 0x184) = (&DAT_007999fe)[iVar8 * 0x84];
                  sVar7 = (&DAT_00799a00)[iVar8 * 0x84];
                }
                param_3 = param_3 * 0x184;
                uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar9);
                *(short *)(&DAT_00bcd61e + param_3) = sVar7;
                *(undefined4 *)(&DAT_00bcd620 + param_3) = uVar2;
                *(undefined2 *)(&DAT_00bcd63c + param_3) = 0;
                return iVar6;
              case 2: ;
                if (param_4 == 0) {
                  if (cVar3 != '\0') goto switchD_004efef8_caseD_1;
                }
                else if (cVar3 == '\0') goto switchD_004efef8_caseD_1;
              }
            }
switchD_004efef8_default: ;
            local_4 = local_4 + 1;
            local_8 = local_8 + 1;
          } while (local_4 < DAT_00eaaf58);
        }
      }
      if ((param_4 == 1) && (*(byte *)((int)&DAT_00799a2c + iVar9 + 1) < 2)) {
        bVar1 = *(byte *)(&DAT_00799a2c + iVar8 * 0x84);
        cVar3 = FUN_004ecea0((uint)bVar1);
        if ((cVar3 != '\0') &&
           (iVar9 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) + *(int *)(&DAT_007999e4 + iVar9)) -
                    DAT_00bca2d8, (FUN_004ed360(iVar9 % 0xe4,iVar9 / 0xe4,3,0), restore_unresolved_return(0x004ed360u)), iVar9 = DAT_00eaae90,
           DAT_00eaae90 != 0)) {
          iVar6 = 1;
          pcVar5 = &DAT_00bcd786;
          while ((((param_3 == iVar6 || (pcVar5[4] == '\0')) ||
                  (*pcVar5 != (&DAT_00bcd602)[param_3 * 0x184])) ||
                 ((*(int *)(pcVar5 + 0x1e) != DAT_00eaae90 || (*(short *)(pcVar5 + 0x54) != iVar8)))
                 )) {
            pcVar5 = pcVar5 + 0x184;
            iVar6 = iVar6 + 1;
            if (0xc8ad41 < (int)pcVar5) {
              iVar4 = DAT_00eaae90 - DAT_00bca2d8;
              iVar6 = param_3 * 0x184;
              *(short *)(&DAT_00bcd61c + iVar6) = (short)(iVar4 % 0xe4);
              *(short *)(&DAT_00bcd61e + iVar6) =
                   ((short)(iVar4 / 0xe4) + (short)(iVar4 >> 0x1f)) -
                   (short)((longlong)iVar4 * 0x8fb823ef >> 0x3f);
              *(int *)(&DAT_00bcd620 + iVar6) = iVar9;
              *(undefined2 *)(&DAT_00bcd63c + iVar6) = 0;
              (&DAT_00bcd656)[param_3 * 0xc2] = (short)iVar8;
              return iVar8;
            }
          }
        }
      }
      if (DAT_005e4ef4 == 5) {
        iVar9 = *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar8 * 0x84) * 4);
      }
      else {
        iVar9 = *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar8 * 0x84) * 4);
      }
      iVar8 = iVar9 + -1 + iVar8;
    }
    iVar8 = iVar8 + 1;
    if (3999 < iVar8) {
      return 0;
    }
  } while( true );
}
