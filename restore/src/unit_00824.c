#include "restore_types.h"
#define DAT_00784bbc (*(undefined *)0x00784bbcu)
#define DAT_0078d788 (*(undefined *)0x0078d788u)
#define DAT_007900bc (*(undefined4 *)0x007900bcu)
#define DAT_007900c0 (*(undefined4 *)0x007900c0u)
#define DAT_007900c1 (*(undefined4 *)0x007900c1u)
#define DAT_007900c2 (*(undefined4 *)0x007900c2u)
#define DAT_007999d8 (*(undefined4 *)0x007999d8u)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ec (*(undefined4 *)0x007999ecu)
#define DAT_007999f8 (*(undefined4 *)0x007999f8u)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a1c (*(undefined4 *)0x00799a1cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a76 (*(undefined4 *)0x00799a76u)
#define DAT_00799a98 (*(undefined4 *)0x00799a98u)
#define DAT_0089b6d8 (*(undefined4 *)0x0089b6d8u)
#define DAT_0089b744 (*(undefined4 *)0x0089b744u)
#define DAT_00904ec0 (*(undefined4 *)0x00904ec0u)
#define DAT_00904ec4 (*(undefined4 *)0x00904ec4u)
#define DAT_0096e640 (*(undefined4 *)0x0096e640u)
#define DAT_0096e644 (*(undefined4 *)0x0096e644u)
#define DAT_0096e664 (*(undefined4 *)0x0096e664u)
#define DAT_0096e668 (*(undefined4 *)0x0096e668u)
#define DAT_0096e66c (*(undefined4 *)0x0096e66cu)
#define DAT_0096e670 (*(undefined4 *)0x0096e670u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define DAT_00eed760 (*(undefined4 *)0x00eed760u)
#define PTR_LAB_00457818 (*(unsigned char * *)0x00457818u)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern void FUN_00455e00();
extern void FUN_00456070();
extern undefined4 FUN_004561e0();
extern undefined4 FUN_004565f0();
extern int FUN_0045d510();
extern int FUN_00475b50();
extern void FUN_00519a30();

