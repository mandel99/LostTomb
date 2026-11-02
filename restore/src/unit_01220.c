#include "restore_types.h"
#define DAT_0070cdf4 (*(undefined4 *)0x0070cdf4u)
#define DAT_0070cdf8 (*(undefined4 *)0x0070cdf8u)
#define DAT_0070cdfc (*(undefined4 *)0x0070cdfcu)
#define DAT_0070ce00 (*(undefined4 *)0x0070ce00u)
#define DAT_0070ce04 (*(undefined4 *)0x0070ce04u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999f4 (*(undefined4 *)0x007999f4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00799a3e (*(undefined4 *)0x00799a3eu)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a42 (*(undefined4 *)0x00799a42u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00799a76 (*(undefined4 *)0x00799a76u)
#define DAT_00799a78 (*(undefined4 *)0x00799a78u)
#define DAT_00799a7a (*(undefined4 *)0x00799a7au)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)
#define _DAT_00d3b4e4 (*(undefined4 *)0x00d3b4e4u)

int FUN_00483910(int param_1,int param_2,int param_3)

{
  int iVar1;
  int iVar2;
  short sVar3;
  short sVar4;
  short sVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  int *local_28;
  short *local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  
  local_20 = param_2 + 2;
  iVar12 = param_3 + 2;
  param_2 = param_2 + -2;
  _DAT_00d3b4e0 = param_3 + -2;
  local_1c = 0;
  if (param_2 < 0) {
    param_2 = 0;
  }
  if (_DAT_00d3b4e0 < 0) {
    _DAT_00d3b4e0 = 0;
  }
  if (DAT_00bca2d0 <= local_20) {
    local_20 = DAT_00bca2d0 + -1;
  }
  if (DAT_00bca2d4 <= iVar12) {
    iVar12 = DAT_00bca2d4 + -1;
  }
  DAT_00bc9768 = DAT_00bca2d8 + _DAT_00d3b4e0 * 0xe4 + param_2;
  if (iVar12 < _DAT_00d3b4e0) {
    return 0;
  }
  local_10 = (iVar12 - _DAT_00d3b4e0) + 1;
  _DAT_00d3b4e0 = local_10 + _DAT_00d3b4e0;
  do {
    _DAT_00d3b4e4 = param_2;
    if (param_2 <= local_20) {
      local_24 = (short *)(&DAT_00b4e6fc + DAT_00bc9768 * 2);
      local_14 = (local_20 - param_2) + 1;
      DAT_00bc9768 = DAT_00bc9768 + local_14;
      _DAT_00d3b4e4 = local_14 + param_2;
      do {
        iVar12 = (int)*local_24;
        if (((0 < iVar12) &&
            (iVar6 = iVar12 * 0x108, *(char *)((int)&DAT_007999dc + iVar6 + 1) != '\0')) &&
           (sVar3 = (&DAT_007999f4)[iVar12 * 0x84], 0 < sVar3)) {
          iVar7 = (int)(short)(&DAT_007999ea)[iVar12 * 0x84];
          local_1c = local_1c + 1;
          if (iVar7 < 0x13) {
            iVar7 = iVar7 + 1;
          }
          iVar7 = iVar7 * 0x58;
          iVar11 = sVar3 * 2;
          param_3 = *(int *)(&DAT_0070cdf4 + iVar7);
          if ((0 < param_3) && (param_3 < 4)) {
            param_3 = param_3 + 1;
          }
          if (0 < DAT_00f53dd8) {
            local_28 = &DAT_00f53ddc;
            local_18 = DAT_00f53dd8;
            do {
              if (*local_28 != 0) {
                switch(*local_28) {
                case 1: ;
                  iVar10 = 0;
                  break;
                case 2: ;
                  iVar10 = 1;
                  break;
                case 3: ;
                  iVar10 = 2;
                  break;
                case 4: ;
                  iVar10 = 3;
                  break;
                case 5: ;
                  iVar10 = 4;
                  break;
                case 6: ;
                  iVar10 = 5;
                  break;
                case 7: ;
                  iVar10 = 6;
                  break;
                case 8: ;
                  iVar10 = 7;
                  break;
default: ;
                  iVar10 = -1;
                  break;
                case 0xd: ;
                  iVar10 = 9;
                  break;
                case 0xf: ;
                  iVar10 = 8;
                  break;
                case 0x11: ;
                  iVar10 = 10;
                  break;
                case 0x13: ;
                  iVar10 = 0xb;
                }
                iVar1 = iVar10 + param_1 * 0x84;
                sVar4 = (&DAT_00799a2e)[iVar1];
                if ((0 < sVar4) && (param_3 != 0)) {
                  iVar2 = iVar10 + iVar12 * 0x84;
                  sVar5 = (&DAT_00799a2c)[iVar2];
                  iVar8 = sVar3 * 6 - (int)sVar5;
                  if (iVar11 < iVar8) {
                    iVar8 = iVar11;
                  }
                  if (0 < iVar8) {
                    iVar9 = iVar8 >> 1;
                    if (sVar4 < iVar8) {
                      switch((int)(iVar10)) {
                      case 1: ;
                      case 2: ;
                      case 3: ;
                      case 4: ;
                      case 5: ;
                      case 7: ;
                        iVar8 = iVar9;
                        if (iVar9 <= sVar4) goto switchD_00483b56_caseD_0;
                      case 0: ;
                      case 6: ;
                        (&DAT_00799a2c)[iVar2] = sVar5 + sVar4;
                        (&DAT_00799a2e)[iVar1] = 0;
                      }
                    }
                    else {
                      switch((int)(iVar10)) {
                      case 1: ;
                      case 2: ;
                      case 3: ;
                      case 4: ;
                      case 5: ;
                      case 7: ;
                        iVar8 = iVar9;
                      case 0: ;
                      case 6: ;
switchD_00483b56_caseD_0: ;
                        (&DAT_00799a2c)[iVar2] = sVar5 + (short)iVar8;
                        (&DAT_00799a2e)[iVar1] = (&DAT_00799a2e)[iVar1] - (short)iVar8;
                      }
                    }
                    param_3 = param_3 + -1;
                  }
                }
              }
              local_28 = local_28 + 1;
              local_18 = local_18 + -1;
            } while (local_18 != 0);
          }
          iVar11 = *(int *)(&DAT_0070ce04 + iVar7);
          if (1 < (byte)(&DAT_007999db)[iVar6]) {
            iVar11 = iVar11 << 2;
          }
          if (iVar11 != 0) {
            *(undefined2 *)((int)&DAT_00799a7a + param_1 * 0x108 + 2) = 10;
            sVar3 = (&DAT_00799a3e)[param_1 * 0x84];
            if (0 < sVar3) {
              sVar4 = (&DAT_00799a3c)[iVar12 * 0x84];
              iVar11 = iVar11 * 2 - (int)sVar4;
              if (0 < iVar11) {
                if (sVar3 < iVar11) {
                  (&DAT_00799a3c)[iVar12 * 0x84] = sVar4 + sVar3;
                  (&DAT_00799a3e)[param_1 * 0x84] = 0;
                }
                else {
                  (&DAT_00799a3c)[iVar12 * 0x84] = sVar4 + (short)iVar11;
                  (&DAT_00799a3e)[param_1 * 0x84] = (&DAT_00799a3e)[param_1 * 0x84] - (short)iVar11;
                }
              }
            }
          }
          iVar11 = *(int *)(&DAT_0070cdf8 + iVar7);
          if (1 < (byte)(&DAT_007999db)[iVar6]) {
            iVar11 = iVar11 << 2;
          }
          if (iVar11 != 0) {
            *(undefined2 *)(&DAT_00799a76 + param_1 * 0x108) = 10;
            sVar3 = (&DAT_00799a40)[param_1 * 0x84];
            if (0 < sVar3) {
              sVar4 = (&DAT_00799a3e)[iVar12 * 0x84];
              iVar11 = iVar11 * 2 - (int)sVar4;
              if (0 < iVar11) {
                if (sVar3 < iVar11) {
                  (&DAT_00799a3e)[iVar12 * 0x84] = sVar4 + sVar3;
                  (&DAT_00799a40)[param_1 * 0x84] = 0;
                }
                else {
                  (&DAT_00799a3e)[iVar12 * 0x84] = sVar4 + (short)iVar11;
                  (&DAT_00799a40)[param_1 * 0x84] = (&DAT_00799a40)[param_1 * 0x84] - (short)iVar11;
                }
              }
            }
          }
          iVar11 = *(int *)(&DAT_0070cdfc + iVar7);
          if (1 < (byte)(&DAT_007999db)[iVar6]) {
            iVar11 = iVar11 << 2;
          }
          if (iVar11 != 0) {
            *(undefined2 *)(&DAT_00799a7a + param_1 * 0x42) = 10;
            sVar3 = (&DAT_00799a42)[param_1 * 0x84];
            if (0 < sVar3) {
              sVar4 = (&DAT_00799a40)[iVar12 * 0x84];
              iVar11 = iVar11 * 2 - (int)sVar4;
              if (0 < iVar11) {
                if (sVar3 < iVar11) {
                  (&DAT_00799a40)[iVar12 * 0x84] = sVar4 + sVar3;
                  (&DAT_00799a42)[param_1 * 0x84] = 0;
                }
                else {
                  (&DAT_00799a40)[iVar12 * 0x84] = sVar4 + (short)iVar11;
                  (&DAT_00799a42)[param_1 * 0x84] = (&DAT_00799a42)[param_1 * 0x84] - (short)iVar11;
                }
              }
            }
          }
          iVar7 = *(int *)(&DAT_0070ce00 + iVar7);
          if (1 < (byte)(&DAT_007999db)[iVar6]) {
            iVar7 = iVar7 << 2;
          }
          if (iVar7 != 0) {
            *(undefined2 *)(&DAT_00799a78 + param_1 * 0x108) = 10;
            sVar3 = (&DAT_00799a44)[param_1 * 0x84];
            if (0 < sVar3) {
              sVar4 = (&DAT_00799a42)[iVar12 * 0x84];
              iVar6 = iVar7 * 2 - (int)sVar4;
              if (0 < iVar6) {
                if (sVar3 < iVar6) {
                  (&DAT_00799a42)[iVar12 * 0x84] = sVar4 + sVar3;
                  (&DAT_00799a44)[param_1 * 0x84] = 0;
                }
                else {
                  (&DAT_00799a42)[iVar12 * 0x84] = sVar4 + (short)iVar6;
                  (&DAT_00799a44)[param_1 * 0x84] = (&DAT_00799a44)[param_1 * 0x84] - (short)iVar6;
                }
              }
            }
          }
        }
        local_24 = local_24 + 1;
        local_14 = local_14 + -1;
      } while (local_14 != 0);
    }
    DAT_00bc9768 = DAT_00bc9768 + (param_2 - local_20) + 0xe3;
    local_10 = local_10 + -1;
    if (local_10 == 0) {
      return local_1c;
    }
  } while( true );
}
