#include "restore_types.h"
#define DAT_006dea2c (*(char *)0x006dea2cu)
#define DAT_006e91d4 (*(char *)0x006e91d4u)
#define DAT_006f5694 (*(char *)0x006f5694u)
#define DAT_006f936c (*(char *)0x006f936cu)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)

void FUN_00452200(void)

{
  char cVar1;
  short sVar2;
  int iVar3;
  char *pcVar4;
  char local_8 [8];
  
  local_8[4] = 0;
  local_8[5] = 0;
  local_8[6] = 0;
  local_8[7] = 0;
  local_8[0] = '\0';
  local_8[1] = 0;
  local_8[2] = 0;
  local_8[3] = 0;
  if (DAT_006f936c != '\0') {
    local_8[4] = 1;
  }
  if (DAT_006f5694 != '\0') {
    local_8[5] = 1;
  }
  if (DAT_006dea2c != '\0') {
    local_8[6] = 1;
  }
  if (DAT_006e91d4 != '\0') {
    local_8[7] = 1;
  }
  pcVar4 = &DAT_00bcd630;
  do {
    if ((pcVar4[-0x2e] == 'U') &&
       (((cVar1 = *pcVar4, cVar1 == '\x0f' || (cVar1 == '\x10')) || (cVar1 == '$')))) {
      sVar2 = (&DAT_007999e8)[*(short *)(pcVar4 + 0x24) * 0x84];
      if (sVar2 == 0xe5) {
        local_8[0] = '\x01';
      }
      if (sVar2 == 0xea) {
        local_8[1] = 1;
      }
      if (sVar2 == 0xeb) {
        local_8[2] = 1;
      }
      if (sVar2 == 0xec) {
        local_8[3] = 1;
      }
      sVar2 = (&DAT_007999e8)[*(short *)(pcVar4 + 0x26) * 0x84];
      if (sVar2 == 0xe5) {
        local_8[0] = '\x01';
      }
      if (sVar2 == 0xea) {
        local_8[1] = 1;
      }
      if (sVar2 == 0xeb) {
        local_8[2] = 1;
      }
      if (sVar2 == 0xec) {
        local_8[3] = 1;
      }
    }
    pcVar4 = pcVar4 + 0x184;
  } while ((int)pcVar4 < 0xc8ad70);
  iVar3 = 0;
  do {
    if ((local_8[iVar3 + 4] != '\0') && (local_8[iVar3] == '\0')) {
      switch((int)(iVar3)) {
      case 0: ;
        DAT_006f936c = '\0';
        break;
      case 1: ;
        DAT_006f5694 = '\0';
        break;
      case 2: ;
        DAT_006dea2c = '\0';
        break;
      case 3: ;
        DAT_006e91d4 = '\0';
      }
    }
    iVar3 = iVar3 + 1;
  } while (iVar3 < 4);
  return;
}
