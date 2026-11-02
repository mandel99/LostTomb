#include "restore_types.h"
#define DAT_0056f7d8 (*(undefined4 *)0x0056f7d8u)
#define DAT_0056f7ec (*(undefined4 *)0x0056f7ecu)
#define DAT_005e4ef4 (*(undefined *)0x005e4ef4u)
#define DAT_0078d790 (*(undefined *)0x0078d790u)
#define DAT_0078d79c (*(undefined *)0x0078d79cu)
#define DAT_0078d7f0 (*(undefined *)0x0078d7f0u)
#define DAT_0078d7f4 (*(undefined *)0x0078d7f4u)
#define DAT_0078d9a8 (*(undefined4 *)0x0078d9a8u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a7e (*(undefined4 *)0x00799a7eu)
#define DAT_00799a8c (*(undefined4 *)0x00799a8cu)
#define DAT_00799b94 (*(undefined4 *)0x00799b94u)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd656 (*(undefined4 *)0x00bcd656u)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00bcd66c (*(undefined4 *)0x00bcd66cu)
#define DAT_00bcd672 (*(undefined4 *)0x00bcd672u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
#define DAT_00d3b4e0 (*(undefined *)0x00d3b4e0u)
#define DAT_00d3b4e4 (*(undefined *)0x00d3b4e4u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern uint FUN_0040ed70();
extern undefined4 FUN_004176a0();
extern undefined4 FUN_00459240();
extern int FUN_0045bb50();
extern undefined4 FUN_0047fe60();
extern undefined4 FUN_00480180();
extern int FUN_00488030();
extern uint FUN_004894a0();
extern void FUN_004b3250();
extern uint FUN_004e05f0();
extern undefined4 FUN_004e6c10();
extern int FUN_004efd90();
extern int FUN_004f29e0();
extern void FUN_004f3000();
extern undefined4 FUN_004f3370();
extern bool FUN_00519840();
extern uint FUN_005421d0();
extern undefined4 FUN_00559110();

void FUN_004beeb0(int param_1)

