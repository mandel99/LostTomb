#include "restore_types.h"
#define DAT_00799b96 (*(undefined4 *)0x00799b96u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd606 (*(undefined4 *)0x00bcd606u)
#define DAT_00bcd652 (*(undefined4 *)0x00bcd652u)

undefined4 FUN_00460fb0(undefined4 *param_1)

{
  short sVar1;
  int iVar2;
  short *psVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  iVar5 = 0;
  local_18 = 0;
  local_10 = 0;
  iVar2 = 0;
  local_8 = 0;
  local_4 = 0;
  local_1c = 0;
  local_14 = 0;
  local_c = 0;
  iVar6 = 1;
  psVar3 = &DAT_00799b96;
  do {
    if (((char)psVar3[-0x5b] == '\x01') && (psVar3[-0x3b] != 0)) {
      if (psVar3[-0x53] == 0x4c) {
        sVar1 = *psVar3;
        iVar5 = iVar5 + 1;
        if (0 < sVar1) {
          iVar4 = sVar1 * 0x184;
          if ((&DAT_00bcd606)[iVar4] == '\0') {
            *psVar3 = 0;
          }
          else if (((&DAT_00bcd602)[iVar4] == '\x19') || ((&DAT_00bcd602)[iVar4] == '\0')) {
            if ((short)(&DAT_00bcd652)[sVar1 * 0xc2] == iVar6) {
              local_18 = local_18 + 1;
            }
            else {
              *psVar3 = 0;
            }
          }
          else {
            *psVar3 = 0;
          }
        }
      }
      if (psVar3[-0x53] == 0xb5) {
        sVar1 = *psVar3;
        iVar2 = iVar2 + 1;
        if (0 < sVar1) {
          iVar4 = sVar1 * 0x184;
          if ((&DAT_00bcd606)[iVar4] == '\0') {
            *psVar3 = 0;
          }
          else if (((&DAT_00bcd602)[iVar4] == 'M') || ((&DAT_00bcd602)[iVar4] == '\0')) {
            if ((short)(&DAT_00bcd652)[sVar1 * 0xc2] == iVar6) {
              local_10 = local_10 + 1;
            }
            else {
              *psVar3 = 0;
            }
          }
          else {
            *psVar3 = 0;
          }
        }
      }
      if (psVar3[-0x53] == 0xb6) {
        sVar1 = *psVar3;
        local_4 = local_4 + 1;
        if (0 < sVar1) {
          iVar4 = sVar1 * 0x184;
          if ((&DAT_00bcd606)[iVar4] == '\0') {
            *psVar3 = 0;
          }
          else if (((&DAT_00bcd602)[iVar4] == 'N') || ((&DAT_00bcd602)[iVar4] == '\0')) {
            if ((short)(&DAT_00bcd652)[sVar1 * 0xc2] == iVar6) {
              local_8 = local_8 + 1;
            }
            else {
              *psVar3 = 0;
            }
          }
          else {
            *psVar3 = 0;
          }
        }
      }
      if ((psVar3[-0x53] == 0x4a) && (sVar1 = psVar3[-1], sVar1 != 0)) {
        if (sVar1 == 0x1019) {
          local_1c = local_1c + 1;
        }
        if (sVar1 == 0x104d) {
          local_14 = local_14 + 1;
        }
        if (sVar1 == 0x104e) {
          local_c = local_c + 1;
        }
      }
    }
    psVar3 = psVar3 + 0x84;
    iVar6 = iVar6 + 1;
  } while ((int)psVar3 < 0x89b78e);
  if (local_1c + local_18 < iVar5) {
    *param_1 = 0x19;
    return 1;
  }
  if (local_14 + local_10 < iVar2) {
    *param_1 = 0x4d;
    return 1;
  }
  if (local_4 <= local_c + local_8) {
    return 0;
  }
  *param_1 = 0x4e;
  return 1;
}
