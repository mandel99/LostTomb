#include "restore_types.h"
#define DAT_009784dc (*(undefined4 *)0x009784dcu)
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)
#define DAT_00bcd78a (*(undefined4 *)0x00bcd78au)

void FUN_004a3560(void)

{
  int iVar1;
  char *pcVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  char *pcVar7;
  
  iVar5 = 1;
  pcVar7 = &DAT_00bcd78a;
LAB_004a356d: ;
  if ((*pcVar7 != '\0') && (pcVar7[0x2a] == '\x06')) {
    iVar6 = 0;
    piVar4 = &DAT_009784dc;
    do {
      if ((char)piVar4[-1] != '\0') {
        iVar1 = 0;
        piVar3 = piVar4;
        do {
          if (iVar5 == *piVar3) {
            pcVar2 = &DAT_00bcd786;
            goto LAB_004a35aa;
          }
          iVar1 = iVar1 + 1;
          piVar3 = piVar3 + 1;
        } while (iVar1 < 4);
      }
      piVar4 = piVar4 + 0xb;
      iVar6 = iVar6 + 1;
    } while ((int)piVar4 < 0x978d74);
    goto LAB_004a35ca;
  }
  goto LAB_004a35cd;
  while (pcVar2 = pcVar2 + 0x184, (int)pcVar2 < 0xc8ad42) {
LAB_004a35aa: ;
    if (((pcVar2[4] != '\0') && (*pcVar2 == 'L')) && (*(int *)(pcVar2 + 0x112) == iVar6))
    goto LAB_004a35cd;
  }
LAB_004a35ca: ;
  *pcVar7 = '\x02';
LAB_004a35cd: ;
  pcVar7 = pcVar7 + 0x184;
  iVar5 = iVar5 + 1;
  if (0xc8ad45 < (int)pcVar7) {
    return;
  }
  goto LAB_004a356d;
}
