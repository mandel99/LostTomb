#include "restore_types.h"
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)
#define DAT_00a76aeb (*(undefined4 *)0x00a76aebu)
#define DAT_00a76aec (*(undefined4 *)0x00a76aecu)
#define DAT_00bc9694 (*(undefined *)0x00bc9694u)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define DAT_00e950dc (*(undefined *)0x00e950dcu)
#define DAT_00ea3fe8 (*(undefined4 *)0x00ea3fe8u)
#define DAT_00ea3fea (*(undefined4 *)0x00ea3feau)
#define DAT_00ea3ff0 (*(undefined4 *)0x00ea3ff0u)
#define DAT_00ea3ff2 (*(undefined4 *)0x00ea3ff2u)
#define DAT_00ea3ff4 (*(undefined4 *)0x00ea3ff4u)
#define DAT_00ea5ee4 (*(undefined4 *)0x00ea5ee4u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eed75c (*(undefined4 *)0x00eed75cu)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)
extern undefined4 FUN_00401af0();
extern undefined4 FUN_0040eb90();
extern undefined4 FUN_00417490();
extern void FUN_00430cd0();
extern void FUN_00458270();
extern void FUN_00459020();
extern int FUN_00475b50();
extern void FUN_00476530();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_0047b1a0();
extern void FUN_0047bae0();
extern undefined4 FUN_0047fe60();
extern int FUN_00481060();
extern int FUN_00481210();
extern int FUN_00487d00();
extern undefined4 FUN_004e0c50();
extern void FUN_004e0cf0();
extern void FUN_004e2970();
extern undefined4 FUN_004e6ae0();
extern undefined4 FUN_004f30b0();
extern void FUN_00517370();
extern void FUN_00517660();
extern void FUN_005178e0();
extern bool FUN_00517a40();
extern bool FUN_00517c20();
extern undefined4 FUN_00542f40();
extern undefined4 FUN_00545a30();
extern undefined4 FUN_0054f8a0();
extern undefined4 FUN_00556230();
extern undefined4 FUN_00558f00();

void FUN_00457af0(void)

