#include "restore_types.h"
#define DAT_0077f748 (*(undefined4 *)0x0077f748u)

int FUN_00443710(void)

{
  uint uVar1;
  int *piVar2;
  int iVar3;
  char *pcVar4;
  int iVar5;
  int iVar6;
  int local_90 [36];
  
  iVar6 = 0;
  piVar2 = local_90;
  for (iVar3 = 0x24; iVar3 != 0; iVar3 = iVar3 + -1) {
    *piVar2 = 0;
    piVar2 = piVar2 + 1;
  }
  pcVar4 = &DAT_0077f748;
  do {
    if ((pcVar4[1] != '\0') && (*pcVar4 == '\x01')) {
      if (pcVar4[0x18] == 0) {
        iVar3 = 8;
      }
      else if ((6 < (byte)pcVar4[0x18]) || (iVar3 = 0xe, pcVar4[0x1a] == '\0')) goto LAB_0044377e;
      iVar5 = 0;
      if (iVar3 != 0) {
        do {
          uVar1 = (uint)(byte)pcVar4[iVar5 + 0x1e];
          if ((uVar1 != 0) && ((byte)pcVar4[iVar5 + 0x1e] < 9)) {
            local_90[uVar1] = local_90[uVar1] + 1;
          }
          iVar5 = iVar5 + 1;
        } while (iVar5 < iVar3);
      }
    }
LAB_0044377e: ;
    pcVar4 = pcVar4 + 0x62;
    if (0x7843d7 < (int)pcVar4) {
      piVar2 = local_90;
      iVar3 = 0x24;
      do {
        if (*piVar2 != 0) {
          iVar6 = iVar6 + 1;
        }
        piVar2 = piVar2 + 1;
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
      return iVar6;
    }
  } while( true );
}