{
  short sVar1;
  bool bVar2;
  int iVar3;
  char cVar4;
  int iVar5;
  int in_ECX;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined1 uVar9;
  int iVar10;
  undefined2 uVar11;
  short *psVar12;
  int iVar13;
  bool bVar14;
  int local_7ec;
  int local_7e8;
  int local_7e4;
  undefined1 local_7d8 [4];
  undefined1 local_7d4 [4];
  int aiStack_7d0 [500];
  
  do {
    iVar6 = 0;
    DAT_0078d7f0 = 0;
    iVar10 = 1;
    iVar5 = *(int *)(in_ECX + 0x32fb4);
    bVar2 = true;
    local_7e8 = 0;
    if ((((iVar5 == 1) || (iVar5 == 2)) || (iVar5 == 3)) || (DAT_0078d790 < 1)) {
LAB_004bf051: ;
      iVar5 = 1;
      if (DAT_0078d790 < 1) {
LAB_004bf526: ;
        (&DAT_00bcd606)[param_1 * 0x184] = 2;
LAB_004bf8ab: ;
        iVar5 = 1;
        if (0 < DAT_0078d790) {
          psVar12 = &DAT_00799b94;
          do {
            if ((((char)psVar12[-0x5a] == '\x01') && (cVar4 = FUN_00488030(iVar5), cVar4 != '\0'))
               && (*psVar12 < 0)) {
              *psVar12 = 0;
            }
            iVar5 = iVar5 + 1;
            psVar12 = psVar12 + 0x84;
          } while (iVar5 <= DAT_0078d790);
        }
        return;
      }
      do {
        if ((&DAT_007999d8)[iVar5 * 0x108] == '\x01') {
          sVar1 = (&DAT_007999e8)[iVar5 * 0x84];
          uVar11 = (undefined2)iVar5;
          if (sVar1 == 0xb7) {
            if (*(byte *)((int)&DAT_00799a2c + iVar5 * 0x108 + 1) < 2) {
              iVar6 = FUN_004f29e0(iVar5,0);
              if ((iVar6 != 0) && (DAT_0078d7f0 < 500)) {
                (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
                aiStack_7d0[DAT_0078d7f0] = iVar6;
                DAT_0078d7f0 = DAT_0078d7f0 + 1;
              }
            }
            else {
              cVar4 = FUN_004f3370(iVar5,local_7d4,local_7d8,1);
              if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
                (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
                aiStack_7d0[DAT_0078d7f0] = 0;
                DAT_0078d7f0 = DAT_0078d7f0 + 1;
              }
            }
            iVar6 = DAT_0078d7f0;
            if (DAT_005e4ef4 == 5) {
              iVar5 = iVar5 + -1 +
                      *(int *)(&DAT_0056f7ec + (uint)*(byte *)(&DAT_00799a2c + iVar5 * 0x84) * 4);
            }
            else {
              iVar5 = iVar5 + -1 +
                      *(int *)(&DAT_0056f7d8 + (uint)*(byte *)(&DAT_00799a2c + iVar5 * 0x84) * 4);
            }
          }
          else if (sVar1 == 0xd5) {
            cVar4 = FUN_005421d0(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (sVar1 == 0xe3) {
            cVar4 = FUN_004176a0(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (sVar1 == 0xe4) {
            cVar4 = FUN_004e6c10(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (sVar1 == 0xe1) {
            cVar4 = FUN_0040ed70(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (sVar1 == 0xde) {
            cVar4 = FUN_004e05f0(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (sVar1 == 0xe5) {
            cVar4 = FUN_00559110(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (sVar1 == 0xea) {
            cVar4 = FUN_00559110(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (sVar1 == 0xeb) {
            cVar4 = FUN_00559110(iVar5,param_1,0);
            iVar6 = DAT_0078d7f0;
            if ((cVar4 != '\0') && (DAT_0078d7f0 < 500)) {
              (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
              aiStack_7d0[DAT_0078d7f0] = 0;
              DAT_0078d7f0 = DAT_0078d7f0 + 1;
              iVar6 = DAT_0078d7f0;
            }
          }
          else if (((sVar1 == 0xec) &&
                   (cVar4 = FUN_00559110(iVar5,param_1,0), iVar6 = DAT_0078d7f0, cVar4 != '\0')) &&
                  (DAT_0078d7f0 < 500)) {
            (&DAT_0078d9a8)[DAT_0078d7f0] = uVar11;
            aiStack_7d0[DAT_0078d7f0] = 0;
            DAT_0078d7f0 = DAT_0078d7f0 + 1;
            iVar6 = DAT_0078d7f0;
          }
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 <= DAT_0078d790);
      bVar14 = iVar6 == 0;
    }
    else {
      psVar12 = &DAT_00799b94;
      do {
        if ((((((char)psVar12[-0x5a] == '\x01') &&
              (cVar4 = FUN_00488030(iVar10), iVar6 = DAT_0078d7f0, cVar4 != '\0')) &&
             ((cVar4 = FUN_004894a0(iVar10), iVar6 = DAT_0078d7f0, cVar4 != '\0' &&
              ((*psVar12 == 0 && (iVar5 = FUN_00459240(iVar10), iVar6 = DAT_0078d7f0, iVar5 == 0))))
             )) && (psVar12[-0x4a] != 0)) &&
           (iVar5 = FUN_00480180((int)psVar12[-0x57],(int)psVar12[-0x56],
                                 *(undefined1 *)((int)psVar12 + -0xb1)), iVar6 = DAT_0078d7f0,
           iVar5 != 0)) {
          iVar7 = (int)psVar12[-0x57];
          iVar8 = (int)psVar12[-0x56];
          _DAT_00d3b4f8 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
          iVar5 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
          if (iVar7 < iVar5) {
            DAT_00d3b4fc = iVar5 - iVar7;
          }
          else if (iVar5 < iVar7) {
            DAT_00d3b4fc = iVar7 - iVar5;
          }
          else {
            DAT_00d3b4fc = 0;
          }
          if (iVar8 < _DAT_00d3b4f8) {
            _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar8;
          }
          else if (_DAT_00d3b4f8 < iVar8) {
            _DAT_00d3b4f8 = iVar8 - _DAT_00d3b4f8;
          }
          else {
            _DAT_00d3b4f8 = 0;
          }
          iVar5 = _DAT_00d3b4f8;
          if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
            iVar5 = DAT_00d3b4fc;
          }
          if (((iVar5 < 0x29) && (DAT_0078d79c < 1)) && (*(byte *)((int)psVar12 + -0xd) < 0x30)) {
            (&DAT_0078d9a8)[DAT_0078d7f0] = (short)iVar10;
            DAT_0078d7f0 = DAT_0078d7f0 + 1;
            iVar6 = DAT_0078d7f0;
            if (499 < DAT_0078d7f0) {
              DAT_0078d7f0 = 499;
              iVar6 = 499;
            }
          }
        }
        iVar10 = iVar10 + 1;
        psVar12 = psVar12 + 0x84;
      } while (iVar10 <= DAT_0078d790);
      bVar14 = false;
      if (iVar6 == 0) goto LAB_004bf051;
    }
    if (bVar14 || iVar6 < 0) goto LAB_004bf526;
    iVar5 = 0;
    local_7e4 = 10000;
    local_7ec = 0;
    DAT_0078d7f4 = 0;
    if (iVar6 < 1) goto LAB_004bf526;
    do {
      iVar13 = (int)(short)(&DAT_0078d9a8)[iVar5];
      iVar6 = aiStack_7d0[iVar5];
      DAT_0078d7f4 = iVar5 + 1;
      iVar8 = iVar13 * 0x108;
      _DAT_00d3b4f8 = (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2];
      iVar7 = (int)*(short *)(&DAT_007999e0 + iVar8);
      iVar5 = (int)*(short *)((int)&DAT_007999dc + iVar8 + 2);
      iVar10 = (int)(short)(&DAT_00bcd60c)[param_1 * 0xc2];
      if (iVar5 < iVar10) {
        DAT_00d3b4fc = iVar10 - iVar5;
      }
      else if (iVar10 < iVar5) {
        DAT_00d3b4fc = iVar5 - iVar10;
      }
      else {
        DAT_00d3b4fc = 0;
      }
      if (iVar7 < _DAT_00d3b4f8) {
        _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar7;
      }
      else if (_DAT_00d3b4f8 < iVar7) {
        _DAT_00d3b4f8 = iVar7 - _DAT_00d3b4f8;
      }
      else {
        _DAT_00d3b4f8 = 0;
      }
      iVar5 = _DAT_00d3b4f8;
      if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
        iVar5 = DAT_00d3b4fc;
      }
      cVar4 = FUN_00488030(iVar13);
      if (cVar4 != '\0') {
        iVar5 = iVar5 + (uint)*(byte *)((int)&DAT_00799a7e + iVar8 + 1);
      }
      iVar10 = local_7ec;
      iVar7 = local_7e8;
      iVar3 = local_7e4;
      if (((iVar5 < local_7e4) &&
          (cVar4 = FUN_00488030(iVar13), iVar10 = iVar13, iVar7 = iVar6, iVar3 = iVar5,
          cVar4 != '\0')) &&
         (iVar5 = FUN_00519840((int)(short)(&DAT_00bcd60c)[param_1 * 0xc2],
                               (int)(short)(&DAT_00bcd60e)[param_1 * 0xc2],
                               (int)(short)(&DAT_007999fe)[iVar13 * 0x84],
                               (int)(short)(&DAT_00799a00)[iVar13 * 0x84]), iVar5 == 0)) {
        *(undefined2 *)(&DAT_00799a8c + iVar8) = 0xffff;
        bVar2 = false;
        iVar10 = local_7ec;
        iVar7 = local_7e8;
        iVar3 = local_7e4;
      }
      local_7e4 = iVar3;
      local_7e8 = iVar7;
      local_7ec = iVar10;
      iVar5 = DAT_0078d7f4;
    } while (DAT_0078d7f4 < DAT_0078d7f0);
    if (local_7ec != 0) {
      cVar4 = FUN_00488030(local_7ec);
      if (cVar4 == '\0') {
        sVar1 = (&DAT_007999e8)[local_7ec * 0x84];
        if (sVar1 == 0xb7) {
          if (1 < *(byte *)((int)&DAT_00799a2c + local_7ec * 0x108 + 1)) {
            iVar5 = param_1 * 0x184;
            (&DAT_00bcd656)[param_1 * 0xc2] = (short)local_7ec;
            *(undefined2 *)(&DAT_00bcd660 + iVar5) = 0;
            (&DAT_00bcd630)[iVar5] = 0x11;
            *(undefined2 *)(&DAT_00bcd62e + iVar5) = 0;
            FUN_004f3000(param_1,local_7ec);
            goto LAB_004bf8ab;
          }
          cVar4 = FUN_004efd90((int)*(short *)(&DAT_00b4e6fc +
                                              *(int *)(&DAT_00bcd618 + local_7e8 * 0x184) * 2));
          if (cVar4 == '\0') {
            if (DAT_005e4ef4 == 3) goto switchD_004bf697_caseD_1;
            if (DAT_005e4ef4 == 5) goto switchD_004bf697_caseD_2;
            uVar9 = 0x19;
          }
          else {
            switch((int)(DAT_005e4ef4)) {
default: ;
switchD_004bf697_caseD_1: ;
              uVar9 = 0x18;
              break;
            case 2: ;
            case 5: ;
switchD_004bf697_caseD_2: ;
              uVar9 = 0xc;
            }
          }
          iVar5 = FUN_0045bb50(local_7ec,uVar9,1);
          if (iVar5 != 0) {
            iVar6 = param_1 * 0x184;
            (&DAT_00bcd672)[iVar6] = uVar9;
            *(undefined2 *)(&DAT_00bcd66c + iVar6) = 0;
            (&DAT_00bcd656)[param_1 * 0xc2] = (short)iVar5;
            *(undefined2 *)(&DAT_00bcd660 + iVar6) = (undefined2)local_7e8;
            uVar11 = DAT_00d3b4e4;
            (&DAT_00bcd630)[iVar6] = 0xf;
            *(undefined2 *)(&DAT_00bcd62e + iVar6) = 0;
            *(undefined2 *)(&DAT_00bcd61c + iVar6) = uVar11;
            *(undefined2 *)(&DAT_00bcd61e + iVar6) = DAT_00d3b4e0;
            *(undefined2 *)(&DAT_00bcd63c + iVar6) = 0;
            FUN_004b3250(param_1);
            *(short *)(&DAT_00bcd660 + local_7e8 * 0x184) = (short)param_1;
            goto LAB_004bf8ab;
          }
        }
        else {
          if (sVar1 == 0xd5) {
            cVar4 = FUN_005421d0(local_7ec,param_1,1);
LAB_004bf833: ;
            if (cVar4 == '\0') {
              (&DAT_00bcd606)[param_1 * 0x184] = 2;
            }
            goto LAB_004bf8ab;
          }
          if (sVar1 == 0xe3) {
            cVar4 = FUN_004176a0(local_7ec,param_1,1);
LAB_004bf865: ;
            if (cVar4 == '\0') {
              (&DAT_00bcd606)[param_1 * 0x184] = 2;
            }
            goto LAB_004bf8ab;
          }
          if (sVar1 == 0xe4) {
            cVar4 = FUN_004e6c10(local_7ec,param_1,1);
          }
          else {
            if (sVar1 == 0xe1) {
              cVar4 = FUN_0040ed70(local_7ec,param_1,1);
              goto LAB_004bf833;
            }
            if (sVar1 == 0xde) {
              cVar4 = FUN_004e05f0(local_7ec,param_1,1);
              goto LAB_004bf865;
            }
            if (sVar1 == 0xe5) {
              cVar4 = FUN_00559110(local_7ec,param_1,1);
            }
            else {
              if (sVar1 == 0xea) {
                cVar4 = FUN_00559110(local_7ec,param_1,1);
                goto LAB_004bf833;
              }
              if (sVar1 == 0xeb) {
                cVar4 = FUN_00559110(local_7ec,param_1,1);
                goto LAB_004bf865;
              }
              if (sVar1 != 0xec) goto LAB_004bf8ab;
              cVar4 = FUN_00559110(local_7ec,param_1,1);
            }
          }
          if (cVar4 != '\0') goto LAB_004bf8ab;
        }
      }
      else {
        iVar6 = local_7ec * 0x108;
        iVar5 = FUN_0047fe60((int)*(short *)((int)&DAT_007999dc + iVar6 + 2),
                             (int)*(short *)(&DAT_007999e0 + iVar6),(&DAT_007999db)[iVar6],2);
        uVar11 = DAT_00d3b4e4;
        if (iVar5 != 0) {
          *(short *)(&DAT_00799a8c + iVar6) = (short)param_1;
          iVar5 = DAT_00d056f8;
          iVar6 = param_1 * 0x184;
          (&DAT_00bcd656)[param_1 * 0xc2] = (short)local_7ec;
          (&DAT_00bcd630)[iVar6] = 10;
          *(undefined2 *)(&DAT_00bcd61c + iVar6) = uVar11;
          *(undefined2 *)(&DAT_00bcd61e + iVar5 * 0x184) = DAT_00d3b4e0;
          *(undefined2 *)(&DAT_00bcd63c + iVar6) = 0;
          goto LAB_004bf8ab;
        }
      }
      (&DAT_00bcd606)[param_1 * 0x184] = 2;
      goto LAB_004bf8ab;
    }
    if (bVar2) goto LAB_004bf526;
  } while( true );
}