{
  int iVar1;
  bool bVar2;
  short sVar3;
  short sVar4;
  char cVar5;
  short sVar6;
  int iVar7;
  int iVar8;
  short *psVar9;
  int iVar10;
  int local_8;
  int local_4;
  
  local_4 = 0;
  FUN_00517a40((int)*(short *)(&DAT_00ea3fe8 + DAT_00eaac28 * 0x49d8),
               (int)*(short *)(&DAT_00ea3fea + DAT_00eaac28 * 0x49d8),0xfffffffe,0xfffffffe);
  iVar8 = 1;
  iVar10 = 0;
  if (0 < DAT_0078d790) {
    psVar9 = &DAT_00799af0;
    do {
      if ((char)psVar9[-8] == '\x01') {
        if (*(char *)((int)psVar9 + -0xb) == '\0') {
          sVar6 = *psVar9;
          if ((sVar6 == 0x48) || (sVar6 == 0xc1)) {
            if ((&DAT_00ea5ee4)[DAT_00eaac28 * 0x1276] == 0) {
              (&DAT_00ea5ee4)[DAT_00eaac28 * 0x1276] = iVar8;
            }
            psVar9[8] = 0;
            iVar10 = FUN_00481060((int)psVar9[-5],(int)psVar9[-4],3);
            if (iVar10 != 0) {
              local_8 = iVar10 + -1;
LAB_004580bd: ;
              sVar6 = *(short *)(&DAT_00eed760 + local_8);
              *(undefined *)(psVar9 + 2) = (&DAT_00a76aec)[local_8];
              psVar9[8] = sVar6;
LAB_004580d2: ;
              sVar6 = DAT_00d3b4e0;
              psVar9[0xb] = DAT_00d3b4e4;
              psVar9[0xc] = sVar6;
            }
          }
          else {
            if (sVar6 == 0xd2) {
              psVar9[8] = 0;
              iVar10 = FUN_00487d00((int)psVar9[-5],(int)psVar9[-4],5);
              goto LAB_00457c5c;
            }
            if (sVar6 == 0xd5) {
              psVar9[8] = 0;
              cVar5 = FUN_00542f40(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
              if (cVar5 != '\0') {
                sVar6 = DAT_00d3b4e0;
                psVar9[0xb] = DAT_00d3b4e4;
                psVar9[0xc] = sVar6;
              }
            }
            else {
              if (sVar6 == 0xe1) {
                psVar9[8] = 0;
                cVar5 = FUN_0040eb90(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                goto LAB_00457cf9;
              }
              if (sVar6 == 0xe6) {
                psVar9[8] = 0;
                cVar5 = FUN_00401af0(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                iVar7 = _DAT_00d3b4e4;
                iVar10 = _DAT_00d3b4e0;
                if (cVar5 != '\0') {
                  iVar1 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4;
                  local_8 = iVar1 + _DAT_00d3b4e4;
                  *(undefined *)(psVar9 + 2) = (&DAT_00a76aec)[_DAT_00d3b4e4 + iVar1];
                  psVar9[8] = *(short *)(&DAT_00eed760 + local_8);
                  psVar9[0xb] = (short)iVar7;
                  psVar9[0xc] = (short)iVar10;
                }
              }
              else if (sVar6 == 0xe3) {
                psVar9[8] = 0;
                cVar5 = FUN_00417490(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                iVar7 = _DAT_00d3b4e4;
                iVar10 = _DAT_00d3b4e0;
                if (cVar5 != '\0') {
                  iVar1 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4;
                  local_8 = iVar1 + _DAT_00d3b4e4;
                  *(undefined *)(psVar9 + 2) = (&DAT_00a76aec)[_DAT_00d3b4e4 + iVar1];
                  psVar9[8] = *(short *)(&DAT_00eed760 + local_8);
                  psVar9[0xb] = (short)iVar7;
                  psVar9[0xc] = (short)iVar10;
                }
              }
              else if (sVar6 == 0xe4) {
                psVar9[8] = 0;
                cVar5 = FUN_004e6ae0(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                iVar7 = _DAT_00d3b4e4;
                iVar10 = _DAT_00d3b4e0;
                if (cVar5 != '\0') {
                  iVar1 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4;
                  local_8 = iVar1 + _DAT_00d3b4e4;
                  *(undefined *)(psVar9 + 2) = (&DAT_00a76aec)[_DAT_00d3b4e4 + iVar1];
                  psVar9[8] = *(short *)(&DAT_00eed760 + local_8);
                  psVar9[0xb] = (short)iVar7;
                  psVar9[0xc] = (short)iVar10;
                }
              }
              else if (sVar6 == 0xde) {
                psVar9[8] = 0;
                cVar5 = FUN_004e0c50(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                if (cVar5 != '\0') goto LAB_004580d2;
              }
              else if (sVar6 == 0xe5) {
                psVar9[8] = 0;
                cVar5 = FUN_00545a30(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                if (cVar5 != '\0') {
                  local_8 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4;
LAB_00457d19: ;
                  iVar7 = _DAT_00d3b4e4;
                  iVar10 = _DAT_00d3b4e0;
                  local_8 = local_8 + _DAT_00d3b4e4;
                  sVar6 = *(short *)(&DAT_00eed760 + local_8);
                  *(undefined *)(psVar9 + 2) = (&DAT_00a76aec)[local_8];
                  psVar9[8] = sVar6;
                  psVar9[0xb] = (short)iVar7;
                  psVar9[0xc] = (short)iVar10;
                }
              }
              else {
                if (sVar6 == 0xea) {
                  psVar9[8] = 0;
                  cVar5 = FUN_00558f00(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                }
                else if (sVar6 == 0xeb) {
                  psVar9[8] = 0;
                  cVar5 = FUN_00556230(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                }
                else {
                  if (sVar6 != 0xec) {
                    if (((((((sVar6 == 0x41) || (sVar6 == 0x42)) || (sVar6 == 0x43)) ||
                          ((sVar6 == 0x44 || (sVar6 == 0x45)))) ||
                         ((sVar6 == 0x9c || ((sVar6 == 0x9d || (sVar6 == 0x9e)))))) ||
                        (sVar6 == 0x9f)) || (sVar6 == 0xa0)) {
                      psVar9[8] = 0;
                      iVar10 = FUN_00481210((int)psVar9[-5],(int)psVar9[-4],3);
LAB_00457c5c: ;
                      if (iVar10 != 0) {
                        sVar6 = *(short *)(&DAT_00eed75c + iVar10 * 4);
                        local_8 = iVar10 + -1;
                        *(undefined *)(psVar9 + 2) = (&DAT_00a76aeb)[iVar10];
                        sVar3 = DAT_00d3b4e0;
                        sVar4 = DAT_00d3b4e4;
                        psVar9[8] = sVar6;
                        psVar9[0xb] = sVar4;
                        psVar9[0xc] = sVar3;
                      }
                    }
                    else if (sVar6 == 0x49) {
                      psVar9[8] = 0;
                      iVar10 = FUN_00475b50(iVar8);
                      *(undefined1 *)(psVar9 + 2) = (&DAT_007999ec)[iVar10 * 0x108];
                      psVar9[8] = (&DAT_007999f8)[iVar10 * 0x84];
                      psVar9[0xb] = (&DAT_007999fe)[iVar10 * 0x84];
                      psVar9[0xc] = (&DAT_00799a00)[iVar10 * 0x84];
                    }
                    else {
                      psVar9[8] = 0;
                      if (sVar6 != 0xb7) {
                        iVar10 = FUN_00481060((int)psVar9[-5],(int)psVar9[-4],
                                              *(undefined1 *)((int)psVar9 + -0xd));
                        goto LAB_00457c5c;
                      }
                      cVar5 = FUN_004f30b0(iVar8,&local_8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                      if (cVar5 != '\0') goto LAB_004580bd;
                    }
                    goto LAB_004580e8;
                  }
                  psVar9[8] = 0;
                  cVar5 = FUN_0054f8a0(iVar8,&DAT_00d3b4e4,&DAT_00d3b4e0);
                }
LAB_00457cf9: ;
                if (cVar5 != '\0') {
                  local_8 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4;
                  goto LAB_00457d19;
                }
              }
            }
          }
        }
        else {
          iVar7 = FUN_0047fe60((int)psVar9[-5],(int)psVar9[-4],*(undefined1 *)((int)psVar9 + -0xd),2
                              );
          if (iVar7 == 0) {
            psVar9[8] = 0;
            psVar9[10] = psVar9[10] + 1;
            if (4 < psVar9[10]) {
              if (psVar9[6] != 0) {
                FUN_00459020(DAT_00eaac28,(int)psVar9[-5],(int)psVar9[-4],(int)psVar9[6]);
                psVar9[6] = 0;
                psVar9[10] = 0;
              }
              *(undefined1 *)(psVar9 + -8) = 2;
            }
          }
          else if ((&DAT_00eed760)[DAT_00bc9768] == 0) {
            psVar9[8] = 0;
            bVar2 = false;
            if ((*psVar9 == 10) && (psVar9[6] == 0)) {
              bVar2 = true;
            }
            if ((psVar9[10] == 0) && ((bVar2 || (iVar10 == 0)))) {
              local_4 = *(int *)(psVar9 + -2);
            }
            sVar6 = psVar9[10] + 1;
            psVar9[10] = sVar6;
            if ((8 < sVar6) && (psVar9[10] = 0, bVar2)) {
              *(undefined1 *)(psVar9 + -8) = 2;
            }
          }
          else {
            psVar9[8] = *(short *)(&DAT_00eed760 + DAT_00bc9768);
            psVar9[10] = 0;
          }
        }
      }
LAB_004580e8: ;
      iVar8 = iVar8 + 1;
      psVar9 = psVar9 + 0x84;
      iVar10 = local_4;
    } while (iVar8 <= DAT_0078d790);
  }
  iVar8 = DAT_00eaac28 * 0x49d8;
  if ((&DAT_00eed760)[*(int *)(&DAT_00ea3ff4 + iVar8)] == 0) {
    iVar10 = 0;
    do {
      FUN_00517c20((int)*(short *)(&DAT_00ea3fe8 + iVar8),(int)*(short *)(&DAT_00ea3fea + iVar8),
                   0xfffffffe,0xfffffffe);
      FUN_00517c20((int)*(short *)(&DAT_00ea3ff0 + DAT_00eaac28 * 0x49d8),
                   (int)*(short *)(&DAT_00ea3ff2 + DAT_00eaac28 * 0x49d8),0xfffffffe,0xfffffffe);
      FUN_00517370();
      FUN_00517660();
      FUN_005178e0();
      FUN_0047b1a0();
      FUN_0047bae0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0);
      FUN_004767d0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0,0);
      FUN_00477500(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1);
      FUN_00476530();
      FUN_00517a40((int)*(short *)(&DAT_00ea3fe8 + DAT_00eaac28 * 0x49d8),
                   (int)*(short *)(&DAT_00ea3fea + DAT_00eaac28 * 0x49d8),0xfffffffe,0xfffffffe);
      iVar8 = DAT_00eaac28 * 0x49d8;
      if ((&DAT_00eed760)[*(int *)(&DAT_00ea3ff4 + iVar8)] != 0) {
        DAT_00e950dc = 1;
        FUN_004e0cf0(0x13b,0,0,0,0);
        DAT_00bc9694 = 0;
        return;
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 < 0xf);
    FUN_00458270();
  }
  else if (iVar10 != 0) {
    FUN_004e2970(0x3f,0,0);
    FUN_004e2970(0x40,0,0);
    FUN_00430cd0(iVar10);
    return;
  }
  return;
}
