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
#define DAT_0089b6d4 (*(undefined4 *)0x0089b6d4u)
#define DAT_0089b6d8 (*(undefined4 *)0x0089b6d8u)
#define DAT_0089b6f8 (*(undefined4 *)0x0089b6f8u)
#define DAT_0089b6fc (*(undefined4 *)0x0089b6fcu)
#define DAT_0089b700 (*(undefined4 *)0x0089b700u)
#define DAT_0089b704 (*(undefined4 *)0x0089b704u)
#define DAT_0089b744 (*(undefined4 *)0x0089b744u)
#define DAT_00904ec0 (*(undefined4 *)0x00904ec0u)
#define DAT_00904ec4 (*(undefined4 *)0x00904ec4u)
#define DAT_0096e640 (*(undefined4 *)0x0096e640u)
#define DAT_0096e644 (*(undefined4 *)0x0096e644u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)
#define DAT_00d3b4fc (*(undefined *)0x00d3b4fcu)
#define DAT_00ea4096 (*(undefined4 *)0x00ea4096u)
#define DAT_00ea42f8 (*(undefined4 *)0x00ea42f8u)
#define DAT_00eaac28 (*(undefined1 *)0x00eaac28u)
#define PTR_DAT_00456f1c (*(unsigned char * *)0x00456f1cu)
#define _DAT_00d3b4f8 (*(undefined4 *)0x00d3b4f8u)
extern int FUN_0045d510();
extern int FUN_00475b50();
extern int FUN_00519e30();

void FUN_00456760(int param_1,int param_2)