void FUN_00456fc0(int param_1,char param_2,undefined4 param_3,int *param_4)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  undefined4 uVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  undefined4 *puVar13;
  int local_14;
  int local_c;
  int local_4;
  
  iVar12 = param_1 * 0x108;
  uVar3 = (uint)*(byte *)((int)&DAT_00799a98 + iVar12 + 1);
  puVar13 = &DAT_0096e644;
  for (iVar8 = 0x1b; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar13 = 0;
    puVar13 = puVar13 + 1;
  }
  puVar13 = &DAT_0089b744;
  for (iVar8 = 0x1a5e0; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar13 = 0;
    puVar13 = puVar13 + 1;
  }
  puVar13 = &DAT_00904ec4;
  for (iVar8 = 0x1a5e0; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar13 = 0;
    puVar13 = puVar13 + 1;
  }
  puVar13 = &DAT_0089b6d8;
  for (iVar8 = 0x1b; iVar8 != 0; iVar8 = iVar8 + -1) {
    *puVar13 = 0;
    puVar13 = puVar13 + 1;
  }
  local_4 = 1;
  if (0 < DAT_0078d788) {
    do {
      if ((((&DAT_007900c0)[local_4 * 0xc4] != '\0') &&
          (iVar8 = (int)*(short *)(&DAT_007900bc + local_4 * 0x31), iVar8 != 0)) &&
         (iVar10 = iVar8 * 0x108, (&DAT_007999d8)[iVar10] == '\x01')) {
        sVar1 = (&DAT_007999e8)[iVar8 * 0x84];
        if ((sVar1 == 0x47) || (sVar1 == 0xbf)) {
          if (((DAT_00784bbc == 0) &&
              (((&DAT_00799a1c)[iVar10] != '\0' && (0 < (short)(&DAT_007999f8)[iVar8 * 0x84])))) &&
             ((&DAT_007999ec)[iVar10] == (&DAT_007999ec)[iVar12])) {
            iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar10 + 2);
            iVar9 = (int)*(short *)((int)&DAT_007999dc + iVar12 + 2);
            iVar10 = (int)*(short *)(&DAT_007999e0 + iVar10);
            _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar12);
            if (iVar4 < iVar9) {
              DAT_00d3b4fc = iVar9 - iVar4;
            }
            else if (iVar9 < iVar4) {
              DAT_00d3b4fc = iVar4 - iVar9;
            }
            else {
              DAT_00d3b4fc = 0;
            }
            if (iVar10 < _DAT_00d3b4f8) {
              _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar10;
            }
            else if (_DAT_00d3b4f8 < iVar10) {
              _DAT_00d3b4f8 = iVar10 - _DAT_00d3b4f8;
            }
            else {
              _DAT_00d3b4f8 = 0;
            }
            iVar10 = _DAT_00d3b4f8;
            if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
              iVar10 = DAT_00d3b4fc;
            }
            if (iVar10 < 0x28) {
              iVar10 = 0;
              while (iVar10 = iVar10 + 1, iVar10 < 9) {
                while (iVar10 < 1) {
                  iVar10 = iVar10 + 1;
                  if (8 < iVar10) goto LAB_004575e7;
                }
                switch((int)(iVar10)) {
                case 1: ;
                  iVar4 = 0;
                  break;
                case 2: ;
                  iVar4 = 1;
                  break;
                case 3: ;
                  iVar4 = 2;
                  break;
                case 4: ;
                  iVar4 = 3;
                  break;
                case 5: ;
                  iVar4 = 4;
                  break;
                case 6: ;
                  iVar4 = 5;
                  break;
                case 7: ;
                  iVar4 = 6;
                  break;
                case 8: ;
                  iVar4 = 7;
                  break;
default: ;
                  iVar4 = -1;
                  break;
                case 0xd: ;
                  iVar4 = 9;
                  break;
                case 0xf: ;
                  iVar4 = 8;
                  break;
                case 0x11: ;
                  iVar4 = 10;
                  break;
                case 0x13: ;
                  iVar4 = 0xb;
                }
                if ((((*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + iVar10) * 4) == 0) &&
                     ((&DAT_00799a2e)[iVar10 + iVar8 * 0x84] != 0)) &&
                    (*(char *)((int)&DAT_007900c2 + uVar3 * 0xc4 + iVar10) != '\x01')) &&
                   ((short)(&DAT_00799a2e)[iVar4 + param_1 * 0x84] < 20000)) {
                  iVar9 = (&DAT_0096e644)[iVar4];
                  sVar1 = (&DAT_00799a00)[iVar8 * 0x84];
                  iVar5 = iVar4 * 4000 + iVar9;
                  (&DAT_00904ec4)[iVar5] = iVar8;
                  iVar11 = DAT_00bca2d8 + (short)(&DAT_007999fe)[iVar8 * 0x84];
                  (&DAT_0096e644)[iVar4] = iVar9 + 1;
                  (&DAT_0089b744)[iVar5] = iVar11 + sVar1 * 0xe4;
                }
              }
            }
          }
        }
        else if (((sVar1 == 0x48) || (sVar1 == 0xc1)) &&
                (((&DAT_00799a1c)[iVar10] != '\0' && (0 < (short)(&DAT_007999f8)[iVar8 * 0x84])))) {
          iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar10 + 2);
          iVar9 = (int)*(short *)((int)&DAT_007999dc + iVar12 + 2);
          iVar10 = (int)*(short *)(&DAT_007999e0 + iVar10);
          _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar12);
          if (iVar4 < iVar9) {
            DAT_00d3b4fc = iVar9 - iVar4;
          }
          else if (iVar9 < iVar4) {
            DAT_00d3b4fc = iVar4 - iVar9;
          }
          else {
            DAT_00d3b4fc = 0;
          }
          if (iVar10 < _DAT_00d3b4f8) {
            _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar10;
          }
          else if (_DAT_00d3b4f8 < iVar10) {
            _DAT_00d3b4f8 = iVar10 - _DAT_00d3b4f8;
          }
          else {
            _DAT_00d3b4f8 = 0;
          }
          iVar10 = _DAT_00d3b4f8;
          if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
            iVar10 = DAT_00d3b4fc;
          }
          if (iVar10 < 0x28) {
            iVar10 = 0;
            while (iVar10 = iVar10 + 1, iVar10 < 9) {
              while (iVar10 < 1) {
                iVar10 = iVar10 + 1;
                if (8 < iVar10) goto LAB_00457283;
              }
              switch((int)(iVar10)) {
              case 1: ;
                iVar4 = 0;
                break;
              case 2: ;
                iVar4 = 1;
                break;
              case 3: ;
                iVar4 = 2;
                break;
              case 4: ;
                iVar4 = 3;
                break;
              case 5: ;
                iVar4 = 4;
                break;
              case 6: ;
                iVar4 = 5;
                break;
              case 7: ;
                iVar4 = 6;
                break;
              case 8: ;
                iVar4 = 7;
                break;
default: ;
                iVar4 = -1;
                break;
              case 0xd: ;
                iVar4 = 9;
                break;
              case 0xf: ;
                iVar4 = 8;
                break;
              case 0x11: ;
                iVar4 = 10;
                break;
              case 0x13: ;
                iVar4 = 0xb;
              }
              if (((*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + iVar10) * 4) == 0) &&
                  (iVar9 = FUN_0045d510(iVar8,iVar10), 0 < iVar9)) &&
                 (((*(char *)((int)&DAT_007900c2 + uVar3 * 0xc4 + iVar10) != '\x01' &&
                   ((short)(&DAT_00799a2e)[iVar4 + param_1 * 0x84] < 20000)) &&
                  ((*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + iVar10) * 2) == 1 ||
                   (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + iVar10) * 2) == 3)))))) {
                iVar9 = (&DAT_0096e644)[iVar4];
                sVar1 = (&DAT_00799a00)[iVar8 * 0x84];
                iVar5 = iVar4 * 4000 + iVar9;
                sVar2 = (&DAT_007999fe)[iVar8 * 0x84];
                (&DAT_00904ec4)[iVar5] = iVar8;
                (&DAT_0089b744)[iVar5] = DAT_00bca2d8 + sVar2 + sVar1 * 0xe4;
                (&DAT_0096e644)[iVar4] = iVar9 + 1;
              }
            }
