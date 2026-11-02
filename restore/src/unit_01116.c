#include "restore_types.h"
#define DAT_005cc8c4 (*(undefined4 *)0x005cc8c4u)
#define DAT_00af598c (*(undefined4 *)0x00af598cu)
#define DAT_00b285cc (*(undefined4 *)0x00b285ccu)
#define DAT_00b4e6fc (*(undefined4 *)0x00b4e6fcu)
#define DAT_00b67d1c (*(undefined4 *)0x00b67d1cu)
#define DAT_00b7482c (*(undefined4 *)0x00b7482cu)
#define DAT_00bc9768 (*(undefined *)0x00bc9768u)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00e94e04 (*(undefined *)0x00e94e04u)
#define _DAT_00bc9734 (*(undefined4 *)0x00bc9734u)
extern void FUN_004746d0();

void FUN_0047bf50(undefined2 param_1,int param_2,int param_3,int param_4,int param_5,
                 undefined4 param_6,uint param_7)

{
  int iVar1;
  int iVar2;
  byte bVar3;
  int iVar4;
  byte *pbVar5;
  int iVar6;
  int local_c;
  int local_8;
  byte *local_4;
  
  iVar1 = param_4;
  if (DAT_00e94e04 == 0) {
    local_c = param_5 + -1;
    param_4 = 0;
  }
  else if (DAT_00e94e04 == 2) {
    param_4 = 0;
    local_c = 0;
  }
  else if (DAT_00e94e04 == 4) {
    param_4 = param_4 + -1;
    local_c = 0;
  }
  else {
    param_4 = param_4 + -1;
    local_c = param_5 + -1;
  }
  if (param_2 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (param_3 < 0) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (DAT_00bca2d0 <= iVar1 + -1 + param_2) {
    _DAT_00bc9734 = 1;
    return;
  }
  if (DAT_00bca2d4 <= param_3 + -1 + param_5) {
    _DAT_00bc9734 = 1;
    return;
  }
  local_8 = 0;
  iVar4 = DAT_00bca2d8 + param_3 * 0xe4 + param_2;
  DAT_00bc9768 = iVar4;
  if (0 < param_5) {
    local_4 = &DAT_005cc8c4;
    do {
      iVar6 = 0;
      pbVar5 = local_4;
      if (0 < iVar1) {
        do {
          iVar2 = *(int *)(pbVar5 + -4) + iVar4;
          (&DAT_00af598c)[iVar2] = (&DAT_00af598c)[iVar2] & 0x3052e80 | param_7;
          *(undefined2 *)(&DAT_00b4e6fc + iVar2 * 2) = param_1;
          bVar3 = (&DAT_00b285cc)[iVar2] & 0xe0;
          (&DAT_00b285cc)[iVar2] = bVar3;
          if (iVar1 == 2) {
            bVar3 = bVar3 | 1;
LAB_0047c0c3: ;
            (&DAT_00b285cc)[iVar2] = bVar3;
          }
          else {
            if (iVar1 == 3) {
              bVar3 = bVar3 | 2;
              goto LAB_0047c0c3;
            }
            if (iVar1 == 4) {
              bVar3 = bVar3 | 3;
              goto LAB_0047c0c3;
            }
            if (iVar1 == 5) {
              bVar3 = bVar3 | 4;
              goto LAB_0047c0c3;
            }
            if (iVar1 == 6) {
              bVar3 = bVar3 | 5;
              goto LAB_0047c0c3;
            }
          }
          (&DAT_00b7482c)[iVar2] = param_6;
          bVar3 = *pbVar5;
          (&DAT_00b67d1c)[iVar2] = bVar3;
          if ((iVar6 == param_4) && (local_8 == local_c)) {
            (&DAT_00b67d1c)[iVar2] = bVar3 | 0x40;
          }
          iVar6 = iVar6 + 1;
          pbVar5 = pbVar5 + 8;
        } while (iVar6 < iVar1);
      }
      local_8 = local_8 + 1;
      local_4 = local_4 + 0x30;
    } while (local_8 < param_5);
  }
  FUN_004746d0(param_2,param_3,iVar1);
  return;
}