{
  short sVar1;
  short sVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  undefined4 *puVar12;
  int local_10;
  int local_4;
  
  iVar9 = param_2 * 0x108;
  uVar3 = (uint)*(byte *)((int)&DAT_00799a98 + iVar9 + 1);
  puVar12 = &DAT_0096e644;
  for (iVar7 = 0x1b; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar12 = 0;
    puVar12 = puVar12 + 1;
  }
  puVar12 = &DAT_0089b744;
  for (iVar7 = 0x1a5e0; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar12 = 0;
    puVar12 = puVar12 + 1;
  }
  puVar12 = &DAT_00904ec4;
  for (iVar7 = 0x1a5e0; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar12 = 0;
    puVar12 = puVar12 + 1;
  }
  puVar12 = &DAT_0089b6d8;
  for (iVar7 = 0x1b; iVar7 != 0; iVar7 = iVar7 + -1) {
    *puVar12 = 0;
    puVar12 = puVar12 + 1;
  }
  local_4 = 1;
  if (0 < DAT_0078d788) {
    do {
      if ((((&DAT_007900c0)[local_4 * 0xc4] != '\0') &&
          (iVar7 = (int)*(short *)(&DAT_007900bc + local_4 * 0x31), iVar7 != 0)) &&
         (iVar11 = iVar7 * 0x108, (&DAT_007999d8)[iVar11] == '\x01')) {
        sVar1 = (&DAT_007999e8)[iVar7 * 0x84];
        if ((sVar1 == 0x47) || (sVar1 == 0xbf)) {
          if (((DAT_00784bbc == 0) &&
              (((&DAT_00799a1c)[iVar11] != '\0' && (0 < (short)(&DAT_007999f8)[iVar7 * 0x84])))) &&
             ((&DAT_007999ec)[iVar11] == (&DAT_007999ec)[iVar9])) {
            iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar11 + 2);
            _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar9);
            iVar8 = (int)*(short *)((int)&DAT_007999dc + iVar9 + 2);
            iVar11 = (int)*(short *)(&DAT_007999e0 + iVar11);
            if (iVar4 < iVar8) {
              DAT_00d3b4fc = iVar8 - iVar4;
            }
            else if (iVar8 < iVar4) {
              DAT_00d3b4fc = iVar4 - iVar8;
            }
            else {
              DAT_00d3b4fc = 0;
            }
            if (iVar11 < _DAT_00d3b4f8) {
              _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar11;
            }
            else if (_DAT_00d3b4f8 < iVar11) {
              _DAT_00d3b4f8 = iVar11 - _DAT_00d3b4f8;
            }
            else {
              _DAT_00d3b4f8 = 0;
            }
            iVar11 = _DAT_00d3b4f8;
            if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
              iVar11 = DAT_00d3b4fc;
            }
            if (iVar11 < 0x28) {
              iVar11 = 0;
              while (iVar11 = iVar11 + 1, iVar11 < 9) {
                while (iVar11 < 1) {
                  iVar11 = iVar11 + 1;
                  if (8 < iVar11) goto LAB_00456d70;
                }
                switch((int)(iVar11)) {
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
                if ((((&DAT_00799a2e)[iVar11 + iVar7 * 0x84] != 0) &&
                    (*(char *)((int)&DAT_007900c2 + uVar3 * 0xc4 + iVar11) != '\x01')) &&
                   ((short)(&DAT_00799a2e)[iVar4 + param_2 * 0x84] < 20000)) {
                  sVar1 = (&DAT_00799a00)[iVar7 * 0x84];
                  iVar8 = (&DAT_0096e644)[iVar4];
                  sVar2 = (&DAT_007999fe)[iVar7 * 0x84];
                  iVar5 = iVar4 * 4000 + iVar8;
                  (&DAT_00904ec4)[iVar5] = iVar7;
                  iVar10 = DAT_00bca2d8 + sVar2;
                  (&DAT_0096e644)[iVar4] = iVar8 + 1;
                  (&DAT_0089b744)[iVar5] = iVar10 + sVar1 * 0xe4;
                }
              }
            }
          }
        }
        else if (((sVar1 == 0x48) || (sVar1 == 0xc1)) &&
                (((&DAT_00799a1c)[iVar11] != '\0' && (0 < (short)(&DAT_007999f8)[iVar7 * 0x84])))) {
          iVar4 = (int)*(short *)((int)&DAT_007999dc + iVar11 + 2);
          _DAT_00d3b4f8 = (int)*(short *)(&DAT_007999e0 + iVar9);
          iVar8 = (int)*(short *)((int)&DAT_007999dc + iVar9 + 2);
          iVar11 = (int)*(short *)(&DAT_007999e0 + iVar11);
          if (iVar4 < iVar8) {
            DAT_00d3b4fc = iVar8 - iVar4;
          }
          else if (iVar8 < iVar4) {
            DAT_00d3b4fc = iVar4 - iVar8;
          }
          else {
            DAT_00d3b4fc = 0;
          }
          if (iVar11 < _DAT_00d3b4f8) {
            _DAT_00d3b4f8 = _DAT_00d3b4f8 - iVar11;
          }
          else if (_DAT_00d3b4f8 < iVar11) {
            _DAT_00d3b4f8 = iVar11 - _DAT_00d3b4f8;
          }
          else {
            _DAT_00d3b4f8 = 0;
          }
          iVar11 = _DAT_00d3b4f8;
          if (_DAT_00d3b4f8 <= DAT_00d3b4fc) {
            iVar11 = DAT_00d3b4fc;
          }
          if (iVar11 < 0x28) {
            iVar11 = 0;
            while (iVar11 = iVar11 + 1, iVar11 < 9) {
              while (iVar11 < 1) {
                iVar11 = iVar11 + 1;
                if (8 < iVar11) goto LAB_00456a26;
              }
              switch((int)(iVar11)) {
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
              if ((((*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + iVar11) * 4) == 0) &&
                   (iVar8 = FUN_0045d510(iVar7,iVar11), 0 < iVar8)) &&
                  ((*(char *)((int)&DAT_007900c2 + uVar3 * 0xc4 + iVar11) != '\x01' &&
                   ((short)(&DAT_00799a2e)[iVar4 + param_2 * 0x84] < 20000)))) &&
                 ((*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + iVar11) * 2) == 1 ||
                  (*(short *)(&DAT_00ea4096 + (DAT_00eaac28 * 0x24ec + iVar11) * 2) == 3)))) {
                iVar8 = (&DAT_0096e644)[iVar4];
                sVar1 = (&DAT_00799a00)[iVar7 * 0x84];
                iVar5 = iVar4 * 4000 + iVar8;
                (&DAT_00904ec4)[iVar5] = iVar7;
                (&DAT_0089b744)[iVar5] =
                     DAT_00bca2d8 + (short)(&DAT_007999fe)[iVar7 * 0x84] + sVar1 * 0xe4;
                (&DAT_0096e644)[iVar4] = iVar8 + 1;
              }
            }
LAB_00456a26: ;
            local_10 = 0;
LAB_00456a2e: ;
            switch((int)(local_10)) {
            case 0: ;
              iVar7 = 9;
              break;
            case 1: ;
              iVar7 = 0xb;
              break;
            case 2: ;
              iVar7 = 10;
              break;
            case 3: ;
              iVar7 = 8;
              break;
default: ;
              goto switchD_00456a37_default;
            }
                    /* WARNING: Could not recover jumptable at 0x00456a43. Too many branches */
                    /* WARNING: Treating indirect jump as call */
            (*(code *)(&PTR_DAT_00456f1c)[iVar7])();
            return;
          }
        }
      }
