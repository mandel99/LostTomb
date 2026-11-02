#include "restore_types.h"
#define DAT_00799a2e (*(undefined4 *)0x00799a2eu)
#define DAT_00f53dd8 (*(undefined *)0x00f53dd8u)
#define DAT_00f53ddc (*(undefined4 *)0x00f53ddcu)
extern uint FUN_00455ca0();

void FUN_00455e00(int param_1,int param_2,int *param_3)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int *piVar5;
  int local_4;
  
  iVar3 = 0;
  *param_3 = -1;
  local_4 = 0;
  param_3[1] = -1;
  param_3[2] = -1;
  param_3[3] = -1;
  if (0 < DAT_00f53dd8) {
    piVar4 = &DAT_00f53ddc;
    piVar5 = param_3;
    do {
      if (*piVar4 != 0) {
        switch(*piVar4) {
        case 1: ;
          iVar2 = 0;
          break;
        case 2: ;
          iVar2 = 1;
          break;
        case 3: ;
          iVar2 = 2;
          break;
        case 4: ;
          iVar2 = 3;
          break;
        case 5: ;
          iVar2 = 4;
          break;
        case 6: ;
          iVar2 = 5;
          break;
        case 7: ;
          iVar2 = 6;
          break;
        case 8: ;
          iVar2 = 7;
          break;
default: ;
          iVar2 = -1;
          break;
        case 0xd: ;
          iVar2 = 9;
          break;
        case 0xf: ;
          iVar2 = 8;
          break;
        case 0x11: ;
          iVar2 = 10;
          break;
        case 0x13: ;
          iVar2 = 0xb;
        }
        if (((&DAT_00799a2e)[iVar2 + param_1 * 0x84] == 0) && (*(int *)(param_2 + iVar2 * 4) != 0))
        {
          *piVar5 = iVar2;
          local_4 = local_4 + 1;
          piVar5 = piVar5 + 1;
          if (3 < local_4) {
            return;
          }
        }
      }
      iVar3 = iVar3 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar3 < DAT_00f53dd8);
  }
  iVar3 = 0;
  if (0 < DAT_00f53dd8) {
    piVar4 = &DAT_00f53ddc;
    piVar5 = param_3 + local_4;
    do {
      if (*piVar4 != 0) {
        switch(*piVar4) {
        case 1: ;
          iVar2 = 0;
          break;
        case 2: ;
          iVar2 = 1;
          break;
        case 3: ;
          iVar2 = 2;
          break;
        case 4: ;
          iVar2 = 3;
          break;
        case 5: ;
          iVar2 = 4;
          break;
        case 6: ;
          iVar2 = 5;
          break;
        case 7: ;
          iVar2 = 6;
          break;
        case 8: ;
          iVar2 = 7;
          break;
default: ;
          iVar2 = -1;
          break;
        case 0xd: ;
          iVar2 = 9;
          break;
        case 0xf: ;
          iVar2 = 8;
          break;
        case 0x11: ;
          iVar2 = 10;
          break;
        case 0x13: ;
          iVar2 = 0xb;
        }
        if (((*(int *)(param_2 + iVar2 * 4) != 0) &&
            ((short)(&DAT_00799a2e)[iVar2 + param_1 * 0x84] < 600)) &&
           (cVar1 = FUN_00455ca0(iVar2,param_1,param_3), cVar1 == '\0')) {
          *piVar5 = iVar2;
          local_4 = local_4 + 1;
          piVar5 = piVar5 + 1;
          if (3 < local_4) {
            return;
          }
        }
      }
      iVar3 = iVar3 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar3 < DAT_00f53dd8);
  }
  return;
}
