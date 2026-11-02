#include "restore_types.h"
#define DAT_005e2ae4 (*(undefined4 *)0x005e2ae4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a30 (*(undefined4 *)0x00799a30u)
#define DAT_00799a3c (*(undefined4 *)0x00799a3cu)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
extern uint FUN_004e0530();

int FUN_004df040(int param_1,int *param_2)

{
  uint uVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint *puVar6;
  int iVar7;
  int local_28;
  int local_24;
  int local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  
  iVar7 = 0;
  param_1 = param_1 - (short)(&DAT_007999ea)[param_1 * 0x84];
  iVar5 = 0;
  local_20 = 0;
  local_1c = 0;
  local_c = 0;
  local_18 = 0;
  uVar1 = *(uint *)(&DAT_00799a3c + param_1 * 0x84);
  local_14 = 0;
  local_10 = 0;
  local_28 = 0;
  local_24 = 1;
  if ((uVar1 & 1) != 0) {
    local_28 = 4;
    if ((uVar1 & 0x10) != 0) {
      local_28 = 3;
    }
    if ((uVar1 & 0x20) != 0) {
      local_28 = local_28 + -1;
    }
    if ((uVar1 & 0x40) != 0) {
      local_28 = local_28 + -1;
    }
    if ((uVar1 & 0x80) != 0) {
      local_28 = local_28 + -1;
    }
  }
  local_8 = 0;
  puVar6 = &DAT_00799a30 + param_1 * 0x42;
  do {
    cVar2 = FUN_004e0530(param_1);
    if (cVar2 != '\0') {
      if (puVar6[1] == 0) {
        return 0;
      }
      iVar3 = puVar6[1] * 0x184;
      cVar2 = (&DAT_00bcd602)[iVar3];
      if (cVar2 == 'O') {
        local_c = local_c + 1;
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
    switch((&DAT_005e2ae4)[*(short *)((int)puVar6 + -0x46) * 0xd] & 0xff) {
    case 0: ;
    case 1: ;
    case 2: ;
    case 3: ;
      iVar4 = 0;
      iVar3 = 0;
      do {
        if ((*puVar6 & 1 << ((byte)iVar3 & 0x1f)) != 0) {
          iVar4 = iVar4 + 1;
        }
        iVar3 = iVar3 + 1;
      } while (iVar3 < 0x10);
      iVar7 = iVar7 + (0x10 - iVar4);
      local_10 = local_10 + (0x10 - iVar4) * 400;
      if (iVar4 != 0) {
        local_24 = 2;
      }
      break;
    case 4: ;
    case 5: ;
    case 6: ;
    case 7: ;
    case 8: ;
    case 9: ;
    case 10: ;
    case 0xc: ;
      if (*puVar6 == 0) {
        iVar5 = iVar5 + 1;
      }
      else if (*puVar6 != 1) break;
      local_20 = local_20 + 1;
    }
    local_8 = local_8 + 1;
    puVar6 = puVar6 + 0x42;
    param_1 = param_1 + 1;
    if (0x73 < local_8) {
      if ((iVar5 == 0) && (local_24 == 1)) {
        local_24 = 2;
      }
      if (((uVar1 >> 1 & 1) != 0) || ((uVar1 & 1) != 0)) {
        local_24 = 3;
      }
      iVar3 = ((((0x120 - local_20) - iVar7) - iVar5) * 100) / 0x120;
      if (param_2 != (int *)0x0) {
        param_2[1] = iVar5;
        *param_2 = local_24;
        param_2[4] = local_1c;
        param_2[5] = local_18;
        param_2[6] = local_14;
        param_2[7] = local_10;
        param_2[10] = local_c;
        param_2[2] = iVar7;
        param_2[3] = local_20;
        param_2[8] = iVar3;
        param_2[9] = 0x6400;
        param_2[0xb] = local_28;
      }
      return iVar3;
    }
  } while( true );
}