LAB_00456d70: ;
      local_4 = local_4 + 1;
    } while (local_4 <= DAT_0078d788);
  }
  iVar7 = 0;
  puVar12 = &DAT_0089b744;
  iVar9 = 0;
  do {
    if ((*(int *)((int)&DAT_0096e644 + iVar9) != 0) &&
       (iVar11 = FUN_00519e30((int)(short)(&DAT_007999fe)[param_2 * 0x84],
                              (int)(short)(&DAT_00799a00)[param_2 * 0x84],puVar12,
                              *(int *)((int)&DAT_0096e644 + iVar9)), iVar11 != 0)) {
      uVar6 = FUN_00475b50(*(undefined4 *)(&DAT_00904ec0 + (iVar11 + iVar7) * 4));
      *(undefined4 *)((int)&DAT_0089b6d8 + iVar9) = uVar6;
    }
    puVar12 = puVar12 + 4000;
    iVar7 = iVar7 + 4000;
    iVar9 = iVar9 + 4;
  } while ((int)puVar12 < 0x904ec4);
  iVar7 = 0;
  sVar1 = (&DAT_00bcd652)[param_1 * 0xc2];
  do {
    iVar9 = sVar1 * 0x84 + iVar7;
    if (*(short *)(&DAT_00799a76 + iVar9 * 2) < 1) {
      switch((int)(iVar7)) {
      case 0: ;
        iVar9 = 9;
        break;
      case 1: ;
        iVar9 = 0xb;
        break;
      case 2: ;
        iVar9 = 10;
        break;
      case 3: ;
        iVar9 = 8;
        break;
default: ;
        iVar9 = -1;
      }
      (&DAT_0096e644)[iVar9] = 0;
      switch((int)(iVar7)) {
      case 0: ;
        DAT_0089b6fc = 0;
        break;
      case 1: ;
        DAT_0089b704 = 0;
        break;
      case 2: ;
        DAT_0089b700 = 0;
        break;
      case 3: ;
        DAT_0089b6f8 = 0;
        break;
default: ;
        DAT_0089b6d4 = 0;
      }
    }
    else {
      *(short *)(&DAT_00799a76 + iVar9 * 2) = *(short *)(&DAT_00799a76 + iVar9 * 2) + -1;
    }
    iVar7 = iVar7 + 1;
  } while (iVar7 < 4);
  return;
switchD_00456a37_default: ;
  if ((((*(int *)(&DAT_00ea42f8 + (DAT_00eaac28 * 0x1276 + -1) * 4) == 0) &&
       (iVar11 = FUN_0045d510(iVar7,0xffffffff), 0 < iVar11)) &&
      ((&DAT_007900c1)[uVar3 * 0xc4] != '\x01')) &&
     ((short)(&DAT_00799a2e)[param_2 * 0x84 + -1] < 20000)) {
    sVar1 = (&DAT_00799a00)[iVar7 * 0x84];
    iVar11 = DAT_0096e640 + -4000;
    sVar2 = (&DAT_007999fe)[iVar7 * 0x84];
    (&DAT_00904ec4)[iVar11] = iVar7;
    DAT_0096e640 = DAT_0096e640 + 1;
    (&DAT_0089b744)[iVar11] = DAT_00bca2d8 + sVar2 + sVar1 * 0xe4;
  }
  local_10 = local_10 + 1;
  if (3 < local_10) goto LAB_00456d70;
  goto LAB_00456a2e;
}
