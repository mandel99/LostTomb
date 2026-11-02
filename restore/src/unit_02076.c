#include "restore_types.h"
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd600 (*(undefined4 *)0x00bcd600u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)

int FUN_004eb800(int param_1,int param_2,int param_3,uint *param_4,uint param_5,int *param_6,
                int *param_7,int param_8,int param_9)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  int local_8;
  
  iVar3 = param_1;
  local_8 = 0;
  iVar2 = DAT_00bca2d8 + param_2 * 0xe4 + param_1;
  param_1 = 1;
  *param_6 = iVar3;
  *param_7 = param_2;
  do {
    param_2 = 0;
    DAT_00bc9768 = iVar2;
    if (0 < param_1) {
      do {
        if ((((-1 < *param_6) && (-1 < *param_7)) && (*param_6 < DAT_00bca2d0)) &&
           (*param_7 < DAT_00bca2d4)) {
          iVar3 = 0;
          if (0 < param_3) {
            puVar4 = param_4;
            do {
              if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
                iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
                do {
                  if (((uint)(byte)(&DAT_00bcd602)[iVar1 * 0x184] == *puVar4) && (param_9 != iVar1))
                  {
                    return iVar1;
                  }
                  iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar1 * 0x184);
                } while (iVar1 != 0);
              }
              iVar3 = iVar3 + 1;
              puVar4 = puVar4 + 1;
            } while (iVar3 < param_3);
          }
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0) &&
             ((param_5 & (&DAT_00af598c)[DAT_00bc9768]) != 0)) goto LAB_004ebc80;
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
           (*param_7 < DAT_00bca2d4)) {
          iVar3 = 0;
          if (0 < param_3) {
            puVar4 = param_4;
            do {
              if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
                iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
                do {
                  if (((uint)(byte)(&DAT_00bcd602)[iVar1 * 0x184] == *puVar4) && (param_9 != iVar1))
                  {
                    return iVar1;
                  }
                  iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar1 * 0x184);
                } while (iVar1 != 0);
              }
              iVar3 = iVar3 + 1;
              puVar4 = puVar4 + 1;
            } while (iVar3 < param_3);
          }
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0) &&
             ((param_5 & (&DAT_00af598c)[DAT_00bc9768]) != 0)) goto LAB_004ebc80;
        }
        DAT_00bc9768 = DAT_00bc9768 + 0xe4;
        param_2 = param_2 + 1;
        *param_7 = *param_7 + 1;
      } while (param_2 < param_1);
    }
    param_2 = 0;
    DAT_00bc9768 = DAT_00bc9768 + -0xe4;
    *param_7 = *param_7 + -1;
    if (0 < param_1) {
      do {
        if ((((-1 < *param_6) && (-1 < *param_7)) && (*param_6 < DAT_00bca2d0)) &&
           (*param_7 < DAT_00bca2d4)) {
          iVar3 = 0;
          if (0 < param_3) {
            puVar4 = param_4;
            do {
              if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
                iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
                do {
                  if (((uint)(byte)(&DAT_00bcd602)[iVar1 * 0x184] == *puVar4) && (param_9 != iVar1))
                  {
                    return iVar1;
                  }
                  iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar1 * 0x184);
                } while (iVar1 != 0);
              }
              iVar3 = iVar3 + 1;
              puVar4 = puVar4 + 1;
            } while (iVar3 < param_3);
          }
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0) &&
             ((param_5 & (&DAT_00af598c)[DAT_00bc9768]) != 0)) goto LAB_004ebc80;
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
           (*param_7 < DAT_00bca2d4)) {
          iVar3 = 0;
          if (0 < param_3) {
            puVar4 = param_4;
            do {
              if (*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2) != 0) {
                iVar1 = (int)*(short *)(&DAT_00b350dc + DAT_00bc9768 * 2);
                do {
                  if (((uint)(byte)(&DAT_00bcd602)[iVar1 * 0x184] == *puVar4) && (param_9 != iVar1))
                  {
                    return iVar1;
                  }
                  iVar1 = (int)*(short *)(&DAT_00bcd600 + iVar1 * 0x184);
                } while (iVar1 != 0);
              }
              iVar3 = iVar3 + 1;
              puVar4 = puVar4 + 1;
            } while (iVar3 < param_3);
          }
          if ((((&DAT_00af598c)[DAT_00bc9768] & 0x80000) == 0) &&
             ((param_5 & (&DAT_00af598c)[DAT_00bc9768]) != 0)) {
LAB_004ebc80: ;
            return param_3 + 1;
          }
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
    iVar2 = iVar2 + -0xe5;
    *param_6 = *param_6 + -1;
    *param_7 = *param_7 + -1;
    param_8 = param_8 + -1;
    if (0xe3 < local_8) {
      return 0;
    }
  } while( true );
}