LAB_00457283: ;
            local_14 = 0;
LAB_0045728b: ;
            switch((int)(local_14)) {
            case 0: ;
              iVar8 = 9;
              break;
            case 1: ;
              iVar8 = 0xb;
              break;
            case 2: ;
              iVar8 = 10;
              break;
            case 3: ;
              iVar8 = 8;
              break;
default: ;
              goto switchD_00457294_default;
            }
                    /* WARNING: Could not recover jumptable at 0x004572a0. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&PTR_LAB_00457818)[iVar8])();
            return;
          }
        }
      }
LAB_004575e7: ;
      local_4 = local_4 + 1;
    } while (local_4 <= DAT_0078d788);
  }
  iVar8 = 0;
  do {
    iVar12 = param_1 * 0x84 + iVar8;
    if (*(short *)(&DAT_00799a76 + iVar12 * 2) < 1) {
      switch((int)(iVar8)) {
      case 0: ;
        DAT_0096e668 = 0;
        break;
      case 1: ;
        DAT_0096e670 = 0;
        break;
      case 2: ;
        DAT_0096e66c = 0;
        break;
      case 3: ;
        DAT_0096e664 = 0;
        break;
default: ;
        DAT_0096e640 = 0;
      }
    }
    else {
      *(short *)(&DAT_00799a76 + iVar12 * 2) = *(short *)(&DAT_00799a76 + iVar12 * 2) + -1;
    }
    iVar8 = iVar8 + 1;
  } while (iVar8 < 4);
  if (*(char *)param_4 == '\0') {
    piVar6 = &DAT_0096e644;
    do {
      if (*piVar6 != 0) {
        FUN_00519a30((int)(short)(&DAT_007999fe)[param_1 * 0x84],
                     (int)(short)(&DAT_00799a00)[param_1 * 0x84]);
        *(char *)param_4 = '\x01';
        break;
      }
      piVar6 = piVar6 + 1;
    } while ((int)piVar6 < 0x96e6b0);
  }
  iVar8 = 0;
  param_4 = &DAT_0089b744;
  local_c = 0;
  do {
    iVar12 = *(int *)((int)&DAT_0096e644 + iVar8);
    if (iVar12 != 0) {
      iVar10 = 0;
      iVar9 = 0;
      iVar4 = 9999;
      piVar6 = param_4;
      if (0 < iVar12) {
        do {
          iVar5 = (&DAT_00eed760)[*piVar6];
          if ((0 < iVar5) && (iVar5 < iVar4)) {
            iVar10 = iVar9 + 1;
            iVar4 = iVar5;
          }
          iVar9 = iVar9 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar9 < iVar12);
        if (iVar10 != 0) {
          uVar7 = FUN_00475b50(*(undefined4 *)(&DAT_00904ec0 + (local_c + iVar10) * 4));
          *(undefined4 *)((int)&DAT_0089b6d8 + iVar8) = uVar7;
          goto LAB_0045772a;
        }
      }
      *(undefined4 *)((int)&DAT_0096e644 + iVar8) = 0;
    }
LAB_0045772a: ;
    param_4 = param_4 + 4000;
    local_c = local_c + 4000;
    iVar8 = iVar8 + 4;
    if (0x904ec3 < (int)param_4) {
      if (param_2 == '\0') {
        FUN_00455e00(param_1,&DAT_0096e644,param_3);
        FUN_004561e0(param_1,&DAT_0096e644,&DAT_0089b6d8);
        return;
      }
      FUN_00456070(param_1,&DAT_0096e644,param_3);
      FUN_004565f0(param_1,&DAT_0096e644,&DAT_0089b6d8);
      return;
    }
  } while( true );
switchD_00457294_default: ;
  if ((((*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + -1) * 4) == 0) &&
       (iVar10 = FUN_0045d510(iVar8,0xffffffff), 0 < iVar10)) &&
      ((&DAT_007900c1)[uVar3 * 0xc4] != '\x01')) &&
     ((short)(&DAT_00799a2e)[param_1 * 0x84 + -1] < 20000)) {
    sVar1 = (&DAT_00799a00)[iVar8 * 0x84];
    iVar10 = DAT_0096e640 + -4000;
    sVar2 = (&DAT_007999fe)[iVar8 * 0x84];
    (&DAT_00904ec4)[iVar10] = iVar8;
    DAT_0096e640 = DAT_0096e640 + 1;
    (&DAT_0089b744)[iVar10] = DAT_00bca2d8 + sVar2 + sVar1 * 0xe4;
  }
  local_14 = local_14 + 1;
  if (3 < local_14) goto LAB_004575e7;
  goto LAB_0045728b;
}
