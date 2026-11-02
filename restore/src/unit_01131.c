#include "restore_types.h"
#define DAT_005cc8c0 (*(undefined *)0x005cc8c0u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b350dc (*(undefined4 *)0x00b350dcu)
#define DAT_00bc9738 (*(undefined *)0x00bc9738u)
#define DAT_00bc973c (*(undefined *)0x00bc973cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
extern void FUN_0047d4d0();

void FUN_0047d360(int param_1,int param_2,int param_3,uint param_4,int *param_5,char param_6)

{
  bool bVar1;
  bool bVar2;
  int iVar3;
  int *piVar4;
  
  DAT_00bc9738 = param_1;
  DAT_00bc973c = param_1;
  if ((param_5 == (int *)0x0) && (DAT_00bca2e0 != 0)) {
    if (DAT_00bca2e0 == 6) {
      param_3 = param_3 + (1 - param_1);
    }
    else if (DAT_00bca2e0 == 4) {
      param_2 = param_2 + (1 - param_1);
      param_3 = param_3 + (1 - param_1);
    }
    else if (DAT_00bca2e0 == 2) {
      param_2 = param_2 + (1 - param_1);
    }
  }
  if ((((2 < param_2) && (2 < param_3)) && (param_1 + -1 + param_2 < DAT_00bca2d0 + -3)) &&
     (param_1 + -1 + param_3 < DAT_00bca2d4 + -3)) {
    bVar2 = false;
    bVar1 = false;
    param_1 = 0;
    DAT_00bc9768 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
    if (0 < DAT_00bc9738) {
      param_5 = &DAT_005cc8c0;
      do {
        iVar3 = 0;
        piVar4 = param_5;
        do {
          if (((&DAT_00af598c)[*piVar4 + DAT_00bc9768] & 0x10000) == 0) {
            if (bVar2) {
              DAT_00bc973c = DAT_00bc9738;
              return;
            }
            bVar1 = true;
          }
          else {
            if (param_6 == '\0') {
              DAT_00bc973c = DAT_00bc9738;
              return;
            }
            if (bVar1) {
              DAT_00bc973c = DAT_00bc9738;
              return;
            }
            bVar2 = true;
          }
          if (((&DAT_00af598c)[*piVar4 + DAT_00bc9768] & param_4 & 0xeefed77f) != 0) {
            DAT_00bc973c = DAT_00bc9738;
            return;
          }
          if (*(short *)(&DAT_00b350dc + (*piVar4 + DAT_00bc9768) * 2) != 0) {
            DAT_00bc973c = DAT_00bc9738;
            return;
          }
          iVar3 = iVar3 + 1;
          piVar4 = piVar4 + 2;
        } while (iVar3 < DAT_00bc9738);
        param_1 = param_1 + 1;
        param_5 = param_5 + 0xc;
      } while (param_1 < DAT_00bc9738);
    }
    FUN_0047d4d0();
  }
  return;
}
