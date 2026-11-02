#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define _DAT_00d3b4e0 (*(undefined4 *)0x00d3b4e0u)

int FUN_004ebc90(int param_1,int param_2,int param_3,uint *param_4,uint param_5,int *param_6,
                int *param_7,int param_8,int param_9,uint param_10)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  uint *puVar5;
  int local_8;
  
  local_8 = 0;
  iVar3 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  *param_6 = param_1;
  param_1 = 1;
  *param_7 = param_2;
  do {
    param_2 = 0;
    DAT_00bc9768 = iVar3;
    if (0 < param_1) {
      do {
        if ((((-1 < *param_6) && (-1 < *param_7)) && (*param_6 < DAT_00bca2d0)) &&
           (*param_7 < DAT_00bca2d4)) {
          iVar4 = 0;
          if (0 < param_3) {
            puVar5 = param_4;
            do {
              if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
                iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
                do {
                  iVar2 = iVar1 * 0x184;
                  if ((((uint)(byte)(&DAT_00bcd602)[iVar2] == *puVar5) && (param_9 != iVar1)) &&
                     ((byte)(&DAT_00bcd630)[iVar2] == param_10)) {
                    return iVar1;
                  }
                  iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar2);
                } while (iVar1 != 0);
              }
              iVar4 = iVar4 + 1;
              puVar5 = puVar5 + 1;
            } while (iVar4 < param_3);
          }
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0) &&
             ((param_5 & (&DAT_00af598c)[DAT_00bc9768]) != 0)) {
            return param_3 + 1;
          }
        }
        param_2 = param_2 + 1;
        DAT_00bc9768 = DAT_00bc9768 + 1;
        *param_6 = *param_6 + 1;
      } while (param_2 < param_1);
    }
    param_2 = 0;
    DAT_00bc9768 = DAT_00bc9768 + -1;
    *param_6 = *param_6 + -1;
    if (0 < param_1) {
      do {
        if ((((-1 < *param_6) && (-1 < *param_7)) && (*param_6 < DAT_00bca2d0)) &&
           ((*param_7 < DAT_00bca2d4 && (iVar4 = 0, 0 < param_3)))) {
          puVar5 = param_4;
          do {
            if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
              iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
              do {
                iVar2 = iVar1 * 0x184;
                if ((((uint)(byte)(&DAT_00bcd602)[iVar2] == *puVar5) && (param_9 != iVar1)) &&
                   ((byte)(&DAT_00bcd630)[iVar2] == param_10)) {
                  return iVar1;
                }
                iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar2);
              } while (iVar1 != 0);
            }
            iVar4 = iVar4 + 1;
            puVar5 = puVar5 + 1;
          } while (iVar4 < param_3);
        }
        DAT_00bc9768 = DAT_00bc9768 + 0xe4;
        param_2 = param_2 + 1;
        *param_7 = *param_7 + 1;
      } while (param_2 < param_1);
    }
    DAT_00bc9768 = DAT_00bc9768 + -0xe4;
    _DAT_00d3b4e0 = _DAT_00d3b4e0 + -1;
    param_2 = 0;
    if (0 < param_1) {
      do {
        if ((((-1 < *param_6) && (-1 < *param_7)) && (*param_6 < DAT_00bca2d0)) &&
           ((*param_7 < DAT_00bca2d4 && (iVar4 = 0, 0 < param_3)))) {
          puVar5 = param_4;
          do {
            if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
              iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
              do {
                iVar2 = iVar1 * 0x184;
                if ((((uint)(byte)(&DAT_00bcd602)[iVar2] == *puVar5) && (param_9 != iVar1)) &&
                   ((byte)(&DAT_00bcd630)[iVar2] == param_10)) {
                  return iVar1;
                }
                iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar2);
              } while (iVar1 != 0);
            }
            iVar4 = iVar4 + 1;
            puVar5 = puVar5 + 1;
          } while (iVar4 < param_3);
        }
        param_2 = param_2 + 1;
        DAT_00bc9768 = DAT_00bc9768 + -1;
        *param_6 = *param_6 + -1;
      } while (param_2 < param_1);
    }
    param_2 = 0;
    DAT_00bc9768 = DAT_00bc9768 + 1;
    *param_6 = *param_6 + 1;
    if (0 < param_1) {
      do {
        if ((((-1 < *param_6) && (-1 < *param_7)) && (*param_6 < DAT_00bca2d0)) &&
           ((*param_7 < DAT_00bca2d4 && (iVar4 = 0, 0 < param_3)))) {
          puVar5 = param_4;
          do {
            if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
              iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
              do {
                iVar2 = iVar1 * 0x184;
                if ((((uint)(byte)(&DAT_00bcd602)[iVar2] == *puVar5) && (param_9 != iVar1)) &&
                   ((byte)(&DAT_00bcd630)[iVar2] == param_10)) {
                  return iVar1;
                }
                iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar2);
              } while (iVar1 != 0);
            }
            iVar4 = iVar4 + 1;
            puVar5 = puVar5 + 1;
          } while (iVar4 < param_3);
        }
        DAT_00bc9768 = DAT_00bc9768 + -0xe4;
        param_2 = param_2 + 1;
        *param_7 = *param_7 + -1;
      } while (param_2 < param_1);
    }
    DAT_00bc9768 = DAT_00bc9768 + 0xe4;
    *param_7 = *param_7 + 1;
    if (param_8 == 0) {
      return 0;
    }
    local_8 = local_8 + 1;
    param_1 = param_1 + 2;
    iVar3 = iVar3 + -0xe5;
    *param_6 = *param_6 + -1;
    *param_7 = *param_7 + -1;
    param_8 = param_8 + -1;
  } while (local_8 < 0xe4);
  return 0;
}
