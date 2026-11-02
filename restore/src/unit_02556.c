#include "restore_types.h"
#define DAT_00601de4 (*(undefined4 *)0x00601de4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern void FUN_004204a0();
extern uint FUN_00542110();

int FUN_00542ad0(int param_1,int *param_2)

{
  uint uVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int local_34;
  int local_30;
  int local_2c;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar6 = 0;
  local_28 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  param_1 = 0;
  local_30 = 0;
  local_20 = 0;
  local_2c = 0;
  local_24 = 0;
  uVar1 = *(uint *)(&DAT_00799a3c + local_28 * 0x84);
  local_1c = 0;
  local_18 = 0;
  local_14 = 0;
  local_10 = 0;
  iVar4 = 0;
  local_34 = 1;
  local_c = 0;
  piVar5 = &DAT_00799a30 + local_28 * 0x42;
  do {
    cVar2 = FUN_00542110(local_28);
    if (cVar2 != '\0') {
      if (piVar5[1] == 0) {
        return 0;
      }
      iVar3 = piVar5[1] * 0x184;
      cVar2 = (&DAT_00bcd602)[iVar3];
      if (cVar2 == 'O') {
        local_10 = local_10 + 1;
      }
      else if (cVar2 == 'Q') {
        if ((&DAT_00bcd630)[iVar3] == '\n') {
          local_18 = local_18 + 1;
        }
        local_1c = local_1c + 1;
      }
      else if (cVar2 == 'U') {
        local_14 = local_14 + 1;
      }
    }
    switch((&DAT_00601de4)[*(short *)((int)piVar5 + -0x46) * 0xd]) {
    case 0: ;
      local_30 = local_30 + (0xd - *piVar5);
      switch(*piVar5) {
      case 0: ;
        local_34 = ((uVar1 >> 1 & 1) != 0) + 1;
        break;
      case 1: ;
      case 2: ;
      case 3: ;
        local_34 = 2;
        break;
      case 4: ;
      case 5: ;
      case 6: ;
      case 7: ;
      case 8: ;
      case 9: ;
      case 10: ;
      case 0xb: ;
      case 0xc: ;
        local_34 = 3;
        break;
      case 0xd: ;
        local_34 = 4;
      }
      break;
    case 1: ;
      if (*piVar5 == 0) {
        param_1 = param_1 + 1;
      }
      else if (*piVar5 != 1) break;
      local_20 = local_20 + 1;
      goto LAB_00542c80;
    case 2: ;
    case 3: ;
    case 4: ;
    case 5: ;
    case 6: ;
    case 7: ;
      if (*piVar5 == 0) {
        param_1 = param_1 + 1;
      }
      else if (*piVar5 != 1) break;
      local_2c = local_2c + 1;
LAB_00542c80: ;
      iVar4 = iVar4 + 400;
      break;
    case 8: ;
    case 9: ;
    case 10: ;
    case 0xb: ;
      if (*piVar5 == 0) {
        param_1 = param_1 + 1;
      }
      else if (*piVar5 != 1) break;
      local_24 = local_24 + 1;
      break;
    case 0xc: ;
      switch(*piVar5) {
      case 0: ;
        param_1 = param_1 + 1;
      case 1: ;
        iVar6 = iVar6 + 1;
        iVar4 = iVar4 + 400;
      case 2: ;
        iVar6 = iVar6 + 1;
        iVar4 = iVar4 + 400;
      case 3: ;
        iVar6 = iVar6 + 1;
        goto LAB_00542c80;
      }
    }
    local_c = local_c + 1;
    piVar5 = piVar5 + 0x42;
    local_28 = local_28 + 1;
    if (99 < local_c) {
      local_4 = 0;
      local_8 = 0;
      FUN_004204a0(0x4f,&local_4);
      FUN_004204a0(0x51,&local_8);
      iVar3 = ((((((0xd5 - iVar6) - local_24) - local_2c) - local_30) - param_1) * 100) / 0xd5;
      if (param_2 != (int *)0x0) {
        param_2[1] = param_1;
        *param_2 = local_34;
        param_2[2] = local_30;
        param_2[5] = local_24;
        param_2[3] = local_20;
        param_2[4] = local_2c;
        param_2[7] = local_8 + local_1c;
        param_2[8] = local_18;
        param_2[9] = local_14;
        param_2[6] = iVar6;
        param_2[10] = local_4 + local_10;
        param_2[0xb] = iVar4;
        param_2[0xc] = iVar3;
        param_2[0xd] = 38000;
      }
      return iVar3;
    }
  } while( true );
}
